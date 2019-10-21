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


//line drawing part 
//function to find zone
int find_zone(){
	double dx=x_en-x_st;
	double dy=y_en-y_st;
	//cout<<"dx = " <<dx<<" "<<dy<<endl;
	if(dx>=0 && dy >=0){
		if(abs(dx)>=abs(dy)) return 0;
		return 1;
	}
	if(dx<=0 && dy>=0){
		if(abs(dy)>=abs(dx)) return 2;
		return 3;
	}
	if(dx<=0 && dy <= 0){
		if(abs(dx)>=abs(dy)) return 4;
		return 5;
	}
	if(dx>=0 && dy<=0){
		if(abs(dy)>=abs(dx)) return 6;
		return 7;
	}
	return 0;
}

double return_x(double x,double y,int pr_zone,int tr_zone,int choice){
		//converting into 0 ghor
		double tx,ty;
		if(pr_zone==0) {
			tx=x;
			ty=y;
		}
		if(pr_zone==1) {
			tx=y;
			ty=x;
		}
		if(pr_zone==2){
			tx=y;
			ty=-x;
		}
		if(pr_zone==3){
			tx=-x;
			ty=y;
		}
		if(pr_zone==4) {
			tx=-x;
			ty=-y;
		}
		if(pr_zone==5) {
			tx=-y;
			ty=-x;
		}
		if(pr_zone==6) {
			tx=-y;
			ty=x;
		}
		if(pr_zone==7) {
			tx=x;
			ty=-y;
		}
			x=tx;
			y=ty;
			if(tr_zone==0) {
				 if(choice==0) return x;
				 else return y;
			}
			if(tr_zone==1) {
				 if(choice==0) return y;
				 else return x;
			}
			if(tr_zone==2) {
				 if(choice==0) return -y;
				 else return x;
			}
			if(tr_zone==3) {
				 if(choice==0) return -x;
				 else return y;
			}
			if(tr_zone==4) {
				 if(choice==0) return -x;
				 else return -y;
			}
			if(tr_zone==5) {
				 if(choice==0) return -y;
				 else return -x;
			}
			if(tr_zone==6) {
				 if(choice==0) return y;
				 else return -x;
			}
			if(tr_zone==7) {
				 if(choice==0) return x;
				 else return -y;
			}
			return -1;
}

void line_draw(int _mod){
	
	
	
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);

	int zone = find_zone();
	if(_mod==0) glColor3f(1.0, 1.0, 1.0);
	if(_mod==1) glColor3f(0.0, 1.0, 0.0);
	if(_mod==2) glColor3f(0.0, 0.0, 1.0);
	if(_mod==3) glColor3f(1.0, 1.0, 1.0);
	if(_mod==4) glColor3f(0.2, 0.5, 1.0);
	if(_mod==5) glColor3f(0.0, 0.3, 0.7);
	/*if(zone==4) glColor3f(0.5, 0.0, 0.0);
	if(zone==5) glColor3f(1.0, 1.0, 0.0);
	if(zone==6) glColor3f(0.4, 0.5, 0.0);
	if(zone==7) glColor3f(0.9, 0.0, 0.4);*/

	double x1=return_x(x_st,y_st,zone,0,0);
	double y1=return_x(x_st,y_st,zone,0,1);
	double x2=return_x(x_en,y_en,zone,0,0);
	double y2=return_x(x_en,y_en,zone,0,1);
	
	//cout<<"x1 = " <<x1<<" "<<y1<<endl;
	
	if(x1>x2){
		double temp=x1;
		x1=x2;
		x2=temp;

		temp=y1;
		y1=y2;
		y2=temp;
	}
	//cout<<"x1 = " <<x1<<" "<<y1<<endl;
	//cout<<"x2 = " <<x2<<" "<<y2<<endl;
	double x = x1;
	double y = y1;

	double dx=x2-x1;
	double dy=y2-y1;
	double d = 2*dy - dx;
	double d_n= 2*dy;
	double d_ne=2*(dy-dx);

	//printf("zone %d\n",zone);
	double tempx=return_x(x,y,0,zone,0);
	double tempy=return_x(x,y,0,zone,1);

	glVertex2i(round(tempx),round(tempy));
	//cout<<"main x " << x<<" "<<y<<endl;
	//cout<<tempx<<" "<<tempy<<endl;
	//printf("x = %d y = %d\n",tempx,tempy);
	while(x<x2){
		if(d<0){
			x++;
			d=d+d_n; //error bare
		}
		else{
			x++;
			y++;
			d=d+d_ne; //error kome
		}
		//cout<<"x = " <<x<<" "<<y<<endl;
		tempx=return_x(x,y,0,zone,0);
		tempy=return_x(x,y,0,zone,1);
		glVertex2i(round(tempx),round(tempy));
		//cout<<"aki " << endl;
		//cout<<tempx<<" "<<tempy<<endl;
		//printf("x = %d y = %d\n",tempx,tempy);
	}
	//printf("ber hoi\n");
    tempx=x_en;
    tempy=y_en;
	glVertex2i(round(tempx),round(tempy));
	glEnd();
	return;
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
			
			//line_draw();
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
	counter = 0;
	for(double i=y_min;i<y_max;i++){
		//edge niye asi jeigular ymin i er shoman
		counter = return_edge(i); 
		// x er basis e sort kori 
		sort(temp_edge_table+1,temp_edge_table+counter,sort_edge_table); 
		//scan line 
		for(int j=1;j<=counter && (j+1)<=counter; j+= 2) {
			//draw line between j and j+1
			//make set of lines 
			x_st = temp_edge_table[j].x;
			y_st = temp_edge_table[j].ymin;
			x_en = temp_edge_table[j+1].x;
			y_en = temp_edge_table[j+1].ymin;
			//tot_line++;
			//printf("x_st = %lf y_st = %lf x_en = %lf y_en = %lf\n",x_st,y_st,x_en,y_en);
			line_draw(mod); 
		}
		//update 
		//now change the edges 
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
	//if(shade>=0) {
		active_list();
	//}
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
