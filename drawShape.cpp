
//g++ code.cpp -lGL -lGLU -lglut -o test
//#include <GL/GLUT.h>

/*
 * Zone 0 : red
 * Zone 1 : green
 * Zone 2 : blue
 * Zone 3 : yellow
 * Zone 4 : purple
 * Zone 5 : cyan
 * Zone 6 : white
 * Zone 7 : orange
 */
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;
int FindZone(int x0,int y0,int x1,int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	if(dx>=0 && dy>=0)
	{
		if(abs(dx)>=abs(dy)){
			//glColor3f(1.0,0,0);
			return 0;
		}
		else
		{
			//glColor3f(0,1.0,0);
			return 1;
		}
	}
	else if(dx>=0 && dy<0)
	{
		if(abs(dx)>=abs(dy)){
			//glColor3f(1,0.5,0);
			return 7;
		}
		else{
			//glColor3f(1.0,1.0,1.0);
			return 6;
			}
	}
	else if(dx<0 && dy>=0)
	{
		if(abs(dx)>=abs(dy))
		{
			//glColor3f(1.0,1.0,0.0);
			return 3;
		}
		else
		{
			//glColor3f(0,0,1.0);
			return 2;
		}
	}
	else if(dx<0 && dy<0)
	{
		if(abs(dx)>=abs(dy)){
			//glColor3f(1.0,0,1.0);
			return 4;
			}
		else{
			//glColor3f(0.0,1.0,1.0);
			return 5;
			}
	}
}
void drawPixel(double x,double y)
{
	glVertex2f(x,y);
}
void draw_line0(int x0,int y0,int x1,int y1)
{
	if(x0 > x1)
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 =temp;
	}
	int dx = x1 - x0;
	int dy = y1- y0;
	int d = 2*dy - dx;
	int dE = 2*dy;
	int dNE = 2*(dy-dx);
	int x = x0;
	int y = y0;
	glBegin(GL_POINTS);
	drawPixel(x,y);
	while(x<x1)
	{
		if(d<0)
		{
			x++;
			d += dE;
		}
		else
		{
			x++;
			y++;
			d += dNE;
		}
		//printf("%d %d\n",x,y);
		drawPixel(x,y);
	}
	glEnd();
	glFlush();
}
void draw_line1(int x0,int y0,int x1,int y1)
{
	if(y0 > y1)
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 =temp;
	}
	int dx = x1 - x0;
	int dy = y1- y0;
	int d = dy - 2*dx;
	int dA = -2*dx;
	int dD = 2*(dy-dx);
	int x = x0;
	int y = y0;
	glBegin(GL_POINTS);
	drawPixel(x,y);
	while(y<y1)
	{
		if(d<0)
		{
			x++;
			y++;
			d += dD;
		}
		else
		{
			y++;
			d += dA;
		}
		drawPixel(x,y);
	}
	glEnd();
	glFlush();
}
void draw_line2(int x0,int y0,int x1,int y1)
{
	if(y0 > y1)
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 =temp;
	}
	int dx = x1 - x0;
	int dy = y1- y0;
	int d = -dy - 2*dx;
	int dA = -2*dx;
	int dD = 2*(-dy-dx);
	int x = x0;
	int y = y0;
	glBegin(GL_POINTS);
	drawPixel(x,y);
	while(y<y1)
	{
		if(d<0)
		{
			y++;
			d += dA;
		}
		else
		{
			x--;
			y++;
			d += dD;
		}
		drawPixel(x,y);
	}
	glEnd();
	glFlush();
}
void draw_line3(int x0,int y0,int x1,int y1)
{
	if(x1 > x0)
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 =temp;
	}
	int dx = x1 - x0;
	int dy = y1- y0;
	int d = -dx - 2*dy;
	int dA = -2*dy;
	int dD = 2*(-dy-dx);
	int x = x0;
	int y = y0;
	glBegin(GL_POINTS);
	drawPixel(x,y);
	while(x>x1)
	{
		if(d<0)
		{
			x--;
			y++;
			d += dD;
		}
		else
		{
			x--;
			d += dA;
		}
		//printf("%d %d\n",x,y);
		drawPixel(x,y);
	}
	glEnd();
	glFlush();
}
void draw_line4(int x0,int y0,int x1,int y1)
{
	if(x1 > x0)
	{
		double temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 =temp;
	}
	double dx = x1 - x0;
	double dy = y1- y0;
	double d = -2*dy + dx;
	double dA = -2*dy;
	double dD = 2*(-dy+dx);
	double x = x0;
	double y = y0;
	glBegin(GL_POINTS);
	drawPixel(x,y);
	while(x>x1)
	{
		if(d<0)
		{
			x--;
			d += dA;
		}
		else
		{
			x--;
			y--;
			d += dD;
		}
		drawPixel(x,y);
	}
	glEnd();
	glFlush();
}
void draw_line5(double x0,double y0,double x1,double y1)
{
	if(y0 < y1)
	{
		double temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 =temp;
	}
	double dx = x1 - x0;
	double dy = y1- y0;
	double d = -dy + 2*dx;
	double dA = 2*dx;
	double dD = 2*(-dy+dx);
	double x = x0;
	double y = y0;
	glBegin(GL_POINTS);
	drawPixel(x,y);
	while(y>y1)
	{
		if(d<0)
		{
			x--;
			y--;
			d += dD;
		}
		else
		{
			y--;
			d += dA;
		}
		drawPixel(x,y);
	}
	glEnd();
	glFlush();
}
void draw_line6(double x0,double y0,double x1,double y1)
{
	if(y0 < y1)
	{
		double temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 =temp;
	}
	double dx = x1 - x0;
	double dy = y1- y0;
	double d = dy + 2*dx;
	double dA = 2*dx;
	double dD = 2*(dy+dx);
	double x = x0;
	double y = y0;
	glBegin(GL_POINTS);
	drawPixel(x,y);
	while(y>y1)
	{
		if(d<0)
		{
			y--;
			d += dA;
		}
		else
		{
			y--;
			x++;
			d += dD;
		}
		drawPixel(x,y);
	}
	glEnd();
	glFlush();
}
void draw_line7(double x0,double y0,double x1,double y1)
{
	if(x0 > x1)
	{
		double temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 =temp;
	}
	double dx = x1 - x0;
	double dy = y1- y0;
	double d = 2*dy + dx;
	double dA = 2*dy;
	double dD = 2*(dy+dx);
	double x = x0;
	double y = y0;
	glBegin(GL_POINTS);
	drawPixel(x,y);
	while(x<x1)
	{
		if(d>0)
		{
			x++;
			d += dA;
		}
		else
		{
			x++;
			y--;
			d += dD;
		}
		drawPixel(x,y);
	}
	glEnd();
	glFlush();
}
void draw(double x0,double y0,double x1,double y1)
{
	int zone = FindZone(x0,y0,x1,y1);
					if(zone==0)
					{
						draw_line0(x0,y0,x1,y1);
					}
					else if(zone==1)
					{
						draw_line1(x0,y0,x1,y1);
					}
					else if(zone==2)
					{
						draw_line2(x0,y0,x1,y1);
					}
					else if(zone==3)
					{
						draw_line3(x0,y0,x1,y1);
					}
					else if(zone==4)
					{
						draw_line4(x0,y0,x1,y1);
					}
					else if(zone==5)
					{
						draw_line5(x0,y0,x1,y1);
					}
					else if(zone==6)
					{
						draw_line6(x0,y0,x1,y1);
					}
					else if(zone==7)
					{
						draw_line7(x0,y0,x1,y1);
					}
}
#define pi 3.1415
struct point
{
	double x[3];
};
struct point cross(struct point p,struct point q)
{
	struct point ans;
	ans.x[0] = p.x[1]*q.x[2] - p.x[2]*q.x[1];
	ans.x[1] = p.x[2]*q.x[0] - p.x[0]*q.x[2];
	ans.x[2] = p.x[0]*q.x[1] - p.x[1]*q.x[0];
	double aa = ans.x[0]*ans.x[0] + ans.x[1]*ans.x[1] + ans.x[2]*ans.x[2];
	aa = sqrt(aa);
	for(int i = 0;i<3;i++)ans.x[i] /= aa;
	return ans;
}
struct point rotatePoint(struct point p,int i,int minus)
{
	struct point n;
	n.x[i] = p.x[i];
	n.x[(i+1)%3] = cos(minus*pi/180)*p.x[(i+1)%3] + sin(minus*pi/180)*p.x[(i+2)%3];
	n.x[(i+2)%3] = -sin(minus*pi/180)*p.x[(i+1)%3] + cos(minus*pi/180)*p.x[(i+2)%3];

	return n;
}
int n,tr;
struct point p[1000];
struct point pw[10000];
double d = 500;
int edgecnt[10005];
struct poly{

	vector <int> polygon;
	double dis;
	int edgecnt;
}a[10005];

bool cmp (struct poly a,struct poly b)
{
	return a.dis>b.dis;
}
struct point mid[100005];
void show()
{
	double mx = 0;
	glClear(GL_COLOR_BUFFER_BIT);
	struct point l;
	for(int i = 0;i<tr;i++)
	{
		memset(mid[i].x,0,sizeof(mid[i]));
		for(int j = 0;j<a[i].edgecnt;j++)
		{

			for(int k = 0;k<3;k++)
			{
				mid[i].x[k] += a[i].polygon[k];
			}
		}
			for(int k = 0;k<3;k++)
			{
				mid[i].x[k] /= a[i].edgecnt;
			}
				l = mid[i];
				l.x[2] = d - mid[i].x[2];
				double dis = 0;
				for(int k = 0 ;k<3;k++){
					cout<<mid[i].x[k]<<" ";
					dis += l.x[k]*l.x[k];
				}
				//cout<<endl;
				a[i].dis = sqrt(dis);
				mx = max(a[i].dis,mx);

	}
		//cout<<endl;
	sort(a,a+tr,cmp);
	double intensity = 0;
	for(int i = 0;i<tr;i++)
	{
		a[i].dis /=mx;
		glBegin(GL_POLYGON);
		for(int j = 0;j<a[i].edgecnt;j++)
		{
			double x[2],y[2];
			for(int k = 0;k<1;k++)
			{

				x[k] = pw[a[i].polygon[j]].x[0]/(1+pw[a[i].polygon[j]].x[2]/d);
				y[k] = pw[a[i].polygon[j]].x[1]/(1+pw[a[i].polygon[j]].x[2]/d);
					//cout<<x[k]<<" "<<y[k]<<" ";
			}

			glVertex2f(x[0],y[0]);
		}
		glColor3f(a[i].dis,a[i].dis,a[i].dis);
		glEnd();
		intensity += 0.00005;
		//cout<<intensity<<endl;
		//cout<<endl;
	}
	//
	cout<<intensity<<endl;
	glFlush();


}
void rot(int axis,int minus)
{
	minus *= 1;
	for(int i = 0;i<n;i++)
	{
		pw[i] = rotatePoint(pw[i],axis,minus);
	}
	show();
}

void processKey(int key,int x,int y)
{
	if(key==100)rot(1,1);//left
	if(key == 101)rot(0,1);//up
	if(key == 102)rot(1,-1);//right
	if(key == 103)rot(0,-1);//down
	if(key == 104)rot(2,1);
	if(key == 105) rot(2,-1);
	if(key == 106)exit(0);
}
void display()
{

	char line[10005];
	for(int i = 0;i < 6;i++)scanf("%s",line);
	scanf("%d",&n);
	for(int i = 0;i < 11;i++)scanf("%s",line);
	scanf("%d",&tr);
	for(int i = 0;i < 6;i++)scanf("%s",line);
	//cout<<n<<" "<<tr<<endl;
	for(int i = 0;i<n;i++)
	{
		for(int j = 0;j<3;j++)
		{
			scanf("%lf",&pw[i].x[j]);
			//pw[i].x[j] *= 1000;
		}
	}
	for(int i = 0;i<tr;i++)
	{
		scanf("%d",&a[i].edgecnt);
		//cout<<i<<" "<<a[i].edgecnt<<endl;
		for(int j = 0;j<a[i].edgecnt;j++)
		{
			int aa;
			scanf("%d",&aa);
			a[i].polygon.push_back(aa);
		}
	}
   show();

}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
	freopen("airplane.ply","r",stdin);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(600,600);
	d = -1000;
    glutCreateWindow("DRAW!!!!");


    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( -600.0, 600.0, -600.0,600.0 );
	glutDisplayFunc(display);
	glutSpecialFunc(processKey);
	glutMainLoop();
    return 0;
}

