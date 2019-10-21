/*
This code implements roatation along an specified axis.

The code has two options.
Youpo may choose to draw n random lines, either from origin, or from anywhere.
freeglut was used in this code.

You must have freeglut and freeglut-dev installed. In Linux, run the following commands:
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold

To run the code, enter the following commands
1. g++ testrpt.cpp -o t -lGL -lGLU -lglut
2. ./t
*/

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
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

void drawPixel(double X, double Y) {
	glVertex2d((X*2) / WINDOW_WIDTH, (Y*2)/WINDOW_HEIGHT );
}
void drawLine(double X0, double Y0, double X1, double Y1) {
	double dx=(X1-X0);
	double dy=(Y1-Y0);
	double steps;
    double xD,yD,x=X0,y=Y0;
	if (fabs(dx) > fabs(dy)) steps = abs(dx);
	else steps = abs(dy);
	xD=dx/(float)steps;
	yD=dy/(float)steps;

	glBegin(GL_POINTS);
	int zone = findZone(X0, Y0, X1, Y1);

    int i;
    for(i=0; i<steps; i++) {
    	x+=xD;
    	y+=yD;
        drawPixel(x, y);
    }
	glEnd( );
	glFlush();
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
void drawFrame(vector <Line> newlines) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    draw_axis();
    glColor3f(0.0, 0.0, 0.0);
    drawLines(newlines);
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

void draw() {
 	glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glPointSize(2);
    double X0, X1, Y0, Y1;
	int i;
    time_t t;

    temp = makeRectangle(-133, -133, 133, 133);
    drawFrame(lines);

    glFlush();
    glutSwapBuffers();
}
void rotate(unsigned char input, int x, int y) {
	printf("%d pressed\n", input);
	switch(input) {
		case 27: break;
		case 97: rotate_left(); break;
        case 119: rotate_up(); break;
        case 100: rotate_right(); break;
		case 115: rotate_down(); break;
        case 113: rotate_z_acl(); break;
        case 101: rotate_z_cl(); break;
       // case 120: reset(); break;
    }
}
int main(int argc, char **argv) {
    lines.push_back(makeLine(-100, -100, 0, 100, -100, 0));
    lines.push_back(makeLine(100, -100, 0, 100, 100, 0));
    lines.push_back(makeLine(100, 100, 0, -100, 100, 0));
    lines.push_back(makeLine(-100, 100, 0, -100, -100, 0));
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-640)/2,
                       (glutGet(GLUT_SCREEN_HEIGHT)-480)/2);
	glutInitWindowSize(640, 480);
	glutCreateWindow("New Window");
	glutDisplayFunc(draw);
	glutKeyboardFunc(rotate);
    glutMainLoop();
	return 0;
}
