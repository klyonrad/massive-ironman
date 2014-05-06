// GD-Praktikum:  kugeln.cpp by Jannis
// Hergenroether / Groch    Last Update: 29.07.2011

#include <iostream> 
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>         //lädt alles für OpenGL

#define PI 3.141592654

int angle=-75;
int factor=2;
int circle=0;
int camcircle=0;
float dist=500;


void boden() { // completely fromjannis
	  glPushMatrix(); 
	  glColor4f(0.5,0.5,0.5,1.0);
	  glBegin(GL_POLYGON);
	  glVertex3f(-10,-8.5,500);
	  glVertex3f(10,-8.5,500);
	  glVertex3f(10,-8.5,-500);
	  glVertex3f(-10,-8.5,-500);
	  glEnd();
	  glPopMatrix();
}

void kugelgebilde() {
	glPushMatrix();
	{
	  glColor4f(0.0,1.,0.,0.75);	  
	  glutSolidSphere(0.1,10,10);	  
	  glTranslatef(0.0,-1,0.0);
  	  glutSolidSphere(0.1,10,10);
  	  glTranslatef(0.0,-1,0.0);
  	  glutSolidSphere(0.1,10,10);
  	  glTranslatef(0.0,-1,0.0);
  	  glutSolidSphere(0.1,10,10);
	  glTranslatef(0.0,-1.25,0.0);
	  glutSolidSphere(0.5,100,100);
	}
	glPopMatrix();
}

void kugelReihe(int breite) { // laaaaaag with AMD A10-5800K.
	for (int i = -breite; i <= breite; i++) {
		glPushMatrix();
		glTranslatef(i, 0, 0);
		kugelgebilde();
		glPopMatrix();
	}
}

void Init()	
{  
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
	  gluLookAt(0.0,-8.5+5,15.,0.,0.,0.,0.,1.,0.);
	  
	  // glRotatef(-10.0,0.0,1.0,0.0);
	  
	  glPushMatrix();
	  
	  //glTranslatef(0.0, sin(circle*PI/180)+1 ,0.0); // Das rollende Ding soll sich auf und ab bewegen
	  
	  glPushMatrix();
	  
		glTranslatef(-5, 0, 0); // Kugelgebilde nach links verschieben
		glRotatef(angle,0.0,0.0,1.0);
		kugelgebilde();
		


	glPopMatrix();
		glTranslatef(5, 0, 0); // Kugelgebilde nach rechts verschieben
		glRotatef(-angle,0.0,0.0,1.0);
		kugelgebilde();

	glPopMatrix();

	kugelReihe(4);




	  
	  glPushMatrix();
	  glColor4f(1.0,0.5,0.0,0.3);
	  glScalef(170.0,1.0,1.0);
	  glutSolidCube(0.1);  // Stange
	  glPopMatrix();
	  
	  
	  glPopMatrix(); 
	  boden();
    
	  glutSwapBuffers();	
   glFlush(); //Buffer leeren

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

void Animate (int value)    
{  
   // RenderScene aufrufen
   glutPostRedisplay();
   
   circle=(circle+8) % 360;   
   camcircle=(camcircle+1) % 360;   

   dist+=0.75;
   dist=(dist >10) ? -500 : dist;

   angle=angle+factor;
   if(angle==1 || angle==-91) // muss ungerade sein, da factor gerade ist
	   factor=factor*-1;
   // Timer wieder registrieren; Animate wird so nach 100 msec mit value+=1 aufgerufen
   glutTimerFunc(10, Animate, ++value);          
}

int main(int argc, char **argv)
{
   glutInit( &argc, argv );                // GLUT initialisieren
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitWindowSize( 600, 600 );         // Fenster-Konfiguration
   glutCreateWindow( "kette" );   // Fenster-Erzeugung
   glutDisplayFunc( RenderScene );         // Zeichenfunktion bekannt machen
   glutReshapeFunc( Reshape );
   // TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
   glutTimerFunc(10, Animate, 0);
   Init();
   glutMainLoop();
   return 0;
}
