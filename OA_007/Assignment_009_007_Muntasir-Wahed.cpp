/*
This code implements roatation along an specified axis.

The code has two options.
Youpo may choose to draw n random lines, either from origin, or from anywhere.
freeglut was used in this code.

You must have freeglut and freeglut-dev installed. In Linux, run the following commands:
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold

To run the code, enter the following commands
1. g++ Assignment_009_007_Muntasir-Wahed.cpp -o t -lGL -lGLU -lglut
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
#define MAXLENGTH 1000
#define YMAX 124
#define XMAX 124
#define YMIN -124
#define XMIN -124
#define point_offset 200
#define INSIDE 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
#define DEGREE  (3.14159/180)
#define DEPTH 2500
#define MAX_FACES 33333
double offsetx = 0;
double offsety = 0;
double offsetz = 1500;
int nV, nF;
/*
  Code for Midpoint Line Drawing Algorithm
*/

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
    if (zone == 0)  {Xp = X; Yp = Y;}
    else if (zone == 1) {Xp = Y; Yp = X;}
    else if (zone == 2) {Xp = -Y; Yp = X;}
    else if (zone == 3) {Xp = -X; Yp = Y;}
    else if (zone == 4) {Xp = -X; Yp = -Y;}
    else if (zone == 5) {Xp = -Y; Yp = -X;}
    else if (zone == 6) {Xp = Y; Yp = -X;}
    else if (zone == 7) {Xp = X; Yp = -Y;}
  	glVertex2d(double(Xp*2) / WINDOW_WIDTH, double(Yp*2)/WINDOW_HEIGHT );
}
void drawLineZone0 (int X1, int Y1, int X2, int Y2, int zone) {
    int dx = X2 - X1;
    int dy = Y2 - Y1;
    int d = dy - (dx/2);
    int X = X1, Y = Y1;
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
    if (zone == 0) drawLineZone0(X0, Y0, X1, Y1, zone);
	  else if (zone == 1) drawLineZone0(Y0, X0, Y1, X1, zone);
	  else if (zone == 2) drawLineZone0(Y0, -X0, Y1, -X1, zone);
	  else if (zone == 3) drawLineZone0(-X0, Y0, -X1, Y1, zone);
	  else if (zone == 4) drawLineZone0(-X0, -Y0, -X1, -Y1, zone);
	  else if (zone == 5) drawLineZone0(-Y0, -X0, -Y1, -X1, zone);
	  else if (zone == 6) drawLineZone0(-Y0, X0, -Y1, X1, zone);
	  else if (zone == 7) drawLineZone0(X0, -Y0, X1, -Y1, zone);
}

/*
  Code for Midpoint Line Drawing Algorithm ends here.
*/


int choice, c2;
int n;

/*
  Some data structures are defined here.
*/


typedef struct vertex {
    double x, y, z;
} Vertex;
typedef struct li {
    Vertex A;
    Vertex B;
} Line;
struct rectangle {
    int ymax, ymin, xmax, xmin;
} temp, t2;
Vertex makeVertex (double X, double Y, double Z) {
    Vertex ret;
    ret.x = X; ret.y = Y; ret.z = Z;
    return ret;
}
Vertex lightSource;

Line makeLine (double _X0, double _Y0, double _Z0, double _X1, double _Y1, double _Z1) {
    Line ret;
    ret.A = makeVertex(_X0, _Y0, _Z0);
    ret.B = makeVertex(_X1, _Y1, _Z1);
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
vector <Vertex> vertices;

typedef struct face {
    int num;
    vector <Vertex> face_vertices;
} Face;

Face makeFace (int num, vector <int> points) {
    Face ret;
    ret.num = num;
    for (int i=0; i<num; i++) {
        ret.face_vertices.push_back(vertices[points[i]]);
    }
    return ret;
}

vector <Face> faces;


typedef struct z_so {
    int index;
    double value;
} z_sorted;
z_sorted makeEntry (int _index, double _value) {
    z_sorted ret;
    ret.index = _index; ret.value = _value;
    return ret;
}
bool cmp (z_sorted a, z_sorted b) {
    return a.value < b.value;
}

/*
    Code for shading begins here.
*/
int indices[MAX_FACES];
void sortByZ () {
    vector <z_sorted> sortedArray;
    for (int i=0; i<nF; i++) {
        sortedArray.push_back(makeEntry(i, faces[i].face_vertices[0].z +
                                        faces[i].face_vertices[1].z +
                                            faces[i].face_vertices[2].z));
    }
    sort(sortedArray.begin(), sortedArray.end(), cmp);
    for (int i=0; i<nF; i++) {
        indices[i] = sortedArray[i].index;
    }
}

double shading (int index) {
    double a, b, c;
    double difx1, difx2, dify1, dify2, difz1, difz2;
    vector <Vertex> cur = faces[index].face_vertices;
    difx1 = cur[1].x - cur[0].x;
    difx2 = cur[2].x - cur[1].x;
    dify1 = cur[1].y - cur[0].y;
    dify2 = cur[2].y - cur[1].y;
    difz1 = cur[1].z - cur[0].z;
    difz2 = cur[2].z - cur[1].z;

    a = dify1*difz2 - dify2*difz1;
    b = -difx1*difz2 + difx2*difz1;
    c = difx1*dify2 - difx2*dify1;

    double div = sqrt(a*a + b*b + c*c);
    a /= div; b /= div; c /= div;

    double xMean, yMean, zMean;
    xMean = (cur[0].x + cur[1].x + cur[2].x)/3;
    yMean = (cur[0].y + cur[1].y + cur[2].y)/3;
    zMean = (cur[0].z + cur[1].z + cur[2].z)/3;

    Vertex tmpLightSource = lightSource;
    tmpLightSource.x -= xMean;
    tmpLightSource.y -= yMean;
    tmpLightSource.z -= zMean;

    div = sqrt(tmpLightSource.x * tmpLightSource.x +
                tmpLightSource.y * tmpLightSource.y +
                    tmpLightSource.z * tmpLightSource.z);

    tmpLightSource.x /= div;
    tmpLightSource.y /= div;
    tmpLightSource.z /= div;

    return (a * tmpLightSource.x + b * tmpLightSource.y + c * tmpLightSource.z);
}



double Y_MIN,Y_MAX;
 //For counting lines
int pointer_edge_table=0,counter=0;
int tot_line;
struct Edge_table{
	double ymin;
	double ymax;
	double x;
	double dx;
	double dy;
	int sign;
	double new_x;
	double new_y;
};
Edge_table edge_table[MAX+1],temp_edge_table[MAX+1];
//function to sort edge table
bool sort_edge_table(const Edge_table &a, const Edge_table &b){
	if(a.ymin<b.ymin) return true;
	else if(a.ymin == b.ymin){
		if(a.x<b.x) return true;
		if(a.x==b.x) return true;
		else return false;
	}
	return false;
}

void make_edge_table(double x1, double y1, double x2, double y2,int number){

	int counter = number;
	if(y1<y2) {
		edge_table[number].ymin = y1;
		edge_table[counter].ymax = y2;
		edge_table[counter].x = x1 ;
		edge_table[counter].dx = abs(x2-x1);
		edge_table[counter].dy = abs(y2 - y1);
		edge_table[counter].new_x = x1;
		edge_table[counter].new_y = y1;
		if(((x2-x1)*(y2-y1))>=0){
				edge_table[counter].sign=1;
		}
		else{
			 edge_table[counter].sign=-1;
		}
	}
	else {
		edge_table[number].ymin = y2;
		edge_table[counter].ymax = y1;
		edge_table[counter].x = x2 ;
		edge_table[counter].dx = abs(x2-x1);
		edge_table[counter].dy = abs(y2 - y1);
		edge_table[counter].new_x = x2;
		edge_table[counter].new_y = y2;
		if(((x2-x1)*(y2-y1))>=0){
				edge_table[counter].sign=1;
		}
		else{
			 edge_table[counter].sign=-1;
		}
	}
}


int return_edge(double ymin){
	for(int i=pointer_edge_table;i<n;i++){
		if(edge_table[i].ymin == ymin){
			counter++;
			temp_edge_table[counter]=edge_table[i];
			pointer_edge_table++;
		}
		else {
			break;
		}
	}
	return counter;
}

void active_list(){
	double y_min,y_max;
	y_min = Y_MIN;
	y_max = Y_MAX;
	pointer_edge_table=0;
	counter = 0;
	for(double i=y_min;i<y_max;i++){
		counter = return_edge(i);
		sort(temp_edge_table+1,temp_edge_table+counter,sort_edge_table);
		double x_st, y_st, x_en, y_en;
		vector <Line> tmplines;
		for(int j=1;j<=counter && (j+1)<=counter; j+= 2) {
			if (DEBUG) printf("\n %lf %lf %lf %lf\n", x_st, y_st, x_en, y_en);
			x_st = temp_edge_table[j].x;
			y_st = temp_edge_table[j].ymin;
			x_en = temp_edge_table[j+1].x;
			y_en = temp_edge_table[j+1].ymin;
            if (DEBUG) printf("x_st = %lf y_st = %lf x_en = %lf y_en = %lf\n",x_st,y_st,x_en,y_en);
			drawLine(x_st, y_st, x_en, y_en);
		}
		int second_counter = 0;
		for(int j=1;j<=counter;j++){
			temp_edge_table[j].ymin++;
			if(temp_edge_table[j].ymin != temp_edge_table[j].ymax){
				second_counter++;
				temp_edge_table[second_counter].ymin=temp_edge_table[j].ymin;
				temp_edge_table[second_counter].ymax=temp_edge_table[j].ymax;
				temp_edge_table[second_counter].dx=temp_edge_table[j].dx;
				temp_edge_table[second_counter].dy=temp_edge_table[j].dy;
				temp_edge_table[second_counter].sign=temp_edge_table[j].sign;
				temp_edge_table[second_counter].new_x=temp_edge_table[j].new_x;
				temp_edge_table[second_counter].new_y=temp_edge_table[j].new_y;
				if(temp_edge_table[second_counter].dy != 0){ //x will change
						double lob = (double)temp_edge_table[j].dx*1.0;
						double hor = (double)temp_edge_table[j].dy*1.0;
						double frac = (double)temp_edge_table[j].new_x+1.0*(temp_edge_table[j].sign)*(lob/hor)*(temp_edge_table[j].ymin-temp_edge_table[j].new_y);
						temp_edge_table[second_counter].x = frac;
				}
				else{
					temp_edge_table[second_counter].x=temp_edge_table[j].x;
				}
			}
		}
		counter = second_counter;
	}
}
void polygon_filling(Face temp){
	Y_MIN = INF;
	Y_MAX = -INF;
	int counter = 0;

    for (int i=0; i<temp.num - 1; i++) {
        for (int j=i+1; j<temp.num; j++) {
            Y_MIN = min(Y_MIN,round(temp.face_vertices[i].y));
            Y_MAX = max(Y_MAX,round(temp.face_vertices[i].y));
            Y_MAX = max(Y_MAX,round(temp.face_vertices[j].y));
            Y_MIN = min(Y_MIN,round(temp.face_vertices[j].y));
            if(temp.face_vertices[i].y != temp.face_vertices[j].y) {
                //printf("%lf %lf %lf %lf\n", round(temp.lines[i].A[0]),round(temp.lines[i].A[1]),round(temp.lines[i].B[0]),round(temp.lines[i].B[1]));
                make_edge_table(round(temp.face_vertices[i].x),round(temp.face_vertices[i].y),
                       round(temp.face_vertices[j].x),round(temp.face_vertices[j].y), counter);
                counter++;
            }
        }

    }

	n=counter;
	if (DEBUG) printf("%d\n", n);
	sort(edge_table,edge_table+n,sort_edge_table);
	//for (int i=0; i<n; i++) printf("%lf %lf\n", edge_table[i].ymin, edge_table[i].ymax);
	active_list();
}
Vertex projectVertex (Vertex current, double depth) {
    Vertex ret;
    ret.x = (current.x + offsetx)/(1 + ((current.z+offsetz)/depth));
    ret.y = (current.y + offsety)/(1 + ((current.z+offsetz)/depth));
    return ret;
}

void draw3DFacesIn2D (vector <Face> newfaces, double depth) {
    for (int i=0; i<newfaces.size(); i++) {
        for (int j=0; j<newfaces[i].face_vertices.size(); j++) {
            newfaces[i].face_vertices[j] = projectVertex(newfaces[i].face_vertices[j], depth);
        }
    }

    sortByZ();

    for (int i=0; i<newfaces.size(); i++) {
        int idx = indices[i];
        double k = shading(idx);
        glColor3f(k, k, k);
        glBegin(GL_POINTS);
        for (int j=0; j<newfaces[idx].face_vertices.size(); j++) {
            polygon_filling(newfaces[idx]);
        }
        glEnd();
        glBegin(GL_POLYGON);
        Vertex cur;
        for (int j=0; j<newfaces[idx].face_vertices.size(); j++) {
            cur = newfaces[idx].face_vertices[j];
            glVertex2d((cur.x * 2)/WINDOW_WIDTH, (cur.y*2)/WINDOW_HEIGHT);
        }
        glEnd();
    }
}
/*
  Initial window is drawn here.
*/
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


void drawFrame(vector <Face> newfaces) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    draw_axis();
    draw3DFacesIn2D(faces, DEPTH);
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

    drawFrame(faces);

    glFlush();
    glutSwapBuffers();
}
/*
    Rotation is implemented in the following code section.
*/
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
Vertex matMultiply (vector <vector < double> > A, Vertex point) {
    Vertex  tmp;
    vector <double> ret;
    ret.push_back(0);
    ret.push_back(0);
    ret.push_back(0);
    vector <double> x;
    x.push_back(point.x);
    x.push_back(point.y);
    x.push_back(point.z);
    for (int i=0; i<A.size(); i++) {
        for (int j=0; j<1; j++) {
            for (int k=0; k<A[0].size(); k++) {
                ret[i] = ret[i] + A[i][k]*x[k];
            }
        }
    }
    tmp.x = ret[0];
    tmp.y = ret[1];
    tmp.z = ret[2];
    return tmp;
}
/*
Vertex matMultiply (vector <vector < double> > A, Vertex point) {
    Vertex ret;
    ret.x = A[0][0] * point.x + A[0][1] * point.y + A[0][2] * point.z;
    ret.y = A[1][0] * point.x + A[1][1] * point.y + A[1][2] * point.z;
    ret.y = A[2][0] * point.x + A[2][1] * point.y + A[2][2] * point.z;
    return ret;
}
*/
void rotationFaceX (double degree) {
    vector < vector < double > > rotationMatrix = makeRotateX(degree);
    for (int i=0; i<faces.size(); i++) {
        for (int j=0; j<faces[i].face_vertices.size(); j++) {
            faces[i].face_vertices[j] = matMultiply(rotationMatrix, faces[i].face_vertices[j]);
        }
    }
}
void rotationFaceY(double degree) {
    vector < vector < double > > rotationMatrix = makeRotateY(degree);
    for (int i=0; i<faces.size(); i++) {
        for (int j=0; j<faces[i].face_vertices.size(); j++) {
            faces[i].face_vertices[j] = matMultiply(rotationMatrix, faces[i].face_vertices[j]);
        }
    }
}

void rotationFaceZ(double degree) {
    vector < vector < double > > rotationMatrix = makeRotateZ(degree);
    for (int i=0; i<faces.size(); i++) {
        for (int j=0; j<faces[i].face_vertices.size(); j++) {
            faces[i].face_vertices[j] = matMultiply(rotationMatrix, faces[i].face_vertices[j]);
        }
    }
}
void rotate_left() {
	rotationFaceX(DEGREE);
}

void rotate_right() {
	rotationFaceX(-DEGREE);
}

void rotate_up() {
	rotationFaceY(DEGREE);
}

void rotate_down() {
	rotationFaceY(-DEGREE);
}
void rotate_z_cl() {
	rotationFaceZ(DEGREE);
}

void rotate_z_acl() {
	rotationFaceY(-DEGREE);
}
void move(unsigned char input, int x, int y) {
	//printf("%d pressed\n", input);
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
    drawFrame(faces);
}

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

int main(int argc, char **argv) {
    FILE * pFile;
    pFile = fopen ("cow.ply","r");
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
    lightSource.x = 16000;
    lightSource.y = 0;
    lightSource.z = 0;
    printf("DONE\n");

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
