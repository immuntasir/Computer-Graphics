#include <bits/stdc++.h>
#include <GL/glut.h>
using namespace std;
#define MAX 7000
#define MAX_LINE 25
#define PI acos(-1.0)
#define INF 1000000000

int given_input,connection;
int mod;

struct Point{
	double x1,y1,z1; 
};
Point point[MAX+1]; 

struct Line{
	double x1,y1,z1,x2,y2,z2;
};

struct Surface{
	int n;
	Line line[MAX_LINE+1]; 
};

Surface surface[MAX+1]; 

struct Vector{
	double x,y,z;
};


int d,m;
double x_st,y_st,x_en,y_en;
double angle;
int choice;
bool flag;

void init2D(double r, double g, double b){
	glClearColor(r,g,b,0.0);
	glMatrixMode (GL_PROJECTION);
	gluOrtho2D (-1000,1000,-1000,1000);
}


int findZone(double X0, double Y0, double X1, double Y1) {
	double dx = X1 - X0;
	double dy = Y1 - Y0;
	if (dx >= 0 && dy >= 0) {
		if (fabs(dx) > fabs(dy)) return 0;
		else return 1;
	}
	else if (dx<=0 && dy>=0) {
		if (fabs(dx) < fabs(dy)) return 2;
		return 3;
	}
	else if (dx <=0 && dy<=0) {
		if (fabs(dx) > fabs(dy)) return 4;
		return 5;
	}
	else if (dx >=0 && dy <= 0) {
		if (fabs(dx) < fabs(dy)) return 6;
		return 7;
	}
}

void drawPixel(int X, int Y) {
	glVertex2d((X*2) / WINDOW_WIDTH, (Y*2)/WINDOW_HEIGHT );
}
void drawPixelbyZone(int X, int Y, int zone) {
    int Xp, Yp;
    //cout << X << " " << Y << endl;
    if (zone == 0)  {Xp = X; Yp = Y;}
    else if (zone == 1) {Xp = Y; Yp = X;}
    else if (zone == 2) {Xp = -Y; Yp = X;}
    else if (zone == 3) {Xp = -X; Yp = Y;}
    else if (zone == 4) {Xp = -X; Yp = -Y;}
    else if (zone == 5) {Xp = -Y; Yp = -X;}
    else if (zone == 6) {Xp = Y; Yp = -X;}
    else if (zone == 7) {Xp = X; Yp = -Y;}
    //cout << (Xp*2) / WINDOW_WIDTH << " " << (Yp*2)/WINDOW_HEIGHT << endl;
	glVertex2d(double(Xp*2) / WINDOW_WIDTH, double(Yp*2)/WINDOW_HEIGHT );
}
void drawLineZone0 (int X1, int Y1, int X2, int Y2, int zone) {
    int dx = X2 - X1;
    int dy = Y2 - Y1;

    int d = dy - (dx/2);
    int X = X1, Y = Y1;
    //cout << X << " " << Y << endl;
    drawPixelbyZone(X, Y, zone);

    while (X < X2) {
        X++;
        if (d < 0)
            d = d + dy;
        else {
            d += (dy - dx);
            Y++;
        }
        drawPixelbyZone(X, Y, zone);
    }
}
void drawLine(double _X0, double _Y0, double _X1, double _Y1) {
	int X0, Y0, X1, Y1;
    X0 = round(_X0);
    Y0 = round(_Y0);
    X1 = round(_X1);
    Y1 = round(_Y1);
    int zone = findZone(X0, Y0, X1, Y1);

    //glColor3f(1.0, 0.0, 0.0);

    if (zone == 0) drawLineZone0(X0, Y0, X1, Y1, zone);
	else if (zone == 1) drawLineZone0(Y0, X0, Y1, X1, zone);
	else if (zone == 2) drawLineZone0(Y0, -X0, Y1, -X1, zone);
	else if (zone == 3) drawLineZone0(-X0, Y0, -X1, Y1, zone);
	else if (zone == 4) drawLineZone0(-X0, -Y0, -X1, -Y1, zone);
	else if (zone == 5) drawLineZone0(-Y0, -X0, -Y1, -X1, zone);
	else if (zone == 6) drawLineZone0(-Y0, X0, -Y1, X1, zone);
	else if (zone == 7) drawLineZone0(X0, -Y0, X1, -Y1, zone);

}
int choice, c2;
int n;
struct rectangle {
    int ymax, ymin, xmax, xmin;
} temp, t2;
typedef struct li {
    vector < double > A;
    vector < double > B;
} Line;
Line makeLine (double _X0, double _Y0, double _Z0, double _X1, double _Y1, double _Z1) {
    Line ret;
    ret.A.push_back(_X0);
    ret.A.push_back(_Y0);
    ret.A.push_back(_Z0);
    ret.B.push_back(_X1);
    ret.B.push_back(_Y1);
    ret.B.push_back(_Z1);
    return ret;
}
vector <Line> lines;

struct rectangle makeRectangle (int _xmin, int _ymin, int _xmax, int _ymax) {
    struct rectangle rect;
    rect.xmin = _xmin;
    rect.ymin = _ymin;
    rect.xmax = _xmax;
    rect.ymax = _ymax;
    return rect;
}
void draw_axis() {
	glColor3f(0.0,1.0,0.0);
	drawLine(-320, 0, 320, 0);
	drawLine(0, 240, 0, -240);
}
void drawRectangle(struct rectangle Rect) {
    drawLine(Rect.xmin, Rect.ymin, Rect.xmax, Rect.ymin);
    drawLine(Rect.xmin, Rect.ymin, Rect.xmin, Rect.ymax);
    drawLine(Rect.xmax, Rect.ymax, Rect.xmax, Rect.ymin);
    drawLine(Rect.xmax, Rect.ymax, Rect.xmin, Rect.ymax);
}
void drawLines(vector <Line> newlines) {
    for (int i=0; i<newlines.size(); i++) {
        drawLine(newlines[i].A[0], newlines[i].A[1], newlines[i].B[0], newlines[i].B[1]);
    }
}
void draw3DLinesIn2D (vector <Line> newlines, double depth) {
    glBegin(GL_POINTS);
    for (int i=0; i<newlines.size(); i++) {
        drawLine((newlines[i].A[0] + offsetx)/(1 + ((newlines[i].A[2]+offsetz)/depth)),
                            (newlines[i].A[1]+offsety)/(1 + ((newlines[i].A[2]+offsetz)/depth)),
                 (newlines[i].B[0]+offsetx)/(1 + ((newlines[i].B[2]+offsetz)/depth)),
                            (newlines[i].B[1]+offsety)/(1 + ((newlines[i].B[2]+offsetz)/depth)));
    }
    glEnd( );
	glFlush();
}
vector < vector  <double> > makeZeroVector (int n, int m) {
    vector <vector <double> > ret;
    vector <double> temp;
    for (int i=0; i<m; i++) temp.push_back(0);
    for (int i=0; i<n; i++) ret.push_back(temp);
    return ret;
}
vector <vector <double> > makeRotateX (double degree) {
    vector <vector <double> > ret = makeZeroVector(3, 3);
    ret[0][0] = 1;
    ret[1][1] = cos(degree);
    ret[1][2] = -sin(degree);
    ret[2][1] = sin(degree);
    ret[2][2] = cos(degree);
    return ret;
}
vector <vector <double> > makeRotateY (double degree) {
    vector <vector <double> > ret = makeZeroVector(3, 3);
    ret[1][1] = 1;
    ret[0][0] = cos(degree);
    ret[0][2] = sin(degree);
    ret[2][0] = -sin(degree);
    ret[2][2] = cos(degree);
    return ret;
}
vector <vector <double> > makeRotateZ (double degree) {
    vector <vector <double> > ret = makeZeroVector(3, 3);
    ret[2][2] = 1;
    ret[0][0] = cos(degree);
    ret[0][1] = -sin(degree);
    ret[1][0] = sin(degree);
    ret[1][1] = cos(degree);
    return ret;
}
vector < double > matMultiply (vector <vector < double> > A, vector < double > x) {
    vector < double >  ret;
    ret.push_back(0);
    ret.push_back(0);
    ret.push_back(0);
    for (int i=0; i<A.size(); i++) {
        for (int j=0; j<1; j++) {
            for (int k=0; k<A[0].size(); k++) {
                ret[i] = ret[i] + A[i][k]*x[k];
            }
        }
    }
    return ret;
}
void rotationx (double degree) {
    vector <Line> newlines;
    Line tmp;
    for (int i=0; i<lines.size(); i++) {
        tmp.A = matMultiply(makeRotateX(degree), lines[i].A);
        tmp.B = matMultiply(makeRotateX(degree), lines[i].B);
        newlines.push_back(tmp);
    }
    lines = newlines;
    drawFrame(lines);
}

void rotationy(double degree) {
    vector <Line> newlines;
    Line tmp;
    for (int i=0; i<lines.size(); i++) {
        tmp.A = matMultiply(makeRotateY(degree), lines[i].A);
        tmp.B = matMultiply(makeRotateY(degree), lines[i].B);
        newlines.push_back(tmp);
    }
    lines = newlines;
    drawFrame(lines);
}

void rotationz(double degree) {
    vector <Line> newlines;
    Line tmp;
    for (int i=0; i<lines.size(); i++) {
        tmp.A = matMultiply(makeRotateZ(degree), lines[i].A);
        tmp.B = matMultiply(makeRotateZ(degree), lines[i].B);
        newlines.push_back(tmp);
    }
    lines = newlines;
    drawFrame(lines);
}
void rotate_left() {
	rotationx(DEGREE);
}

void rotate_right() {
	rotationx(-DEGREE);
}

void rotate_up() {
	rotationy(DEGREE);
}

void rotate_down() {
	rotationy(-DEGREE);
}
void rotate_z_cl() {
	rotationz(DEGREE);
}

void rotate_z_acl() {
	rotationz(-DEGREE);
}
void move(unsigned char input, int x, int y) {
	printf("%d pressed\n", input);
	switch(input) {
		case 27: break;
		case 97: offsetx+=1; break;
        case 119: offsety+=1; break;
        case 100: offsetx-=1; break;
		case 115: offsety-=1; break;
        case 113: offsetz+=1; break;
        case 101: offsetz-=1; break;
        case 106: rotate_left(); break;
        case 105: rotate_up(); break;
        case 108: rotate_right(); break;
		case 107: rotate_down(); break;
        case 117: rotate_z_acl(); break;
        case 111: rotate_z_cl(); break;
       // case 120: reset(); break;
    }
    drawFrame(lines);
}
void general_rotation(double x,double y, double z, double &x1, double &y1, double &z1){
	double value = (angle * PI)/180.0;
	x1 = x * cos(value) - y * sin(value);
	y1 = x * sin(value) + y * cos(value);
	z1 = z;
	return;
}




void general_projection(double x, double y, double z, double &x1, double &y1, double &z1, int d,int m){
	double tx1=(x)/(1+(z+m)/d);
	double ty1=(y)/(1+(z+m)/d);
	double tz1=(z)/(1+((z+m)/d));
	
	x1 = tx1;
	y1 = ty1;
	z1 = tz1;
}

void controlling(){
	
	glClear(GL_COLOR_BUFFER_BIT);
	d=2500; 
	m=1500;
	
	for(int i=0;i<connection;i++){
		for(int j=0;j<surface[i].n;j++){
			double x1,y1,z1;
			general_projection(surface[i].line[j].x1,surface[i].line[j].y1,surface[i].line[j].z1,x1,y1,z1,d,m);
			
			double x2,y2,z2;
			general_projection(surface[i].line[j].x2,surface[i].line[j].y2,surface[i].line[j].z2,x2,y2,z2,d,m);
			
			x_st=x1;
			y_st=y1;
			x_en=x2;
			y_en=y2;
			
		}
	}
}

//For polygon filling 
struct Edge_table{
	double ymin;
	double ymax;
	double x;
	double dx;
	double dy;
	int sign;
	double new_x;
	double new_y;
};
Edge_table edge_table[MAX+1],temp_edge_table[MAX+1]; //Full edge table and koto tuku niye kaj kortesi 
double Y_MIN,Y_MAX;
int n; //For counting lines 
int pointer_edge_table=0,counter=0;
int tot_line;

//function to sort edge table 
bool sort_edge_table(const Edge_table &a, const Edge_table &b){
	if(a.ymin<b.ymin) return true;
	else if(a.ymin == b.ymin){
		if(a.x<b.x) return true;
		if(a.x==b.x) return true;
		else return false;
	}
	return false;
} 

//function to make edge table 
void make_edge_table(double x1, double y1, double x2, double y2,int number){
	
	int guni = number;
	if(y1<y2) {
		edge_table[number].ymin = y1;
		edge_table[guni].ymax = y2;
		edge_table[guni].x = x1 ;
		edge_table[guni].dx = abs(x2-x1);
		edge_table[guni].dy = abs(y2 - y1);
		edge_table[guni].new_x = x1;
		edge_table[guni].new_y = y1;
		if(((x2-x1)*(y2-y1))>=0){
				edge_table[guni].sign=1;
		}
		else{
			 edge_table[guni].sign=-1;
		}
	}
	else {
		edge_table[number].ymin = y2;
		edge_table[guni].ymax = y1;
		edge_table[guni].x = x2 ;
		edge_table[guni].dx = abs(x2-x1);
		edge_table[guni].dy = abs(y2 - y1);
		edge_table[guni].new_x = x2;
		edge_table[guni].new_y = y2;
		if(((x2-x1)*(y2-y1))>=0){
				edge_table[guni].sign=1;
		}
		else{
			 edge_table[guni].sign=-1;
		}
	}
}


//return those edeges where ymin == desired ymin
int return_edge(double ymin){
	//start from pointer
    counter = 0;
	for(int i=pointer_edge_table;i<n;i++){
		if(edge_table[i].ymin == ymin){
			counter++;
			temp_edge_table[counter]=edge_table[i];
			pointer_edge_table++;
		}
		else {
			break;
		}
	}
	return counter; 
}

//The working function 
void active_list(){
	double y_min,y_max;
	y_min = Y_MIN;
	y_max = Y_MAX;
	pointer_edge_table=0;
	for(double i=y_min;i<y_max;i++){
		counter = return_edge(i); 
		sort(temp_edge_table+1,temp_edge_table+counter,sort_edge_table); 
		for(int j=1;j<=counter && (j+1)<=counter; j+= 2) {
			x_st = temp_edge_table[j].x;
			y_st = temp_edge_table[j].ymin;
			x_en = temp_edge_table[j+1].x;
			y_en = temp_edge_table[j+1].ymin;
			drawLine(x_st, y_st, x_en, y_en);
		}
		int second_counter = 0;
		for(int j=1;j<=counter;j++){
			temp_edge_table[j].ymin++;
			if(temp_edge_table[j].ymin != temp_edge_table[j].ymax){
				second_counter++;
				temp_edge_table[second_counter].ymin=temp_edge_table[j].ymin;
				temp_edge_table[second_counter].ymax=temp_edge_table[j].ymax;
				temp_edge_table[second_counter].dx=temp_edge_table[j].dx;
				temp_edge_table[second_counter].dy=temp_edge_table[j].dy;
				temp_edge_table[second_counter].sign=temp_edge_table[j].sign;
				temp_edge_table[second_counter].new_x=temp_edge_table[j].new_x;
				temp_edge_table[second_counter].new_y=temp_edge_table[j].new_y;
				if(temp_edge_table[second_counter].dy != 0){ //x will change 
						double lob = (double)temp_edge_table[j].dx*1.0;
						double hor = (double)temp_edge_table[j].dy*1.0;
						double frac = (double)temp_edge_table[j].new_x+1.0*(temp_edge_table[j].sign)*(lob/hor)*(temp_edge_table[j].ymin-temp_edge_table[j].new_y);
						temp_edge_table[second_counter].x = frac;
				}
				else{
					temp_edge_table[second_counter].x=temp_edge_table[j].x;
				}
			}
		}
		counter = second_counter;
	}
}

double vector_value(Vector a) {
	double value = a.x*a.x + a.y * a.y + a.z * a.z;
	value = sqrt(value);
	return value;
}

Vector cross_product(Vector a, Vector b){
	Vector result;
	result.x = a.y * b.z - b.y * a.z;
	result.y = b.x * a.z - a.x * b.z;
	result.z = a.x * b.y - b.x * a.y ;
	double value = vector_value(result);
	result.x /= value;
	result.y /= value;
	result.z /= value;
	return result;
}

Vector vorkendro(Surface temp) {
		Vector result;
		result.x=0;
		result.y=0;
		result.z=0;
		for(int i=0;i<temp.n;i++) {
			result.x = result.x + temp.line[i].x1;
			result.y = result.y + temp.line[i].y1;
			result.z = result.z + temp.line[i].z1;
		}
		result.x /= temp.n;
		result.y /= temp.n;
		result.z /= temp.n;
		return result;
}

Vector light_normal_vector(Vector P) {
	Vector S,L;
	S.x=0;
	S.y=0;
	S.z=2500;
	
	L.x = P.x- S.x;
	L.y = P.y- S.y;
	L.z = P.z- S.z;
	return L;
}

double dot_product(Vector a, Vector b){
	double value;
	value = a.x * b.x + a.y * b.y + a.z * b.z;
	return value;
}

int shading(Surface temp){
	Vector N,P,One,Two,L;
	P.x=0;
	P.y=0;
	P.z=0;
	One.x = temp.line[0].x2 - temp.line[0].x1;
	One.y = temp.line[0].y2 - temp.line[0].y1;
	One.z = temp.line[0].z2 - temp.line[0].z1;
	
	Two.x = temp.line[1].x2 - temp.line[1].x1;
	Two.y = temp.line[1].y2 - temp.line[1].y1;
	Two.z = temp.line[1].z2 - temp.line[1].z1;
	
	N = cross_product(One,Two);
	P = vorkendro(temp);
	L = light_normal_vector(P);
	
	double result = dot_product(N,L);
	if(result<0) return -1;
	if(result>=0) return 1;
		
}

void polygon_filling(Surface temp){
	Y_MIN = INF; 
	Y_MAX = -INF;
	int guni = 0; 
	for(int i=0;i<temp.n;i++) {
		Y_MIN = min(Y_MIN,round(temp.line[i].y1));
		Y_MAX = max(Y_MAX,round(temp.line[i].y1));
		Y_MAX = max(Y_MAX,round(temp.line[i].y2));
		Y_MIN = min(Y_MIN,round(temp.line[i].y2));
		if(temp.line[i].y1 != temp.line[i].y2) {
			make_edge_table(round(temp.line[i].x1),round(temp.line[i].y1),round(temp.line[i].x2),round(temp.line[i].y2),guni);
			guni++;
		}
	}
	n=guni; 
	sort(edge_table,edge_table+n,sort_edge_table);
	int shade = shading(temp);
    active_list();
}


void controlling2(){
	glClear(GL_COLOR_BUFFER_BIT);
	d=2500; 
	m=1500;
	
	for(int i=0;i<connection;i++){
		Surface temp;
		temp.n = surface[i].n;
		for(int j=0;j<surface[i].n;j++){
			
			double x1,y1,z1,x2,y2,z2;
			
			
			if(choice==0) {
				//around z axis 
				general_rotation(surface[i].line[j].x1,surface[i].line[j].y1,surface[i].line[j].z1,x1,y1,z1);
				general_projection(x1,y1,z1,x1,y1,z1,d,m);
				
				general_rotation(surface[i].line[j].x2,surface[i].line[j].y2,surface[i].line[j].z2,x2,y2,z2);
				general_projection(x2,y2,z2,x2,y2,z2,d,m);
			}
			else if(choice==1) {
				general_rotation(surface[i].line[j].y1,surface[i].line[j].z1,surface[i].line[j].x1,y1,z1,x1);
				general_projection(x1,y1,z1,x1,y1,z1,d,m);
				
				general_rotation(surface[i].line[j].y2,surface[i].line[j].z2,surface[i].line[j].x2,y2,z2,x2);
				general_projection(x2,y2,z2,x2,y2,z2,d,m);
			}
			else if(choice==2) {
				general_rotation(surface[i].line[j].z1,surface[i].line[j].x1,surface[i].line[j].y1,z1,x1,y1);
				general_projection(x1,y1,z1,x1,y1,z1,d,m);
				
				general_rotation(surface[i].line[j].z2,surface[i].line[j].x2,surface[i].line[j].y2,z2,x2,y2);
				general_projection(x2,y2,z2,x2,y2,z2,d,m);
			}
			
			
			temp.line[j].x1 = x1;
			temp.line[j].y1 = y1;
			temp.line[j].z1 = z1;
			
			temp.line[j].x2 = x2;
			temp.line[j].y2 = y2;
			temp.line[j].z2 = z2;
			
			x_st = x1;
			y_st = y1;
			x_en = x2;
			y_en = y2;
			mod=0;
			//line_draw(mod);
			
		}
		mod=i;
		polygon_filling(temp);
		
	}
	glFlush(); 
}




void input(){
	scanf("%d %d",&given_input,&connection);
	for(int i=0;i<given_input;i++){
		scanf("%lf %lf %lf",&point[i].x1,&point[i].y1,&point[i].z1);
		if(flag) {
			point[i].x1 *= 500/100;
			point[i].y1 *= 500/100;
			point[i].z1 *= 500/100;
		}
		//cout<<point[i].x1<<" "<<point[i].y1<<" "<<point[i].z1<<endl;
	}
	//cout<<given_input<<" "<<connection<<endl;
	for(int i=0;i<connection;i++){
		scanf("%d",&surface[i].n);
		//cout<<surface[i].n<<endl;
		int target[MAX_LINE+1];
		for(int j=0;j<surface[i].n;j++) {
			scanf("%d",&target[j]);
			//cout<<target[j]<<endl;
		}
		for(int j=0;j<surface[i].n;j++) {
			int one = target[j];
			int two = target[(j+1)%surface[i].n];
			
			//cout<<one<<" "<<two << endl;
			
			surface[i].line[j].x1 = point[one].x1;
			surface[i].line[j].y1 = point[one].y1;
			surface[i].line[j].z1 =	point[one].z1;
			
			surface[i].line[j].x2 = point[two].x1;
			surface[i].line[j].y2 =	point[two].y1;
			surface[i].line[j].z2 = point[two].z1;
		}
	}
	angle=0;
}

void catchKey(int key, int x, int y){
	if(key == GLUT_KEY_LEFT) {
		//printf("Left key is pressed\n");
		//printf("moving around x axis\n");
		angle=angle+1;
		choice=1;
		controlling2();
	}        
    else if(key == GLUT_KEY_RIGHT) {
		// printf("Right key is pressed\n");
		 //printf("moving around y axis\n");
		 angle=angle+1;
		 choice=2;
		 controlling2();
	}  
    else if(key == GLUT_KEY_DOWN) {
		 //printf("Down key is pressed\n");
		 //printf("moving around z axis\n");
		 angle=angle-1;
		 choice=0;
		 controlling2();
		 
	}  
    else if(key == GLUT_KEY_UP) {
        //printf("Up key is pressed\n");
        //printf("moving around z axis\n");
        angle=angle+1;
		choice=0;
		controlling2();
     }
}



int main(int argc, char **argv) {
	flag=false;
	freopen("airplanetest.ply","r",stdin);
	input();
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (600,600);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("points and lines");
	init2D(0.0,0.0,0.0);
	glutSpecialFunc(catchKey);
	glutDisplayFunc(controlling2);
	glutMainLoop();
	
}
