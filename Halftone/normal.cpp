#include <GL/glut.h>
#include <bits/stdc++.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#define MUL .33
using namespace std;

void ReadBMP(char* filename) {
	int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    glClear (GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

    for(int i = 0; i < height; i++) {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3) {
        	double r,g,b;
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;
            r = data[j]/255.0;
          	g = data[j+1]/255.0;
          	b = data[j+2]/255.0;
            glColor3f (r,g,b);
    		glVertex2i((int)j/3,(int)i);
        }
    }
    fclose(f);
    glEnd();
    glFlush();
}

void drawNormal(void) {
    ReadBMP("Lion.bmp");
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640,480);
    glutCreateWindow("Normal");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glutDisplayFunc(drawNormal);
    glutMainLoop();
    return 0;
}
