//gcc midpointalgo.c -o t -lGL -LGLU -lglut
#include<bits/stdc++.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include<time.h>
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define pi 3.1416

using namespace std;

int n_p, n_i;

double li_x, li_y, li_z;

double x[2950], y[2950], z[2950];
//nx, ny, nz;
double cx[2950], cy[2950];
int in_one[5850], in_two[5850], in_three[5850];
int m=2500, d=1600;

int findZone(double X0, double Y0, double X1, double Y1)
{
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
void drawPixel(double X, double Y, int zone)
{
	glVertex2d((X*2) / WINDOW_WIDTH, (Y*2)/WINDOW_HEIGHT);
}
void drawLine(double X0, double Y0, double X1, double Y1)
{

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
    	for(int i=0; i<steps; i++)
	{
	    	x+=xD;
	    	y+=yD;
		glVertex2d((x*2) / WINDOW_WIDTH, (y*2)/WINDOW_HEIGHT);

    	}
	glEnd();
	//glFlush();
}
void draw_axis()
{
	glColor3f(1.0,1.0,1.0);
	glPointSize(1);
	drawLine(-320.0, 0.0, 320.0, 0.0);
	drawLine(0.0, 240.0, 0.0, -240.0);
}
void fix_cx_cy()
{
	for(int i=0; i<n_p; i++)
	{
		cx[i] = x[i]/(1+((z[i]+m)/d));
		cy[i] = y[i]/(1+((z[i]+m)/d));

	}
}
struct z_buf
{
	int ind;
	double val;
};
bool sortByVal(const z_buf &lhs, const z_buf &rhs) { return lhs.val < rhs.val; }
int no_ch_ind[8000];

void z_buffer()
{

	vector<z_buf> z_b(n_i);
	for(int i=0; i<n_i; i++)
	{
		z_b[i].ind = i;
		z_b[i].val = z[in_one[i]]+z[in_two[i]]+z[in_three[i]];

	}

	sort(z_b.begin(), z_b.end(), sortByVal);

	for(int p=0; p<n_i; p++)
	{
		no_ch_ind[p] = z_b[p].ind;
	}

}
void fill_poly(int i)
{
	glBegin(GL_POLYGON);
	glVertex2d((cx[in_one[i]]*2) / WINDOW_WIDTH, (cy[in_one[i]]*2)/WINDOW_HEIGHT);
	glVertex2d((cx[in_two[i]]*2) / WINDOW_WIDTH, (cy[in_two[i]]*2)/WINDOW_HEIGHT);
	glVertex2d((cx[in_three[i]]*2) / WINDOW_WIDTH, (cy[in_three[i]]*2)/WINDOW_HEIGHT);
	glEnd();

}
double shading(int i)
{


		double a, b, c, nx, ny, nz, lx, ly, lz, lig_x, lig_y, lig_z, x_c, y_c, z_c, tot_lig, div_by;
		double x1 = x[in_two[i]]-x[in_one[i]];
		double y1 = y[in_two[i]]-y[in_one[i]];
		double z1 = z[in_two[i]]-z[in_one[i]];
		double x2 = x[in_three[i]]-x[in_two[i]];
		double y2 = y[in_three[i]]-y[in_two[i]];
		double z2 = z[in_three[i]]-z[in_two[i]];
		a = y1*z2 - y2*z1;
		b = -x1*z2 + x2*z1;
		c = x1*y2 - x2*y1;

		//normal
		//a = (y[in_one[i]]*z[in_two[i]]) - (y[in_two[i]]*z[in_one[i]]);
		//b = - (x[in_one[i]]*z[in_two[i]]) + (x[in_two[i]]*z[in_one[i]]);
		//c = (x[in_one[i]]*y[in_two[i]]) - (x[in_two[i]]*y[in_one[i]]);

		div_by = sqrt((a*a)+(b*b)+(c*c));
		nx = a/div_by;
		ny = b/div_by;
		nz = c/div_by;

		//printf("nx = %lf\nny = %lf\nnz = %lf\n", nx, ny, nz);
		//lightsource

        x_c = (x[in_one[i]]+x[in_two[i]]+x[in_three[i]])/3;
		y_c = (y[in_one[i]]+y[in_two[i]]+y[in_three[i]])/3;
		z_c = (z[in_one[i]]+z[in_two[i]]+z[in_three[i]])/3;

		lig_x = li_x - x_c;
		lig_y = li_y - y_c;
		lig_z = li_z - z_c;

		tot_lig = sqrt((lig_x*lig_x)+(lig_y*lig_y)+(lig_z*lig_z));
		//printf("nx = %lf\nny = %lf\nnz = %lf\n", nx, ny, nz);

		lx = lig_x/tot_lig;
		ly = lig_y/tot_lig;
		lz = lig_z/tot_lig;
		//printf("lx = %lf\nly = %lf\nlz = %lf\n", lx, ly, lz);

		double p;

		p = (nx*lx+ny*ly+nz*lz);

		return p;

}
void draw_poly()
{
	z_buffer();
	//int ind;

	for(int i=0; i<n_i; i++)
	{

		double k = shading(no_ch_ind[i]);


			glColor3f(1.0*k,1.0*k,1.0*k);
			drawLine(cx[in_one[no_ch_ind[i]]], cy[in_one[no_ch_ind[i]]], cx[in_two[no_ch_ind[i]]], cy[in_two[no_ch_ind[i]]]);
			drawLine(cx[in_two[no_ch_ind[i]]], cy[in_two[no_ch_ind[i]]], cx[in_three[no_ch_ind[i]]], cy[in_three[no_ch_ind[i]]]);
			drawLine(cx[in_three[no_ch_ind[i]]], cy[in_three[no_ch_ind[i]]], cx[in_one[no_ch_ind[i]]], cy[in_one[no_ch_ind[i]]]);
			//glColor3f(1.0*k,1.0*k,1.0*k);
			fill_poly(no_ch_ind[i]);



	}

}
void rotationy(double degree)
{
	for(int i=0; i<n_p; i++)
	{
		double xx, yy, zz;
		zz = z[i]*cos(degree*(pi/180)) - x[i]*sin(degree*(pi/180));
		xx = z[i]*sin(degree*(pi/180)) + x[i]*cos(degree*(pi/180));


		x[i] = xx;
		z[i] = zz;

	}

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	fix_cx_cy();
	draw_axis();
	draw_poly();
	glFlush();

}
void rotationx(double degree)
{
	for(int i=0; i<n_p; i++)
	{
		double xx, yy, zz;
		yy = y[i]*cos(degree*(pi/180)) - z[i]*sin(degree*(pi/180));
		zz = y[i]*sin(degree*(pi/180)) + z[i]*cos(degree*(pi/180));

		y[i] = yy;
		z[i] = zz;
	}


	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	fix_cx_cy();
	draw_axis();
	draw_poly();
	glFlush();

}
void rotationz(double degree)
{
	for(int i=0; i<n_p; i++)
	{
		double xx, yy;
		xx = x[i]*cos(degree*(pi/180)) - y[i]*sin(degree*(pi/180));
		yy = x[i]*sin(degree*(pi/180)) + y[i]*cos(degree*(pi/180));


		x[i] = xx;
		y[i] = yy;
	}


	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	fix_cx_cy();
	draw_axis();
	draw_poly();
	glFlush();

}
void rotate_left()
{
	rotationx(-1);

}
void rotate_right()
{
	rotationx(1);

}
void rotate_up()
{
	rotationy(1);

}
void rotate_down()
{
	rotationy(-1);

}
void rotate_z_cl()
{
	rotationz(1);

}
void rotate_z_acl()
{
	rotationz(-1);

}

void fix_lx()
{
	li_x = 16000;
	li_y = 0;
	li_z = 0;
}
void fix_ly()
{
	li_x = 0;
	li_y = 16000;
	li_z = 0;
}
void fix_min()
{
	li_x = 16000;
	li_y = 16000;
	li_z = 0;
}
void fix_lz()
{
	li_x = 0;
	li_y = 0;
	li_z = 16000;
}

void rotate(int k, int x, int y)
{
	switch(k)
	{
		case 27: break;

		case 104: fix_min(); break;
		case 105: fix_lx(); break;
		case 106: fix_ly(); break;
		//case 107: fix_min_ly(); break;
		case 107: fix_lz(); break;
		//case 107: fix_min_ly(); break;

		case 100: rotate_up(); break;
		case 102: rotate_down(); break;
		case 101: rotate_left(); break;
		case 103: rotate_right(); break;
		//case 105: rotate_z_cl(); break;
		//case 104: rotate_z_acl(); break;

	}
}
void draw(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPointSize(2);


}
int main(int argc, char **argv)
{
	int k;

	//printf("light vector : \n");
	//scanf("%lf%lf%lf", &li_x, &li_y, &li_z);
	printf("Enter 9 for light from right.\nEnter 7 for light from up.\nEnter 1 for light from front.\nEnter 3 for light from (x,y).\n");
	//freopen("cow.ply","r",stdin);
	freopen("airplane.ply","r",stdin);

 	scanf("%d", &n_p);
	scanf("%d", &n_i);

	for(int i=0; i<n_p; i++)
	{

 		scanf("%lf%lf%lf",&x[i],&y[i], &z[i]);

		//for plane
		x[i] = x[i]/2;
		y[i] = y[i]/2;
		z[i] = z[i]/2;

		//for goru
		/*x[i] = x[i]*200;
		y[i] = y[i]*200;
		z[i] = z[i]*200;*/
	}
	//for(int i=0; i<n_p; i++)
	//printf("X = %lf, Y = %lf, Z = %lf\n", x[i], y[i], z[i]);


	for(int i=0; i<n_i; i++)
	{

 		scanf("%d %d %d %d",&k, &in_one[i],&in_two[i], &in_three[i]);

	}

	//for(int i=0; i<n_i; i++)
	//printf("%d %d %d %d", k, in_one[i], in_two[i], in_three[i]);

 	glutInit(&argc, argv);
 	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-640)/2, (glutGet(GLUT_SCREEN_HEIGHT)-480)/2);
	glutInitWindowSize(640,480);
	glutCreateWindow("Plane Goru Window");
	glutDisplayFunc(draw);
	glutSpecialFunc(rotate);
	glutMainLoop();

	return 0;
}



