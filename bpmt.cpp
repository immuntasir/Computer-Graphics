#include <GL/glut.h> // Include the GLUT header file
#include <bits/stdc++.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#define MUL .33
using namespace std;
double ara[1000][1000];
int width, height;
double getHalfTone (int row, int col) {
    double sum;
    sum += ara[row+1][col];
    sum += ara[row-1][col];
    sum += ara[row][col+1];
    sum += ara[row][col-1];
    sum += ara[row+1][col+1];
    sum += ara[row+1][col-1];
    sum += ara[row-1][col+1];
    sum += ara[row-1][col-1];
    sum += ara[row][col];
    sum /= 10;

    if (sum<25) {
        return 0;
    }
    else if (sum<=50) {
        return 1;
    }
    else if (sum<=76) {
        return 2;
    }
    else if (sum<=102) {
        return 3;
    }
    else if (sum<=128) {
        return 4;
    }
    else if (sum<=154) {
        return 5;
    }
    else if (sum<=180) {
        return 6;
    }
    else if (sum<=206) {
        return 7;
    }
    else if (sum<=232) {
        return 8;
    }
    else  {
        return 9;
    }
}
double mat [10][3][3] =
    {   //0
        {
            {0,0,0},
            {0,0,0},
            {0,0,0}
        },
        //1
        {
            {0,255,0},
            {0,0,0},
            {0,0,0}
        },
        //2
        {
            {0,255,0},
            {0,0,0},
            {0,0,255}
        },
        //3
        {
            {255,255,0},
            {0,0,0},
            {0,0,255}
        },
        //4
        {
            {255,255,0},
            {0,0,0},
            {255,0,255}
        },
        //5
        {
            {255,255,255},
            {0,0,0},
            {255,0,255}
        },
        //6
        {
            {255,255,255},
            {0,0,255},
            {255,0,255}
        },
        //7
        {
            {255,255,255},
            {0,0,255},
            {255,255,255}
        },
        //8
        {
            {255,255,255},
            {255,0,255},
            {255,255,255}
        },
        //9
        {
            {255,255,255},
            {255,255,255},
            {255,255,255}
        },

    };
void replaceMatrix (int row, int col, int halftone) {
    ara[row][col] = mat[halftone][1][1]/255;
    ara[row+1][col] = mat[halftone][1+1][1]/255;
    ara[row-1][col] = mat[halftone][1-1][1]/255;
    ara[row][col+1] = mat[halftone][1][1+1]/255;
    ara[row][col-1] = mat[halftone][1][1-1]/255;
    ara[row+1][col+1] = mat[halftone][1+1][1+1]/255;
    ara[row+1][col-1] = mat[halftone][1+1][1-1]/255;
    ara[row-1][col+1] = mat[halftone][1-1][1+1]/255;
    ara[row-1][col-1] = mat[halftone][1-1][1-1]/255;
}
unsigned char* data;
unsigned char* ReadBMP(char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    width = *(int*)&info[18];
    height = *(int*)&info[22];

    cout << endl;
    cout << "  Name: " << filename << endl;
    cout << " Width: " << width << endl;
    cout << "Height: " << height << endl;

    int row_padded = (width*3 + 3) & (~3);
    data = new unsigned char[row_padded];
    unsigned char tmp;

    for(int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;

           // cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
        }
    }

    fclose(f);
    return data;
}
void drawNormal() {
	glClear (GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);


    int i;



    int row = 1;
    int col = 1;
    int totrow, totcol;
    for(int i = 0; i < height; i++) {
        col = 1;
        for(int j = 0; j < width*3; j += 3) {
        	double r,g,b;
            double fl = (r+g+b)/3;
            ara[col++][row] = fl;
        }
        row++;
        totrow = col;
        totcol = row;
    }
    double newval;
    for (row = 1; row <= totrow; row+=3) {
        for (col = 1; col <= totcol; col+=3) {
            //cout << "hafltone: " << newval << endl;
            newval = getHalfTone(row, col);
            replaceMatrix(row, col, newval);

        }
    }

    for (row = 1; row <= totrow; row++) {
        for (col = 1; col <= totcol; col++) {
            glColor3f (ara[row][col],ara[row][col],ara[row][col]);
    		glVertex2i(row, col);

        }
    }

    glEnd();
    glFlush();
}



/*
	Now data should contain the (R, G, B) values of the pixels.
	The color of pixel (i, j) is stored at data[3 * (i * width + j)], data[3 * (i * width + j) + 1] and data[3 * (i * width + j) + 2]
*/

int main(int argc, char** argv) {
    ReadBMP("Lion.bmp");
    glutInit(&argc, argv);

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640,480);
    glutCreateWindow("Normal");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glutDisplayFunc(drawNormal);

    glutInitWindowPosition(640, 0);
    glutInitWindowSize(640,480);
    glutCreateWindow("Normal");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glutDisplayFunc(drawNormal);

    glutMainLoop();

    return 0;
}
