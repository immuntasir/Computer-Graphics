/* 
This code implements the Midpoint algorithm for line drawing. 
To help with visualization, it uses different colors for different zones of slopes. 

The code has two options. 
You may choose to draw n random lines, either from origin, or from anywhere.
freeglut was used in this code.

You must have freeglut and freeglut-dev installed. In Linux, run the following commands:
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold

To run the code, enter the following commands
1. gcc Assignment_004_007_Muntasir-Wahed.c -o t -lGL -lGLU -lglut 
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

int ComputeOutCode(double x, double y) {
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
void drawPixel(double X, double Y, int zone) {
    double Xp, Yp;
    if (zone == 0)  {Xp = X; Yp = Y;}
    else if (zone == 1) {Xp = Y; Yp = X;}    
    else if (zone == 2) {Xp = -Y; Yp = X;}
    else if (zone == 3) {Xp = -X; Yp = Y;}
    else if (zone == 4) {Xp = -X; Yp = -Y;}
    else if (zone == 5) {Xp = -Y; Yp = -X;}
    else if (zone == 6) {Xp = Y; Yp = -X;}
    else if (zone == 7) {Xp = X; Yp = -Y;}
    
	glVertex2d((Xp*2) / WINDOW_WIDTH, (Yp*2)/WINDOW_HEIGHT );
}
void drawLineZone0 (double X0, double Y0, double X1, double Y1, int zone) {
	int i;
	double dx=(X1-X0);
	double dy=(Y1-Y0);
	double d = 2*dy - dx;
	double dE = 2*dy;
	double dNE = 2*dy-dx;	
	double X = X0, Y = Y0;
	drawPixel(X, Y, zone);
	while (X < X1) {
		if (d < 0) {
			X = X + 1;
			d += dE;
		}
		else {
			X = X + 1;
			Y = Y + 1;
			d += dNE;
		}
		drawPixel(X, Y, zone);
	}
}

void drawLine(double X0, double Y0, double X1, double Y1) {
	glBegin(GL_POINTS);
	
    int zone = findZone(X0, Y0, X1, Y1);
    if (zone == 0) drawLineZone0(X0, Y0, X1, Y1, zone);
	else if (zone == 1) drawLineZone0(Y0, X0, Y1, X1, zone);
	else if (zone == 2) drawLineZone0(Y0, -X0, Y1, -X1, zone);	
	else if (zone == 3) drawLineZone0(-X0, Y0, -X1, Y1, zone);
	else if (zone == 4) drawLineZone0(-X0, -Y0, -X1, -Y1, zone);
	else if (zone == 5) drawLineZone0(-Y0, -X0, -Y1, -X1, zone);	
	else if (zone == 6) drawLineZone0(-Y0, X0, -Y1, X1, zone);
	else if (zone == 7) drawLineZone0(X0, -Y0, X1, -Y1, zone);	
	
	glEnd( );
}
int choice, c2;
int n;
void DrawLineAfterClipping(double X0, double Y0, double X1, double Y1) {
	int outcode0 = ComputeOutCode(X0, Y0);
	int outcode1 = ComputeOutCode(X1, Y1);
	int accept = 0;
	while (1) {
		if (!(outcode0 | outcode1)) {
			accept = 1;
			break;
		} 
        else if (outcode0 & outcode1) {
            glColor3f(0.0, 0.0, 1.0);                
            drawLine(X0, Y0, X1, Y1);				
            break;
		} 
        else {
			double X, Y;
			int oc = outcode0 ? outcode0 : outcode1;
            if (oc & LEFT) {  
				Y = Y0 + (Y1 - Y0) * (XMIN - X0) / (X1 - X0);
				X = XMIN;
			} 
            else if (oc & RIGHT) { 
				Y = Y0 + (Y1 - Y0) * (XMAX - X0) / (X1 - X0);
				X = XMAX;
			}            
            else if (oc & BOTTOM) { 
				X = X0 + (X1 - X0) * (YMIN - Y0) / (Y1 - Y0);
				Y = YMIN;
			} 
            else if (oc & TOP) { 
				X = X0 + (X1 - X0) * (YMAX - Y0) / (Y1 - Y0);
				Y = YMAX;
			} 
            if (oc == outcode0) {
                glColor3f(1.0, 0.0, 0.0);                
                drawLine(X0, Y0, X1, Y1);				
                X0 = X;
				Y0 = Y;
				outcode0 = ComputeOutCode(X0, Y0);
			} 
            else {
                glColor3f(1.0, 0.0, 0.0);                
                drawLine(X0, Y0, X1, Y1);
				X1 = X;
				Y1 = Y;
				outcode1 = ComputeOutCode(X1, Y1);
			}
		}
	}
	if (accept) {
        glColor3f(0.0, 1.0, 0.0);
		drawLine(X0, Y0, X1, Y1);
	}
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
    drawLine(XMIN, YMIN, XMAX, YMIN);
    drawLine(XMIN, YMIN, XMIN, YMAX);
    drawLine(XMAX, YMAX, XMAX, YMIN);
    drawLine(XMAX, YMAX, XMIN, YMAX);
    srand((unsigned) time(&t));
    for (i=0; i<n; i++) {
        if (c2 == 1) X0 = 0;
        else X0 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;
        if (c2 == 1) Y0 = 0;
        else Y0 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;    
        X1 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;
        Y1 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;
        DrawLineAfterClipping(X0, Y0, X1, Y1);
    }
    
    glFlush();
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    printf("Enter the number of lines: ");
    scanf("%d", &n);
    // printf("1. From origin.\n");
    // printf("2. From anywhere to anywhere");
    // scanf("%d", &c2);    
    c2 = 1;
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
