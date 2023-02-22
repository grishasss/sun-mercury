#include <GL/glut.h>
#include "bits/stdc++.h"
#include "Vector.h"


#ifdef EXACTLY
	#define double long double
#endif


using namespace std;
const int FPS = 100; 	// frame per second
const int CPF = 100000; 	// calc per frame

// of corse FPS * CPF <= 1e8

const double G = 6.67430* 10e-11;


void reshape(GLint w, GLint h) {
  	glViewport(0, 0, w, h);
  	GLfloat aspect = (GLfloat)w / (GLfloat)h;
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	if (w <= h) {
		glOrtho(-50.0, 50.0, -50.0/aspect, 50.0/aspect, -1.0, 1.0);
  	} else {
		glOrtho(-50.0*aspect, 50.0*aspect, -50.0, 50.0, -1.0, 1.0);
  	}
}







const double degRad = 180. / M_PI;

void  triangle(Vector a , Vector b , Vector c){
	glBegin(GL_TRIANGLES);
	glVertex3d(a.x , a.y , a.z);
	glVertex3d(b.x , b.y , b.z);
	glVertex3d(c.x , c.y , c.z);
	glEnd();
}

void sphere(double r , bool light = 0){
  	int cnt1 = 70;
  	int cnt2 = 70;
  	vector<vector<Vector>> A(cnt1 + 1 , vector<Vector>(cnt2));
  	for(int i = 0; i <= cnt1; i++){
  		double a = 180. * i / (cnt1);
  		double h = cos(a / degRad) * r;
  		double rr = sin(a / degRad)* r;
  		for(int j = 0; j < cnt2; j++){
  			A[i][j] = Vector(sin(2 * M_PI* j / cnt2) * rr, cos(2 * M_PI* j / cnt2) * rr ,h);
  		}
  	}
  	for(int i = 0 ; i < cnt1 ; i++){
  		for(int j = 0 ; j < cnt2  ; j++){
  			Vector a = (A[i + 1][j] - A[i][j]) % (A[i + 1][(j + 1) % cnt2] - A[i][j]);
  			a.tonormal();
  			if(!light) a = a * (-1);
  			glNormal3f(a.x , a.y , a.z);
  			float u[] = {1 , 1,  1 , 1};
  			// glMaterialfv(GL_BACK, GL_AMBIENT, u);
  			
  			triangle(A[i][j] , A[i + 1][j] , A[i + 1][(j + 1) % cnt2]);
  			// glColor3f(1, 1, 1);
  			triangle(A[i][j] , A[i][(j + 1) % cnt2] , A[i + 1][(j + 1) % cnt2]);
  		}
  	}
}

void circle(double r){ // on (xoy)
	int cnt = 100;
	for(int i = 0; i < cnt;i++){
		triangle(Vector(0 , 0) , Vector(r , 0), 
			Vector(r * cos(M_PI * 2 / cnt) , r * sin(M_PI * 2 / cnt)));
		glRotatef(360. / cnt , 0 , 0 , 1);
	}
}

double b = 0;
class Obj{
public:
	Vector velocity , cord;
	double mass , period;
};

Obj sun , mercury;
  	

double post = 0;
void recalc(Obj &a , Obj &b , double dt){
	Vector r = b.cord - a.cord;
	Vector F = r * pow(r.abs() ,  -3) * a.mass * b.mass * G * (-1);
	b.velocity = b.velocity + F * (dt / b.mass);
	a.velocity = a.velocity - F * (dt / a.mass);
	b.cord =  b.cord + b.velocity * dt;
	a.cord =  a.cord + a.velocity * dt;

	
	post = clock();
}

double calcEng(vector<Obj> a){
	double Eng = 0;
	
	for(int i = 0; i < a.size() ; i++){
		Eng += a[i].mass / 2 * a[i].velocity.sabs();
		for(int j = 0; j < i; j++){
			Eng += -G * a[i].mass * a[j].mass / (a[j].cord - a[i].cord).abs(); 
		}
	}
	return Eng;
}

void display() {
	cout << "time between frame: "<< (clock() - post) / CLOCKS_PER_SEC << endl; 	
  	glClear(GL_COLOR_BUFFER_BIT);
 	// glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	
  	for(int iter = 0; iter < CPF ; iter++){
  		recalc(sun , mercury , 0.1);
  	}

  	cout << calcEng({sun , mercury}) << endl;


  	glEnable(GL_LIGHTING);
  	glEnable(GL_LIGHT0);
  	glEnable(GL_COLOR_MATERIAL);

  	glEnable(GL_DEPTH_TEST);
 
  	glRotatef(70 ,1, 0 , 0);

  	double scale  = 1 / 1.496e11;
  	// glScalef(scale , scale , 1);
  	glTranslatef(sun.cord.x * scale ,  sun.cord.y * scale , 0);
  	
  	float pos[] = {0,  0, 0 , 1};
  	glLightfv(GL_LIGHT0 , GL_POSITION , pos);
  	
  	glColor3f(1 , 1 , 1);
  	sphere(0.2 , 1);
  	
  	glLoadIdentity();
  	glRotatef(70 , 1 , 0 , 0);

  	glTranslatef(mercury.cord.x * scale ,  mercury.cord.y * scale , 0);
  	glColor3f(255 / 255., 178/ 255., 102 / 255.);
  	
  	sphere(0.07 );
  	
  	// glFlush();
  	glutSwapBuffers();
}



void Trender(int v) {
	glutPostRedisplay();
  	glutTimerFunc(1000./FPS, Trender, 0);
}



void mouse (int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON) {
        exit(0);
    }
}
signed main(int argc, char** argv) {
	sun.mass = 1.98892 * 10e30;
  	sun.cord = Vector(0 , 0 , 0);
  	mercury.mass = 3.33022*10e23;
  	mercury.cord = Vector((0.20563593 + 1) * 57909227000 , 0 , 0);
	mercury.velocity = Vector(0, 500000 , 0);



  	glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  	glutInitWindowPosition(80, 80);
  	glutInitWindowSize(1000, 1000);
  	glutCreateWindow("1");
  	// glutReshapeFunc(reshape);
  	glutMouseFunc(mouse);

  	glutDisplayFunc(display);
  	glutTimerFunc(1000. / FPS, Trender, 0);
  	glutMainLoop();
}