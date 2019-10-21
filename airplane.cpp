/*
This code implements roatation along an specified axis.

The code has two options.
Youpo may choose to draw n random lines, either from origin, or from anywhere.
freeglut was used in this code.

You must have freeglut and freeglut-dev installed. In Linux, run the following commands:
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold

To run the code, enter the following commands
1. g++ cow.cpp -o t -lGL -lGLU -lglut
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
#define point_offset 1
#define INSIDE 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
#define DEGREE  (3.14159/180)
#define DEPTH 2500
double offsetx = 0;
double offsety = 0;
double offsetz = 7000;



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
int cnt = 11;
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

struct rectangle makeRectangle (int _xmin, int _ymin, int _xmax, int _ymax) {
    struct rectangle rect;
    rect.xmin = _xmin;
    rect.ymin = _ymin;
    rect.xmax = _xmax;
    rect.ymax = _ymax;
    return rect;
}
void draw_axis() {
	glBegin(GL_POINTS);
    glColor3f(0.0,1.0,0.0);
	drawLine(-500, 0, 500, 0);
	drawLine(0, -500, 0, 500);
    glEnd( );
	glFlush();
}
void drawRectangle(struct rectangle Rect) {
    drawLine(Rect.xmin, Rect.ymin, Rect.xmax, Rect.ymin);
    drawLine(Rect.xmin, Rect.ymin, Rect.xmin, Rect.ymax);
    drawLine(Rect.xmax, Rect.ymax, Rect.xmax, Rect.ymin);
    drawLine(Rect.xmax, Rect.ymax, Rect.xmin, Rect.ymax);
}
void draw3DLinesIn2D (vector <Line> newlines, double depth) {
    for (int i=0; i<newlines.size(); i++) {
        drawLine((newlines[i].A[0] + offsetx)/(1 + ((newlines[i].A[2]+offsetz)/depth)),
                            (newlines[i].A[1]+offsety)/(1 + ((newlines[i].A[2]+offsetz)/depth)),
                 (newlines[i].B[0]+offsetx)/(1 + ((newlines[i].B[2]+offsetz)/depth)),
                            (newlines[i].B[1]+offsety)/(1 + ((newlines[i].B[2]+offsetz)/depth)));
    }
}
void drawFrame(vector <Line> newlines) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    draw_axis();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    draw3DLinesIn2D(newlines, DEPTH);
    glEnd( );
	glFlush();
}
void draw() {
 	glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glPointSize(2);
    double X0, X1, Y0, Y1;
	int i;
    time_t t;

    drawFrame(lines);

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
    Line tmp;
    for (int i=0; i<lines.size(); i++) {
        tmp.A = matMultiply(makeRotateX(degree), lines[i].A);
        tmp.B = matMultiply(makeRotateX(degree), lines[i].B);
        newlines.push_back(tmp);
    }
    lines = newlines;
   // drawFrame(lines);
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
    //drawFrame(lines);
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
    //drawFrame(lines);
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
    drawFrame(lines);
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
int main(int argc, char **argv) {
    FILE * pFile;
    pFile = fopen ("airplane.ply","r");
    double d;
    removeHeader(pFile);
    //fscanf(pFile, "%lf",&d);
    printf("Number of vertices: %d\n", nV);
    printf("Number of face: %d\n", nF);
    //printf("the point %lf\n", d);
    double a, b, c;
    for (int i=0; i<nV; i++) {
        fscanf(pFile, "%lf %lf %lf", &a, &b, &c);
        if (i<10) printf("%lf %lf %lf\n", a, b, c);
        vertices.push_back(makeVertex(a*point_offset, b*point_offset, c*point_offset));
    }
    for (int i=0; i<nF; i++) {
        int N, D;
        vector <int> tmp;
        fscanf(pFile, "%d", &N);
        //printf("%d\n",N);
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
