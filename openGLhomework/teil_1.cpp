// GD-Praktikum:   teil_1.cpp  (Teil 1: Start-Programm)
// Hergenroether / Groch    Last Update: 29.07.2011

#include <iostream> 
#include <GL/freeglut.h>      //l�dt alles f�r OpenGL

void Init()	
{
	// Hier finden jene Aktionen statt, die zum Programmstart einmalig 
	// durchgef�hrt werden m�ssen	
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(1, 0.5, 0 , 0); // sets the background color to: orange 
}

void RenderScene() //Zeichenfunktion
{
	// Hier befindet sich der Code der in jedem Frame ausgefuehrt werden muss
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity ();   // Aktuelle Model-/View-Transformations-Matrix zuruecksetzen
	glBegin( GL_POLYGON ); {
		glColor4f( 0., 1., 0., 1.);
		glVertex3f( -0.5, -0.5, -1. );
		glVertex3f(  0.5, -0.5, -1. );
		glVertex3f(  0.5,  0.5, -1. );
		glVertex3f( -0.5,  0.5, -1. );
	}
	glEnd();
	glBegin( GL_POLYGON ); {
		glColor4f( 1.0, 0.0, 0.0, 1.0); // bottom left
		glVertex3f( -0.5, -0.5, -0.5 ); // actually red is more warm, but who cares...
		glColor4f( 0, 0, 1, 0); // blue is the warmest color
		glVertex3f(  0.5, -0.5, -0.5 ); // bottom right
		glColor4f( 0, 0, 1, 0); // blue is the warmest color
		glVertex3f(  0.5,  0.5, -0.5 ); // upper right
		glColor4f( 0, 0, 1, 0); // blue is the warmest color
		glVertex3f( -0.5,  0.5, -0.5 ); // upper left
	}
	glEnd(); 
	glFlush(); //Buffer leeren   

}

void Reshape(int width,int height)
{
	/* Hier finden die Reaktionen auf eine Ver�nderung der Gr��e des Graphikfensters statt */
	
	// kopiert von Aufgabestellung:
	glMatrixMode(GL_PROJECTION);  // Matrix f�r Transformation: Frustum->viewport 	
	glLoadIdentity ();  // Aktuelle Transformations-Matrix zuruecksetzen 
	glViewport(0,0,width,height); // Viewport definieren 	 
	glOrtho( -1., 1., -1., 1., 0.3, 1.3); // Frustum definieren (siehe unten)	
	glMatrixMode(GL_MODELVIEW); // Matrix f�r Modellierung/Viewing 
}

void Animate (int value)    
{
	/* Hier werden Berechnungen durchgef�hrt, die zu einer Animation der Szene  
	erforderlich sind. Dieser Prozess l�uft im Hintergrund und wird alle 
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
