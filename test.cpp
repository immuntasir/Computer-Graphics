/*
To run the code, enter the following commands
1. g++ Assignment_008_007_Muntasir-Wahed.cpp -o t -lGL -lGLU -lglut
2. ./t
*/

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
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

#define INSIDE 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
#define DEGREE  (3.14159/180)
#define DEPTH 1500
double offsetx = 0;
double offsety = 0;
double offsetz = 4000;


double min (double a, double b) {
    if (a < b) return a;
    else return b;
}
double max (double a, double b) {
    if (a > b) return a;
    else return b;
}
int findOutcode(double x, double y) {
	int code = INSIDE;
	if (x < XMIN) code |= LEFT;
	else if (x > XMAX) code |= RIGHT;
	if (y < YMIN) code |= BOTTOM;
	else if (y > YMAX) code |= TOP;
	return code;
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
    vector <int> lines;
} face;
face makeFace (int num, vector <int> lines) {
    face ret;
    ret.num = num; ret.lines = lines;
    return ret;
}
vector <vertex> vertices;
vector <face> faces;
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
			drawLine(x_st, y_st, x_en, y_en);
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
void drawFrame(vector <Line> newlines, vector <Face> faces) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    draw_axis();
    glColor3f(0.0, 0.0, 0.0);
    draw3DLinesIn2D(newlines, DEPTH);
    for (int i=0; i<faces.size)

}
void draw() {
 	glClearColor(1.0,1.0,1.0,1.0);
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
    vector <Face> newfaces;
    Line tmp;
    for (int i=0; i<lines.size(); i++) {
        tmp.A = matMultiply(makeRotateX(degree), lines[i].A);
        tmp.B = matMultiply(makeRotateX(degree), lines[i].B);
        newlines.push_back(tmp);
    }

    faces = makeFace(newlines.size(), newlines);
    lines = newlines;
    drawFrame(lines, faces);
}

void rotationy(double degree) {
    vector <Line> newlines;
    Line tmp;
    for (int i=0; i<lines.size(); i++) {
        tmp.A = matMultiply(makeRotateY(degree), lines[i].A);
        tmp.B = matMultiply(makeRotateY(degree), lines[i].B);
        newlines.push_back(tmp);
    }
    faces = makeFace(newlines.size(), newlines);
    lines = newlines;
    drawFrame(lines, faces);
}

void rotationz(double degree) {
    vector <Line> newlines;
    Line tmp;
    for (int i=0; i<lines.size(); i++) {
        tmp.A = matMultiply(makeRotateZ(degree), lines[i].A);
        tmp.B = matMultiply(makeRotateZ(degree), lines[i].B);
        newlines.push_back(tmp);
    }
    faces = makeFace(newlines.size(), newlines);
    lines = newlines;
    drawFrame(lines, faces);
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
    fscanf(pFile, "%lf",&d);
    printf("Number of vertices: %d\n", nV);
    printf("Number of face: %d\n", nF);
    printf("the point %lf\n", d);
    double a, b, c;
    for (int i=0; i<nV; i++) {
        fscanf(pFile, "%lf %lf %lf", &a, &b, &c);
        vertices.push_back(makeVertex(a, b, c));
    }
    for (int i=0; i<nF; i++) {
        int N, D;
        vector <int> tmp;
        fscanf(pFile, "%d", &N);
        printf("%d\n",N);
        for (int j=0; j<N; j++) {
            fscanf(pFile, "%d", &D);
            tmp.push_back(D);
        }
        faces.push_back(makeFace(N, tmp));
    }
    printf("DONE\n");
    for (int i=0; i<nF; i++) {
        //printf("Face %d: %d %d\n", i, faces[i].num, faces[i].lines.size());
        for (int j=0; j<faces[i].num - 1; j++) {
            for (int k=j+1; k<faces[i].num; k++) {
                lines.push_back(makeLine(vertices[faces[i].lines[j]].x, vertices[faces[i].lines[j]].y, vertices[faces[i].lines[j]].z,
                                          vertices[faces[i].lines[k]].x, vertices[faces[i].lines[k]].y, vertices[faces[i].lines[k]].z));
            }
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
