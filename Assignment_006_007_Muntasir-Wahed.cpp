/*
This code implements roatation along an specified axis.

The code has two options.
Youpo may choose to draw n random lines, either from origin, or from anywhere.
freeglut was used in this code.

You must have freeglut and freeglut-dev installed. In Linux, run the following commands:
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold

To run the code, enter the following commands
1. g++ Assignment_006_007_Muntasir-Wahed.cpp -o t -lGL -lGLU -lglut
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
double degree = (3.14159/180);
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
    double X0, Y0, X1, Y1;
} Line;
Line makeLine (double _X0, double _Y0, double _X1, double _Y1) {
    Line ret;
    ret.X0 = _X0;
    ret.X1 = _X1;
    ret.Y0 = _Y0;
    ret.Y1 = _Y1;
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
        drawLine(newlines[i].X0, newlines[i].Y0, newlines[i].X1, newlines[i].Y1);
    }
}
void drawFrame(vector <Line> newlines) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    draw_axis();
    glColor3f(0.0, 0.0, 0.0);
    drawLines(newlines);
}
void rotationx() {
    vector <Line> newlines;
    for (int i=0; i<lines.size(); i++) {
        Line temp = makeLine(lines[i].X0 * cos(degree), lines[i].Y0, lines[i].X1 * cos(degree), lines[i].Y1);
        newlines.push_back(temp);
    }
    drawFrame(newlines);
}

void rotationy() {
    vector <Line> newlines;
    for (int i=0; i<lines.size(); i++) {
        Line temp = makeLine(lines[i].X0, lines[i].Y0 * cos(degree), lines[i].X1, lines[i].Y1 * cos(degree));
        newlines.push_back(temp);
    }
    drawFrame(newlines);
}

void rotationz() {
    vector <Line> newlines;
    for (int i=0; i<lines.size(); i++) {
        Line temp = makeLine(lines[i].X0 * cos(degree) - lines[i].Y0 * sin(degree),
                              lines[i].X0 * sin(degree) - lines[i].Y0 * cos(degree),
                                lines[i].X1 * cos(degree) - lines[i].Y1 * sin(degree),
                                    lines[i].X1 * sin(degree) - lines[i].Y1 * cos(degree) );
        newlines.push_back(temp);
    }
    drawFrame(newlines);
}
void rotate_left()
{
	rotationx();
	degree = degree + (3.1416/180);
}

void rotate_right()
{
	rotationx();
	degree = degree -(3.1416/180);
}

void rotate_up()
{
	rotationy();
	degree = degree + (3.1416/180);
}

void rotate_down()
{
	rotationy();
	degree = degree - (3.1416/180);
}
void rotate_z_cl()
{
	rotationz();
	degree = degree + (3.1416/180);
}

void rotate_z_acl()
{
	rotationz();
	degree = degree - (3.1416/180);
}
void reset() {
    degree = (3.1416/180);
    drawFrame(lines);
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
        case 120: reset(); break;
    }
}
int main(int argc, char **argv) {
    lines.push_back(makeLine(-100, -100, 100, -100));
    lines.push_back(makeLine(100, -100, 100, 100));
    lines.push_back(makeLine(100, 100, -100, 100));
    lines.push_back(makeLine(-100, 100, -100, -100));
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
