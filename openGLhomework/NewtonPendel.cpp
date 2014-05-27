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

bool start =  true;
bool leftball = true;
bool pause = false;

double xpos = 3.0;
double zpos = 15;

GLuint himmelnr = 0; // eindeutige Nummer für die Textur
BYTE* hibuf;

using namespace std;


SHORT WINAPI GetAsyncKeyState(
  _In_  int vKey
);

BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, int width, int height )
{
	int padding = 0;
	int scanlinebytes = width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 )
		padding++;
	int psw = scanlinebytes + padding;
	BYTE* newbuf = new BYTE[width*height*3];
	long bufpos = 0;   
	long newpos = 0;
	for ( int y = 0; y < height; y++ )
		for ( int x = 0; x < 3 * width; x+=3 )
		{
			newpos = y * 3 * width + x;     
			bufpos = ( height - y - 1 ) * psw + x;
			newbuf[newpos] = Buffer[bufpos + 2];       
			newbuf[newpos + 1] = Buffer[bufpos+1]; 
			newbuf[newpos + 2] = Buffer[bufpos];     
		}
		delete Buffer;

	return newbuf;
}

BYTE* LoadBMP ( int* width, int* height, long* size, LPCTSTR bmpfile )
{
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	DWORD bytesread;
	HANDLE file = CreateFile ( bmpfile , GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	ReadFile ( file, &bmpheader, sizeof ( BITMAPFILEHEADER ), &bytesread, NULL );

	ReadFile ( file, &bmpinfo, sizeof ( BITMAPINFOHEADER ), &bytesread, NULL );

	*width   = bmpinfo.biWidth;
	*height  = abs ( bmpinfo.biHeight );
	*size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* buffer = new BYTE[ *size ];
	ReadFile ( file, buffer, *size, &bytesread, NULL );
	
	CloseHandle ( file );
	glPixelStorei ( GL_UNPACK_ALIGNMENT,   4 );
	glPixelStorei ( GL_UNPACK_ROW_LENGTH,  0 );
	glPixelStorei ( GL_UNPACK_SKIP_ROWS,   0 );
	glPixelStorei ( GL_UNPACK_SKIP_PIXELS, 0 );
	BYTE *ergbuffer = ConvertBMPToRGBBuffer( buffer, *width, *height );
	return ergbuffer;
}

void textur(){
	// Textur in Karte erzeugen
    glGenTextures(1, &himmelnr);
	
    // Mit unserer erzeugten Textur arbeiten
    glBindTexture(GL_TEXTURE_2D, himmelnr);
	static int width;
	static int height;
	static long size;
	if(hibuf == NULL)
		hibuf = LoadBMP(&width, &height, &size, L"himmel2.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	 // Textur in die Grafikkarte hochladen
    glTexImage2D(GL_TEXTURE_2D,     // 2D Textur
                 0,                 // Detailsstufe (für Mipmaps)
                 3,                 // Farbkomponenten
                 width,				// Breite
                 height,			// Höhe
                 0,                 // Rand
				 GL_RGB,			// Pixel-Format
                 GL_UNSIGNED_BYTE,  // Datentyp der Komponenten (0 bis 255)
                 hibuf);           // Pixel-Puffer
	
}

void himmel(){
	glPushMatrix();
		glColor4f(3.0f, 3.0f, 3.0f, 1.0f);
		textur();
		glutSolidSphere(50,100,100);
		glDeleteTextures(1, &himmelnr);
	glPopMatrix();
}


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

void kugelgebilde() { // Kugel, die mit Stange zusammenhängt
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

void kugelReihe(int breite) { // breite ist anzahl von kugeln. einfügen und nebeneinander positionieren
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
	GLfloat light_pos [] = {1,2,0,0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glEnable(GL_COLOR_MATERIAL);					//objekte leuchen bunt und nicht nur in lichtfarbe

	// OpenGL Texturen aktivieren
    glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);		//textur ordentlich auf kugel auftragen
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
   	  
   	  glClearColor(0.,0.2,1.0,1.0);
	  gluLookAt(xpos, 3, zpos,0.,0.,0.,0.,1.,0.);

	  himmel();			//Skysphere
	  boden();			//lila bodenplatte
	  
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
	  glColor4f(1.0,0.5,0.0,0.5);
	  glScalef(170.0,1.0,1.0);
	  glutSolidCube(0.1);  // Stange

	  glPopMatrix();
	  
	  glutSwapBuffers();	
   glFlush(); //Buffer leeren
}

double round(double x){
	x = (int)(x*100+0.5)/100.0;			//berechnung nur von 2 nachkomma stellen
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
	   xpos -= 0.1;				//bewege Kamera 0.1 in -x Richtung
   if(GetAsyncKeyState(0x53))		//Abfragen ob 'S' gedrückt wurde
	   xpos += 0.1;				//bewege Kamera 0.1 in x Richtung
   if(GetAsyncKeyState(0x44))		//Abfragen ob 'A' gedrückt wurde
	   zpos -= 0.1;				//bewege Kamera 0.1 in -z Richtung
   if(GetAsyncKeyState(0x41))		//Abfragen ob 'D' gedrückt wurde
	   zpos += 0.1;				//bewege Kamera 0.1 in z Richtung
   if(GetAsyncKeyState(0x20)){      //Abfragen ob 'Space' gedrückt wurde
	   angle = MAX = -75;			//Neustart
	   factor = 2;
   }

   if(GetAsyncKeyState(0x6b))		// '+' beschleunigt die Rotation des Pendels
	   crate += 0.1;
   if(GetAsyncKeyState(0x6d))		// '-' verlangsamt die Rotation
	   crate -= 0.1;
}


void Animate (int value)    
{  
   // RenderScene aufrufen
   glutPostRedisplay();
   
   keyEvents();							//abfangen der Tastendrücke (asychron)

   if(!pause){
	   if(crate == 0)
		   crate = 0.4;
	   if(abs(MAX) <= 4)				//Stoppen wenn Kugeln ausgependelt sind
		   factor = angle =  0;
		angle=angle + factor * round(cos(angle * PI / 180));					//Kugeln beschleunigen negativ wenn aufwärts und positiv wenn abwärts

	if(((int)angle<=1 && (int) angle>= 0) || ((int) angle< MAX + 1)  )			// wegen Ungenauigkeit	    
	   factor=factor*-1;														//wechsel der pendelrichtung
	if(((int)angle<=1 && (int) angle>= 0)){
	   leftball = !leftball;													//wenn Kugel bei 0 Grad: andere Kugel
	   MAX *= 0.96;																//auspendeln
	}
	
   }else{
	   crate = 0;
   }
   // Timer wieder registrieren; Animate wird so nach 10 msec mit value+=1 aufgerufen
   glutTimerFunc(10, Animate, ++value);  
        
}

int main(int argc, char **argv)
{
   glutInit( &argc, argv );                // GLUT initialisieren
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitWindowSize( 600, 600 );         // Fenster-Konfiguration
   glutCreateWindow( "Newtonpendel" );   // Fenster-Erzeugung
   glutDisplayFunc( RenderScene );         // Zeichenfunktion bekannt machen
   glutReshapeFunc( Reshape );
   // TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
   glutTimerFunc(10, Animate, 0);
   Init();
   glutMainLoop();
   return 0;
}
