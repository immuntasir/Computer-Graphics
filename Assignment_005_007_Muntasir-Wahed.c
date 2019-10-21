/* 
This code implements the Cyrus Beck algorithm for line clipping. 

The code has two options. 
Youpo may choose to draw n random lines, either from origin, or from anywhere.
freeglut was used in this code.

You must have freeglut and freeglut-dev installed. In Linux, run the following commands:
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold

To run the code, enter the following commands
1. gcc Assignment_005_007_Muntasir-Wahed.c -o t -lGL -lGLU -lglut 
2. ./t
*/

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
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
}
int choice, c2;
int n;
struct rectangle {
    int ymax, ymin, xmax, xmin;
} ;
struct rectangle makeRectangle (int _xmin, int _ymin, int _xmax, int _ymax) {
    struct rectangle rect;    
    rect.xmin = _xmin;
    rect.ymin = _ymin;
    rect.xmax = _xmax;
    rect.ymax = _ymax;
    return rect;
} 
double dotProduct(double X0, double Y0, double X1, double Y1) {
    return X0*X1 + Y0*Y1;
}
void cyrusBeck(double X0, double Y0, double X1, double Y1, struct rectangle rect) {
	double t,t1=0,t2=1;
	double a1,b1,a2,b2;

	t = (rect.ymax - Y0) / (Y1 - Y0);
	if(Y0 > Y1) t1 = max(t1,t);
	else t2 = min(t2,t);
	
    t = (rect.ymin - Y0) / (Y1 - Y0);
	if(Y1 > Y0) t1 = max(t1,t);
	else t2 = min(t2,t);
	
    t = (rect.xmin - X0) / (X1 - X0);
	if (X1 > X0) t1 = max(t1,t);
	else t2 = min(t2,t);
	
    t = (rect.xmax - X0) / (X1 - X0);
	if(X0 > X1) t1 = max(t1,t);
	else t2 = min(t2,t);

    double oldx0, oldx1, oldy0, oldy1;
    oldx0 = X0;
    oldx1 = X1;
    oldy0 = Y0;
    oldy1 = Y1;
	
    if(t2 > t1) {
		a1 = X0; 
        a2 = X1; 
        b1 = Y0; 
        b2 = Y1;

		a2 = a1 + t2 * (a2 - a1);
		b2 = b1 + t2 * (b2 - b1);

		a1 = a1 + t1 * (X1 - a1);
		b1 = b1 + t1 * (Y1 - b1);
        
        X0 = a1;
        X1 = a2;
        Y0 = b1;
        Y1 = b2;
        glColor3f(1.0, 0.0, 0.0);
        drawLine(X0, Y0, X1, Y1);
        glBegin(GL_POINTS);        
        if (X0 != oldx0 && Y0 != oldy0) {
            glColor3f(0.0, 1.0, 0.0);
            drawPixel(X0, Y0);
            drawPixel(0, 0);
        }
        else if (X1 != oldx1 && Y1 != oldy1) {
            glColor3f(0.0, 1.0, 0.0);
            drawPixel(X1, Y1);
        }  	
        glEnd();
        printf("t1 = %.2lf t2 = %.2lf\n", t1, t2);
    } 
}
void drawRectangle(double _XMIN, double _YMIN, double _XMAX, double _YMAX) {
    drawLine(_XMIN, _YMIN, _XMAX, _YMIN);
    drawLine(_XMIN, _YMIN, _XMIN, _YMAX);
    drawLine(_XMAX, _YMAX, _XMAX, _YMIN);
    drawLine(_XMAX, _YMAX, _XMIN, _YMAX);
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
    glColor3f(0.0, 0.0, 0.0);
    //drawRectangle(33, 33, 133, 133);
    //drawRectangle(-133, -133, -33, -33);
    drawRectangle(-133, -133, 133, 133);
    srand((unsigned) time(&t));
    for (i=0; i<n; i++) {
        if (c2 == 1) X0 = 0;
        else X0 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;
        if (c2 == 1) Y0 = 0;
        else Y0 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;    
        X1 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;
        Y1 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;
        //cyrusBeck(X0, Y0, X1, Y1, makeRectangle(33, 33, 133, 133));
        //cyrusBeck(X0, Y0, X1, Y1, makeRectangle(-133, -133, -33, -33));   
        drawLine(X0, Y0, X1, Y1);
        cyrusBeck(X0, Y0, X1, Y1, makeRectangle(-133, -133, 133, 133));
    }
    
    glFlush();
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    printf("Enter the number of lines: ");
    scanf("%d", &n);
    printf("1. From origin.\n");
    printf("2. From anywhere to anywhere");
    scanf("%d", &c2);    
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-640)/2,
                       (glutGet(GLUT_SCREEN_HEIGHT)-480)/2);
	glutInitWindowSize(640, 480);
	glutCreateWindow("New Window");
	glutDisplayFunc(draw);
	glutMainLoop();
	return 0;
}
