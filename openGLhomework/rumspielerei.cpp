// GD-Praktikum:   teil_1.cpp  (Teil 1: Start-Programm)
// Hergenroether / Groch    Last Update: 29.07.2011

#include <iostream> 
#include <GL/freeglut.h>      //lädt alles für OpenGL
#include "wuerfel.hpp"

float fRotation = 0.0;		//ih globale Variable
bool down = true;

void Init()	
{
	// Hier finden jene Aktionen statt, die zum Programmstart einmalig
	// durchgeführt werden müssen
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

void RenderScene() //Zeichenfunktion
{
	// Hier befindet sich der Code der in jedem Frame ausgefuehrt werden muss
	glLoadIdentity ();   // Aktuelle Model-/View-Transformations-Matrix zuruecksetzen

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//Erweiterung für die Z -Achse
	glClearColor( 1., 0.5, 0.0, 1.0 );

	glutWireCube(0.2);

	{ // Drehung WireCubes:
		glTranslatef(0.1, 0.1, 0);
		glRotatef( fRotation, 0, 1, 0);
		glTranslatef(0.1, -0.1, 0);
	}
	
	glutWireCube(0.4);
	glutWireCube(0.6);
	glutWireCube(0.8);
	glutWireCube(1);
	glutWireCube(1.2);
	glutWireCube(1.4);
	glLoadIdentity();

	//Ablauf von unten nach oben
	//glRotatef( fRotation, 1, 0, 0);
	//glutWireTeapot(1);
	//glPushMatrix();


	//glPopMatrix();
	//Drehung
	glTranslatef(0.1, 0.1, 0);
	glRotatef( fRotation, 0, 0, 1);			//Ohne Push und Pop kann ich auch beide hier drehen und spare 1 Zeile / Operation
	glTranslatef(0.1, -0.1, 0);

	glPushMatrix();

	//Oberarm
	glTranslatef(0.8,0,0);
	glScalef(1.5, 0.25, 0.25);
	Wuerfel(0.4);

	glPopMatrix();

	//Unterarm
	glTranslatef(0.2,0,0);
	glScalef(1.5, 0.5, 0.5);
	Wuerfel(0.4);

	glutSwapBuffers();
	glFlush(); //Buffer leeren   
}

void Reshape(int width,int height)
{
	// Hier finden die Reaktionen auf eine Veränderung der Größe des 
	// Graphikfensters statt
	// Matrix für Transformation: Frustum->viewport
	glMatrixMode(GL_PROJECTION);
	// Aktuelle Transformations-Matrix zuruecksetzen
	glLoadIdentity ();
	// Viewport definieren
	glViewport(0,0,width,height);
	// Frustum definieren (siehe unten)

	//11a:
	glOrtho( -1.4, 1.4, -1.4, 1.4, 0.0, 3.0);				//Größerer Sichtbereich
	gluLookAt ( 1., 1., 1., 0., 0., 0., 0., 1., 0.);		//Betrachte den Würfel von Vorne Oben

	// Matrix für Modellierung/Viewing
	glMatrixMode(GL_MODELVIEW);
}

void Animate (int value)    
{
	// Hier werden Berechnungen durchgeführt, die zu einer Animation der Szene  
	// erforderlich sind. Dieser Prozess läuft im Hintergrund und wird alle 
	// 100 msec aufgerufen. Der Parameter "value" wird einfach nur um eins 
	// inkrementiert und dem Callback wieder uebergeben. 
	std::cout << "value=" << value << std::endl;

	//Realistische Rotation um 175°
	if(down){
		if(fRotation == -85.0)
			down = false;
		fRotation -= 0.5;
	}
	else{
		if(fRotation == 90.0)
			down = true;
		fRotation += 0.5;
	}
	//Originale Rotation 360°
	/*
	fRotation = fRotation - 1.0; // Rotationswinkel aendern
	if ( fRotation <= 0.0) {
	fRotation = fRotation + 360.0;
	}*/


	// RenderScene aufrufen 
	glutPostRedisplay();

	// Timer wieder registrieren; Animate wird so nach 100 msec mit value+=1 aufgerufen
	glutTimerFunc(10, Animate, ++value);          
}

int main(int argc, char **argv)
{
	glutInit( &argc, argv );                // GLUT initialisieren
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);      //Erweiterung für die Z Achse  
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
