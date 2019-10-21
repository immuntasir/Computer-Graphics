#include <GL/glut.h> // Include the GLUT header file
#include <bits/stdc++.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#define MUL .33
using namespace std;

void ReadBMP(char* filename)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);


    int i;
    FILE* f = fopen(filename, "rb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    cout << "Name: " << filename << endl;
    cout << "Width: " << width << endl;
    cout << "Height: " << height << endl;

    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    for(int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)

        	double r,g,b;

            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;

            r = data[j]/255.0;
        	g = data[j+1]/255.0;
        	b = data[j+2]/255.0;

            double fl = (r+g+b)/3;
            glColor3f (fl,fl,fl);
    		glVertex2i((int)j/3,(int)i);

            //cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
        }
    }

    fclose(f);
    glEnd();
    glFlush();
}



/*
	Now data should contain the (R, G, B) values of the pixels.
	The color of pixel (i, j) is stored at data[3 * (i * width + j)], data[3 * (i * width + j) + 1] and data[3 * (i * width + j) + 2]
*/

void cow(void)
{
    ReadBMP("Lion.bmp");
}




int main(int argc, char** argv) {

    //freopen("cow.ply","r",stdin);
//    freopen("out.txt","w",stdout);





    glutInit(&argc, argv);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-700)/2,
                       (glutGet(GLUT_SCREEN_HEIGHT)-700)/2);
	glutInitWindowSize(700,700);
    glutCreateWindow("Tiger");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 699, 0, 699);
    //glOrtho(-1000, 1000.0, -1000, 1000.0, -4000, 4000.0);

    glutDisplayFunc(cow);

    glutMainLoop();

    return 0;
}
