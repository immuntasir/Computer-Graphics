/* 
This code implements the Midpoint algorithm for line drawing. 
To help with visualization, it uses different colors for different zones of slopes. 

The code has two options. 
You may either specify two specific points.
Or you may choose to draw n random lines, either from origin, or from anywhere.
freeglut was used in this code.

You must have freeglut and freeglut-dev installed. In Linux, run the following commands:
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold

To run the code, enter the following commands
1. gcc 8-point-symmetry.c -o t -lGL -lGLU -lglut 
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
double pX0[100010];
double pY0[100010];
double pX1[100010];
double pY1[100010];
double X0, Y0, X1, Y1;


int findZone() {
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
void swap (double *a, double *b) {
    double temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
void drawPixel(int zone, double X, double Y) {
    double temp;	
    if (zone == 0) {
         
    }	
    else if (zone == 1) {
        swap(&X, &Y);
    }
    else if (zone == 2) {
        X = -X;
        swap(&X, &Y);
    }
    else if (zone == 3) {
        X = -X;
    }
    else if (zone == 4) {
        X = -X;
        Y = -Y;
    }
    else if (zone == 5) {
        Y = -Y;
        X = -X;        
        swap(&X, &Y);    
    }
    else if (zone == 6) {
        Y = -Y;
        swap(&X, &Y);
    }
    else if (zone == 7) {
        Y = -Y;
    }    

    glVertex2d((X*2) / WINDOW_WIDTH, (Y*2)/WINDOW_HEIGHT );
}
void drawLineZone(int zone) {
	int i;
    if (zone == 0) {
        
    }
    else if (zone == 1) {        
        swap(&X1, &Y1);
        swap(&X0, &Y0);
    }
    else if (zone == 2) {
        Y0 = -Y0;
        Y1 = -Y1;
        swap(&X1, &Y1);
        swap(&X0, &Y0);
    }
    else if (zone == 3) {
        X0 = -X0;
        X1 = -X1;        
    }
    else if (zone == 4) {
        X0 = -X0;
        X1 = -X1;
        Y0 = -Y0;
        Y1 = -Y1;
    }
    else if (zone == 5) {
        Y0 = -Y0;
        Y1 = -Y1;
        X0 = -X0;
        X1 = -X1;
        swap(&X1, &Y1);
        swap(&X0, &Y0);
    }
    else if (zone == 6) {
        X1 = -X1;
        X0 = -X0;
        swap(&X1, &Y1);
        swap(&X0, &Y0);
    }
    else if (zone == 7) {
        Y1 = -Y1;
        Y0 = -Y0;
    }
    double dx=(X1-X0);
	double dy=(Y1-Y0);
	double d = 2*dy - dx;
	double dE = 2*dy;
	double dNE = 2*dy-dx;	
	double X = X0, Y = Y0;
	drawPixel(zone, X, Y);
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
		drawPixel(zone, X, Y);
	}
}


void drawLine() {
	double dx=(X1-X0);
	double dy=(Y1-Y0);
	double steps;
    double xD,yD,x=X0,y=Y0;
	if (fabs(dx) > fabs(dy)) steps = abs(dx);
	else steps = abs(dy);
	xD=dx/(float)steps;
	yD=dy/(float)steps;	
	
	glBegin(GL_POINTS);
	int zone = findZone();
	// printf("%d\n", zone);
	if (zone == 0) glColor3f(1.0, 0.0, 0.0);
	else if (zone == 1) glColor3f(0.0, 1.0, 0.0);
	else if (zone == 2) glColor3f(0.0, 0.0, 1.0);
	else if (zone == 3) glColor3f(1.0, 1.0, 0.0);
	else if (zone == 4) glColor3f(0.0, 1.0, 1.0);
	else if (zone == 5) glColor3f(1.0, 0.0, 1.0);
	else if (zone == 6) glColor3f(0.5, 0.5, 1.0);
	else if (zone == 7) glColor3f(0.0, 0.0, 0.0);
	drawLineZone(zone);
	glEnd( );
}
int choice, c2;
int n;
void draw() {
 	glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
 
  	glPointSize(2);  

	if (choice == 1) drawLine();
    else {
        int i;
        time_t t;
        srand((unsigned) time(&t));
        for (i=0; i<n; i++) {
            if (c2 == 1) X0 = 0;
            else X0 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;
            if (c2 == 1) Y0 = 0;
            else Y0 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;    
            X1 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;
            Y1 = (rand() % (2*MAXLENGTH)) - MAXLENGTH;
            drawLine();
        }
    }
	glFlush();
        glutSwapBuffers();
}

int main(int argc, char **argv) {
    printf("1. Draw a line with specific points\n");
    printf("2. Draw n random lines.\n");
    
    scanf("%d", &choice);	
	printf("%d\n", choice);
    if (choice == 1) {
        printf("Enter the first point (x0, y0): ");	
	    scanf("%lf %lf", &X0, &Y0);	
	
	    printf("Enter the second point (x1, y1): ");	
	    scanf("%lf %lf", &X1, &Y1);
    }
    	
    else {
        printf("Enter the number of lines: ");
        scanf("%d", &n);
        printf("1. From origin.\n");
        printf("2. From anywhere to anywhere");
        scanf("%d", &c2);    
    }	
	
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
