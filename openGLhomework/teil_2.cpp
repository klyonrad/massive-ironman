// GD-Praktikum:   teil_1.cpp  (Teil 1: Start-Programm)
// Hergenroether / Groch    Last Update: 29.07.2011

#include <iostream> 
#include <GL/freeglut.h>      //lädt alles für OpenGL
#include "wuerfel.hpp"

void Init()	
{
	// Hier finden jene Aktionen statt, die zum Programmstart einmalig 
	// durchgeführt werden müssen	
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(1, 0.5, 0 , 0); // sets the background color to: orange 
}

void RenderScene() //Zeichenfunktion
{
	// Hier befindet sich der Code der in jedem Frame ausgefuehrt werden muss
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity ();   // Aktuelle Model-/View-Transformations-Matrix zuruecksetzen
	gluLookAt (1, 1, 0, 
		0, 0, 0, 
		-1, 1, 0);
	Wuerfel(0.4);
	glFlush(); //Buffer leeren   
}

void Reshape(int width,int height)
{
	/* Hier finden die Reaktionen auf eine Veränderung der Größe des Graphikfensters statt */

	// kopiert von Aufgabestellung:
	glMatrixMode(GL_PROJECTION);	// Matrix für Transformation: Frustum->viewport 	
	glLoadIdentity ();				// Aktuelle Transformations-Matrix zuruecksetzen 
	glViewport(0,0,width,height);	// Viewport definieren 	 
	glOrtho( -1, 1, -1, 1, 
		0, 2);	// Frustum definieren (siehe unten)	
	glMatrixMode(GL_MODELVIEW);		// Matrix für Modellierung/Viewing 
}

void Animate (int value)    
{
	/* Hier werden Berechnungen durchgeführt, die zu einer Animation der Szene  
	erforderlich sind. Dieser Prozess läuft im Hintergrund und wird alle 
	1000 msec aufgerufen. Der Parameter "value" wird einfach nur um eins 
	inkrementiert und dem Callback wieder uebergeben. */
	std::cout << "value=" << value << std::endl;
	// RenderScene aufrufen
	glutPostRedisplay();
	// Timer wieder registrieren; Animate wird so nach 100 msec mit value+=1 aufgerufen
	glutTimerFunc(100, Animate, ++value);          
}

int main(int argc, char **argv)
{
	glutInit( &argc, argv );                // GLUT initialisieren
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);        
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
