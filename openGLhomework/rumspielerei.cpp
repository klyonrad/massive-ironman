// GD-Praktikum:  kugeln.cpp by Jannis
// Hergenroether / Groch    Last Update: 29.07.2011

#include <iostream> 
#include <cstdlib>
#include <math.h>
#include <Windows.h>
#include <GL/freeglut.h>         //lädt alles für OpenGL

#define PI 3.141592654

float angle=-75;
int MAX = (int) angle;
float factor=2.0;
float circulate = 1.0;
float crate = 0.4;

bool leftball = true;
bool pause = false;

double xpos = 3.0;
double zpos = 15;

using namespace std;


SHORT WINAPI GetAsyncKeyState(
  _In_  int vKey
);


void boden() {
	  glPushMatrix();
	  
	  glColor4f(0.5,0.,0.5,1.0);
	  glBegin(GL_POLYGON);
	  glNormal3f(0,1,0);
	  glVertex3f(-50,-8.5,50);
	  glVertex3f(50,-8.5,50);
	  glVertex3f(50,-8.5,-50);
	  glVertex3f(-50,-8.5,-50);
	  glEnd();
	  glPopMatrix();
}

void kugelgebilde() {
	glPushMatrix();
	{
	  glPushMatrix();
		glColor4f(0.2,0.2,0.2,1.);
		glTranslatef(0.0,-1.5,0.0);
		glScalef(1.0,30.0,1.0);
		glutSolidCube(0.1);
	  glPopMatrix();
	  
	  glColor4f(0.0,1.,0.,0.75);	
	  glTranslatef(0.0,-3,0.0);
	  glutSolidSphere(0.5,100,100);
	}
	glPopMatrix();
}

void kugelReihe(int breite) { // laaaaaag with AMD A10 APU
	for (int i = -breite; i <= breite; i++) {
		glPushMatrix();
		glTranslatef(i, 0, 0);
		kugelgebilde();
		glPopMatrix();
	}
}

void Init()	
{  

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	GLfloat light_pos [] = {1,1,1,0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glEnable(GL_COLOR_MATERIAL);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

   glEnable(GL_DEPTH_TEST);
   glClearDepth(1.0);
}

void RenderScene()
{
   glLoadIdentity ();
   	  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   	  
   	  glEnable(GL_BLEND);
   	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   	  
	  
   	  glClearColor(0.,0.,1.0,1.0);
	  gluLookAt(xpos, 3, zpos,0.,0.,0.,0.,1.,0.);
	  boden();
	  circulate += crate;
	  glRotatef(circulate,0.0,1.0,0.0); // rotate ALL the things!
	  
	  glPushMatrix();	  	  
	  glPushMatrix();
	  
		glTranslatef(-5, 0, 0); // Kugelgebilde nach links verschieben
		if(leftball){
			glRotatef(angle,0.0,0.0,1.0);
		}
		kugelgebilde();
		

	  glPopMatrix();
		glTranslatef(5, 0, 0); // Kugelgebilde nach rechts verschieben
		if(!leftball){
			glRotatef(-angle,0.0,0.0,1.0);
		}
		kugelgebilde();
		

	glPopMatrix();

	kugelReihe(4);
	  
	  glPushMatrix();
	  glColor4f(1.0,0.5,0.0,0.3);
	  glScalef(170.0,1.0,1.0);
	  glutSolidCube(0.1);  // Stange

	  glPopMatrix();
	 
	  kugelReihe(1);
	  
		glPushMatrix();
			glColor4f(1.0,0.5,0.0,0.3);
			glScalef(170.0,1.0,1.0);
			glutSolidCube(0.1);  // Stange
		glPopMatrix();
	  
	  glPopMatrix(); 
	  
	  glutSwapBuffers();	
   glFlush(); //Buffer leeren
}

double round(double x){
	x = (int)(x*100+0.5)/100.0;
	return x;
}

void Reshape(int width,int height)
{
	// Matrix für Transformation: Frustum->viewport
	glMatrixMode(GL_PROJECTION);
	// Aktuelle Transformations-Matrix zuruecksetzen
	glLoadIdentity ();
	// Viewport definieren
	glViewport(0,0,width,height);
    gluPerspective(80., 1., 0.1, 500.);	
	glMatrixMode(GL_MODELVIEW);
}

void keyEvents(){
	if(GetAsyncKeyState(0x13))
	   pause = !pause;
	if(GetAsyncKeyState(0x1b))		//Esc beendet
	   exit(0);
   if(GetAsyncKeyState(0x57))		//Abfragen ob 'W' gedrückt wurde
	   xpos -= 0.1;
   if(GetAsyncKeyState(0x53))		//Abfragen ob 'S' gedrückt wurde
	   xpos += 0.1;
   if(GetAsyncKeyState(0x44))		//Abfragen ob 'A' gedrückt wurde
	   zpos -= 0.1;
   if(GetAsyncKeyState(0x41))		//Abfragen ob 'D' gedrückt wurde
	   zpos += 0.1;
   if(GetAsyncKeyState(0x20)){      //Abfragen ob 'Space' gedrückt wurde
	   angle = MAX = -75;			//Neustart
	   factor = 2;
   }

   if(GetAsyncKeyState(0x6b))		// '+' beschleunigt
	   crate += 0.1;
   if(GetAsyncKeyState(0x6d))		// '-' verlangsamt
	   crate -= 0.1;
}


void Animate (int value)    
{  
   // RenderScene aufrufen
   glutPostRedisplay();
   
   keyEvents();

   if(!pause){
	   if(crate == 0)
		   crate = 0.4;
	   if(abs(MAX) <= 4)				//Stoppen wenn Kugeln ausgependelt sind
		   factor = angle =  0;
		angle=angle + factor * round(cos(angle * PI / 180));

	if(((int)angle<=1 && (int) angle>= 0) || ((int) angle< MAX + 1)  ) // wegen Ungenauigkeit	    
	   factor=factor*-1;
	if(((int)angle<=1 && (int) angle>= 0)){
	   leftball = !leftball;
	   MAX *= 0.96;
	}
	// Timer wieder registrieren; Animate wird so nach 10 msec mit value+=1 aufgerufen
   }else{
	   crate = 0;
   }
   glutTimerFunc(20, Animate, ++value);  
        
}

int main(int argc, char **argv)
{
   glutInit( &argc, argv );                // GLUT initialisieren
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitWindowSize( 600, 600 );         // Fenster-Konfiguration
   glutCreateWindow( "Klick-Klack" );   // Fenster-Erzeugung
   glutDisplayFunc( RenderScene );         // Zeichenfunktion bekannt machen
   glutReshapeFunc( Reshape );
   // TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
   glutTimerFunc(10, Animate, 0);
   Init();
   glutMainLoop();
   return 0;
}
