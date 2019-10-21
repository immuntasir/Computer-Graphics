/*
This code implements roatation along an specified axis.

The code has two options.
Youpo may choose to draw n random lines, either from origin, or from anywhere.
freeglut was used in this code.

You must have freeglut and freeglut-dev installed. In Linux, run the following commands:
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold

To run the code, enter the following commands
1. g++ bpm.cpp -o t -lGL -lGLU -lglut
2. ./t
*/

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define INF 1000000000
#define MAX 7000
#define DEBUG 0
#include <bits/stdc++.h>
using namespace std;

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/freeglut.h>
#include "GL/gl.h"

unsigned char * data;
int height, width;
void drawPixel(double X, double Y) {
	 glVertex2d((x*2) / WINDOW_WIDTH, (y*2)/WINDOW_HEIGHT );
}

void draw() {
 	glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
  	glPointSize(2);

    glBegin(GL_POINTS);
  	/*
  	for(int i = 0; i < height; i++)
    {
        int cur = 0;
        for(int j = 0; j < width*3; j += 3)
        {
            glColor3f(((double)(int)data[j])/255.0, ((double)(int)data[j+1])/255.0, ((double)(int)data[j+2])/255.0);
            drawPixel(i, cur++);
        }
    }
    */
    glColor3f(0.0, 0.0, 1.0);
    for (int i=0; i<1000000; i++) {
        drawPixel(0, (double)i);
        cout << 0 << ", " << i << endl;
    }


    glEnd();
	glFlush();
    glutSwapBuffers();
}
int main(int argc, char **argv) {
    ReadBMP("Lion.bmp");
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
