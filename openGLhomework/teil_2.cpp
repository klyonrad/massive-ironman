// GD-Praktikum:   teil_1.cpp  (Teil 1: Start-Programm)
// Hergenroether / Groch    Last Update: 29.07.2011

#include <iostream> 
#include <GL/freeglut.h>      //l�dt alles f�r OpenGL
#include "wuerfel.hpp"

void Init()	
{
   // Hier finden jene Aktionen statt, die zum Programmstart einmalig
   // durchgef�hrt werden m�ssen
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

void RenderScene() //Zeichenfunktion
{
   // Hier befindet sich der Code der in jedem Frame ausgefuehrt werden muss
	glLoadIdentity ();   // Aktuelle Model-/View-Transformations-Matrix zuruecksetzen

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//Erweiterung f�r die Z -Achse
	glClearColor( 1., 0.5, 0.0, 1.0 );

	Wuerfel(0.4);

	
	glFlush(); //Buffer leeren   
}

void Reshape(int width,int height)
{
   // Hier finden die Reaktionen auf eine Ver�nderung der Gr��e des 
   // Graphikfensters statt
	// Matrix f�r Transformation: Frustum->viewport
	glMatrixMode(GL_PROJECTION);
	// Aktuelle Transformations-Matrix zuruecksetzen
	glLoadIdentity ();
	// Viewport definieren
	glViewport(0,0,width,height);
	// Frustum definieren (siehe unten)

	//11a:
	//glOrtho( -1, 1, -1., 1., 0.0, 2.0);
	//gluLookAt ( 0., 1, 1, 0., 0., 0., 0., 1., 0.);		//Betrachte den W�rfel von Vorne Oben

	//11b:
	//glOrtho( -1, 1, -1., 1., 0.0, 1.0);
	//gluLookAt ( 1., 0, 0, 0., 0., 0., 0., 1., 0.);			//Betachte den W�rfel von Oben

	//11c:
	gluPerspective(45., 1., 0.1, 2.0);						//Stelle den W�rfel entzerrt dar
	gluLookAt ( 1., 1, 1, 0., 0., 0., 0., 1., 0.);			//Betracte den W�rfel von Oben rechts


	// Matrix f�r Modellierung/Viewing
	glMatrixMode(GL_MODELVIEW);
}

void Animate (int value)    
{
   // Hier werden Berechnungen durchgef�hrt, die zu einer Animation der Szene  
   // erforderlich sind. Dieser Prozess l�uft im Hintergrund und wird alle 
   // 1000 msec aufgerufen. Der Parameter "value" wird einfach nur um eins 
   // inkrementiert und dem Callback wieder uebergeben. 
   std::cout << "value=" << value << std::endl;
   // RenderScene aufrufen 
   glutPostRedisplay();
  
   // Timer wieder registrieren; Animate wird so nach 100 msec mit value+=1 aufgerufen
   glutTimerFunc(100, Animate, ++value);          
}

int main(int argc, char **argv)
{
   glutInit( &argc, argv );                // GLUT initialisieren
   glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB |GLUT_DEPTH);      //Erweiterung f�r die Z Achse  
   glutInitWindowSize( 600, 600 );         // Fenster-Konfiguration
   glutCreateWindow( "Luka; Max" );   // Fenster-Erzeugung
   glutDisplayFunc( RenderScene );         // Zeichenfunktion bekannt machen
   glutReshapeFunc( Reshape );
   // TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
   glutTimerFunc( 10, Animate, 0);
   Init();
   glutMainLoop();
   return 0;
}
