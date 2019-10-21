/*
This code implements roatation along an specified axis.

The code has two options.
Youpo may choose to draw n random lines, either from origin, or from anywhere.
freeglut was used in this code.

You must have freeglut and freeglut-dev installed. In Linux, run the following commands:
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold

To run the code, enter the following commands
1. g++ cow.cpp -o t -lGL -lGLU -lglut
2. ./t
*/

#define DEBUG 0
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define INF 1000000000
#define MAX 7000
#include <bits/stdc++.h>
using namespace std;

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/freeglut.h>
#include "GL/gl.h"
#define MAXLENGTH 1000
#define YMAX 124
#define XMAX 124
#define YMIN -124
#define XMIN -124
#define point_offset 300
#define INSIDE 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
#define DEGREE  (3.14159/180)
#define DEPTH 2500
double offsetx = 0;
double offsety = 0;
double offsetz = 7000;

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
Edge_table edge_table[MAX+1],temp_edge_table[MAX+1];
typedef struct Vertex {
    double x, y, z;
} vertex;
vertex makeVertex (double X, double Y, double Z) {
    vertex ret;
    ret.x = X; ret.y = Y; ret.z = Z;
    return ret;
}
typedef struct Face {
    int num;
    vector <Line> lines;
} face;
face makeFace (int num, vector <Line> lines) {
    face ret;
    ret.num = num; ret.lines = lines;
    return ret;
}
vector <vertex> vertices;
vector <face> faces;
vector <face> projected;
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
int cnt = 11;
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

void draw_axis() {
	glBegin(GL_POINTS);
    glColor3f(0.0,1.0,0.0);
	drawLine(-500, 0, 500, 0);
	drawLine(0, -500, 0, 500);
    glEnd( );
	glFlush();
}
void drawRectangle(struct rectangle Rect) {
    drawLine(Rect.xmin, Rect.ymin, Rect.xmax, Rect.ymin);
    drawLine(Rect.xmin, Rect.ymin, Rect.xmin, Rect.ymax);
    drawLine(Rect.xmax, Rect.ymax, Rect.xmax, Rect.ymin);
    drawLine(Rect.xmax, Rect.ymax, Rect.xmin, Rect.ymax);
}
void draw3DLinesIn2D (vector <Line> newlines, double depth) {
    for (int i=0; i<newlines.size(); i++) {
        drawLine((newlines[i].A[0] + offsetx)/(1 + ((newlines[i].A[2]+offsetz)/depth)),
                            (newlines[i].A[1]+offsety)/(1 + ((newlines[i].A[2]+offsetz)/depth)),
                 (newlines[i].B[0]+offsetx)/(1 + ((newlines[i].B[2]+offsetz)/depth)),
                            (newlines[i].B[1]+offsety)/(1 + ((newlines[i].B[2]+offsetz)/depth)));
    }
}
//Full edge table and koto tuku niye kaj kortesi
double Y_MIN,Y_MAX;
 //For counting lines
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

void make_edge_table(double x1, double y1, double x2, double y2,int number){

	int counter = number;
	if(y1<y2) {
		edge_table[number].ymin = y1;
		edge_table[counter].ymax = y2;
		edge_table[counter].x = x1 ;
		edge_table[counter].dx = abs(x2-x1);
		edge_table[counter].dy = abs(y2 - y1);
		edge_table[counter].new_x = x1;
		edge_table[counter].new_y = y1;
		if(((x2-x1)*(y2-y1))>=0){
				edge_table[counter].sign=1;
		}
		else{
			 edge_table[counter].sign=-1;
		}
	}
	else {
		edge_table[number].ymin = y2;
		edge_table[counter].ymax = y1;
		edge_table[counter].x = x2 ;
		edge_table[counter].dx = abs(x2-x1);
		edge_table[counter].dy = abs(y2 - y1);
		edge_table[counter].new_x = x2;
		edge_table[counter].new_y = y2;
		if(((x2-x1)*(y2-y1))>=0){
				edge_table[counter].sign=1;
		}
		else{
			 edge_table[counter].sign=-1;
		}
	}
}


int return_edge(double ymin){
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

void active_list(){
	double y_min,y_max;
	y_min = Y_MIN;
	y_max = Y_MAX;
	pointer_edge_table=0;
	counter = 0;
	for(double i=y_min;i<y_max;i++){
		counter = return_edge(i);
		sort(temp_edge_table+1,temp_edge_table+counter,sort_edge_table);
		double x_st, y_st, x_en, y_en;
		vector <Line> tmplines;
		for(int j=1;j<=counter && (j+1)<=counter; j+= 2) {
			if (DEBUG) printf("\n %lf %lf %lf %lf\n", x_st, y_st, x_en, y_en);
			x_st = temp_edge_table[j].x;
			y_st = temp_edge_table[j].ymin;
			x_en = temp_edge_table[j+1].x;
			y_en = temp_edge_table[j+1].ymin;
            if (DEBUG) printf("x_st = %lf y_st = %lf x_en = %lf y_en = %lf\n",x_st,y_st,x_en,y_en);
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
void polygon_filling(Face temp){
	Y_MIN = INF;
	Y_MAX = -INF;
	int counter = 0;

	for(int i=0;i<temp.num;i++) {
		Y_MIN = min(Y_MIN,round(temp.lines[i].A[1]));
		Y_MAX = max(Y_MAX,round(temp.lines[i].A[1]));
		Y_MAX = max(Y_MAX,round(temp.lines[i].B[1]));
		Y_MIN = min(Y_MIN,round(temp.lines[i].B[1]));
		if(temp.lines[i].A[1] != temp.lines[i].B[1]) {
            //printf("%lf %lf %lf %lf\n", round(temp.lines[i].A[0]),round(temp.lines[i].A[1]),round(temp.lines[i].B[0]),round(temp.lines[i].B[1]));
			make_edge_table(round(temp.lines[i].A[0]),round(temp.lines[i].A[1]),round(temp.lines[i].B[0]),round(temp.lines[i].B[1]), counter);
			counter++;
		}
	}
	n=counter;
	if (DEBUG) printf("%d\n", n);
	sort(edge_table,edge_table+n,sort_edge_table);
	//for (int i=0; i<n; i++) printf("%lf %lf\n", edge_table[i].ymin, edge_table[i].ymax);
	active_list();
}
vector <Face> projectFaces (vector <Face> tmpfaces) {
    vector <Face> ret;
    double depth = DEPTH;
    for (int i=0; i<tmpfaces.size(); i++) {
        vector <Line> tmplines;
        if (DEBUG) printf("a\n");
        if (DEBUG) printf("%d\n", tmpfaces[i].num);
        vector <Line> newlines;
        newlines = tmpfaces[i].lines;

        for (int j=0; j<tmpfaces[i].num; j++) {
            if (newlines[j].A.size() != 3 || newlines[j].B.size() != 3) continue;
            tmplines.push_back(makeLine((newlines[j].A[0] + offsetx)/(1 + ((newlines[j].A[2]+offsetz)/depth)),
                            (newlines[j].A[1]+offsety)/(1 + ((newlines[j].A[2]+offsetz)/depth)),
                                        0,
                 (newlines[j].B[0]+offsetx)/(1 + ((newlines[j].B[2]+offsetz)/depth)),
                            (newlines[j].B[1]+offsety)/(1 + ((newlines[j].B[2]+offsetz)/depth)),
                               0));


        }
        ret.push_back(makeFace(tmplines.size(), tmplines));
    }
    return ret;
}
void drawFrame(vector <Line> newlines, vector <Face> faces) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    draw_axis();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    //draw3DLinesIn2D(newlines, DEPTH);
    projected = projectFaces(faces) ;
    if (DEBUG) printf("Projection done\n");
    for (int i=0; i<projected.size(); i++) {
        //printf("try so hard.\n");
        polygon_filling(projected[i]);
    }
    glEnd( );
	glFlush();
}
void draw() {
 	glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glPointSize(2);
    double X0, X1, Y0, Y1;
	int i;
    time_t t;

    drawFrame(lines, faces);

    glFlush();
    glutSwapBuffers();
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
    for (int i=0; i<faces.size(); i++) {
        for (int j=0; j<faces[i].num; j++) {
            tmp.A = matMultiply(makeRotateX(degree), faces[i].lines[j].A);
            tmp.B = matMultiply(makeRotateX(degree), faces[i].lines[j].B);
            faces[i].lines[j] = tmp;
        }
    }
    for (int i=0; i<lines.size(); i++) {
        tmp.A = matMultiply(makeRotateX(degree), lines[i].A);
        tmp.B = matMultiply(makeRotateX(degree), lines[i].B);
        newlines.push_back(tmp);
    }

    lines = newlines;
   // drawFrame(lines);
}

void rotationy(double degree) {
    vector <Line> newlines;
    Line tmp;
    for (int i=0; i<faces.size(); i++) {
        for (int j=0; j<faces[i].num; j++) {
            tmp.A = matMultiply(makeRotateY(degree), faces[i].lines[j].A);
            tmp.B = matMultiply(makeRotateY(degree), faces[i].lines[j].B);
            faces[i].lines[j] = tmp;
        }
    }
    for (int i=0; i<lines.size(); i++) {
        tmp.A = matMultiply(makeRotateY(degree), lines[i].A);
        tmp.B = matMultiply(makeRotateY(degree), lines[i].B);
        newlines.push_back(tmp);
    }
    lines = newlines;
    //drawFrame(lines);
}

void rotationz(double degree) {
    vector <Line> newlines;
    Line tmp;
    for (int i=0; i<faces.size(); i++) {
        for (int j=0; j<faces[i].num; j++) {
            tmp.A = matMultiply(makeRotateZ(degree), faces[i].lines[j].A);
            tmp.B = matMultiply(makeRotateZ(degree), faces[i].lines[j].B);
            faces[i].lines[j] = tmp;
        }
    }
    for (int i=0; i<lines.size(); i++) {
        tmp.A = matMultiply(makeRotateZ(degree), lines[i].A);
        tmp.B = matMultiply(makeRotateZ(degree), lines[i].B);
        newlines.push_back(tmp);
    }
    lines = newlines;
    //drawFrame(lines);
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
	if (DEBUG) printf("%d pressed\n", input);
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
    drawFrame(lines, faces);
}
int nV, nF;
void removeHeader (FILE *fd) {
    char str[100];
    while (fscanf(fd, "%s", str)) {
        if (strcmp("vertex", str) == 0) {
            fscanf(fd, "%d", &nV);
        }
        else if (strcmp("face", str) == 0) {
            fscanf(fd, "%d", &nF);
        }
        else if (strcmp("end_header", str) == 0 ) return;
    }
}

int main(int argc, char **argv) {
    FILE * pFile;
    pFile = fopen ("cow.ply","r");
    double d;
    removeHeader(pFile);
    //fscanf(pFile, "%lf",&d);
    printf("Number of vertices: %d\n", nV);
    printf("Number of face: %d\n", nF);
    //printf("the point %lf\n", d);
    double a, b, c;
    for (int i=0; i<nV; i++) {
        fscanf(pFile, "%lf %lf %lf", &a, &b, &c);
        if (i<10) printf("%lf %lf %lf\n", a, b, c);
        vertices.push_back(makeVertex(a*point_offset, b*point_offset, c*point_offset));
    }
    for (int i=0; i<nF; i++) {
        int N, D;
        vector <int> tmp;
        fscanf(pFile, "%d", &N);
        //printf("%d\n",N);
        for (int j=0; j<N; j++) {
            fscanf(pFile, "%d", &D);
            tmp.push_back(D);
        }
        vector <Line> tmplines;
        tmplines.push_back(makeLine( vertices[tmp[0]].x, vertices[tmp[0]].y, vertices[tmp[0]].z,
                                        vertices[tmp[1]].x, vertices[tmp[1]].y , vertices[tmp[1]].z ));
        tmplines.push_back(makeLine( vertices[tmp[0]].x, vertices[tmp[0]].y, vertices[tmp[0]].z,
                                        vertices[tmp[2]].x, vertices[tmp[2]].y , vertices[tmp[2]].z ));
        tmplines.push_back(makeLine( vertices[tmp[2]].x, vertices[tmp[2]].y, vertices[tmp[2]].z,
                                        vertices[tmp[1]].x, vertices[tmp[1]].y , vertices[tmp[1]].z ));
        faces.push_back(makeFace(N, tmplines));
    }
    if (DEBUG) printf("DONE\n");

    for (int i=0; i<nF; i++) {
        for (int j=0; j<faces[i].num; j++) {
            if (DEBUG) printf("%d %d\n", faces[i].lines[j].A.size(), faces[i].lines[j].B.size());
            //assert(faces[i].lines[j].A.size() != 3 || faces[i].lines[j].B.size() != 3 );
            lines.push_back(faces[i].lines[j]);
        }

    }
    printf("DONE\n");
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-WINDOW_WIDTH)/2,
                       (glutGet(GLUT_SCREEN_HEIGHT)-WINDOW_HEIGHT)/2);
	gluOrtho2D(-500,500,-500,500);
	glutInitWindowSize(WINDOW_HEIGHT, WINDOW_WIDTH);
	glutCreateWindow("New Window");
	glutDisplayFunc(draw);
	glutKeyboardFunc(move);
    glutMainLoop();
	return 0;
}
