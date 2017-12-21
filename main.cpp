//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#include <stdlib.h>
#include <GL/glut.h>
//#include "GL/glut.h"
#include "vec2.h"
//#pragma comment(lib, "glut32.lib")

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <stdarg.h>

using namespace std;

#define WINDOW_WIDTH  800	
#define WINDOW_HEIGHT 600
#define MAX_RADIUS 100
#define MIN_RADIUS 40

#define M_PI           3.14159265358979323846

#define DEGREE2RAD(x) x * M_PI / 180.0

//#define  TIMER_PERIOD  180 // Period for the timer.
#define  TIMER_PERIOD  20 // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532

//lights to activate
bool blight[3];
float lightColor[3][3] = { { 1, 0, 0 }, { 0, 1, 0 }, {0, 0, 1} };

// light circles
vec2_t lightPosition[3];

// vector
vec2_t lightVector[3];

float fincrement[3];
const float INCREMENT[3] = { 0.04f, 0.01f, 0.025f };

struct
{
	float radius;
	vec2_t center;
}circles[3];

//
int randomValue(int low, int high)
{
	return (int) (low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (float)(high - low))) + 0.5f);
}

//
// to draw circle, center at (x,y)
//  radius r and color red, green, blue
//
void circle(unsigned char red, unsigned char green, unsigned char blue, int x, int y, int r )
{
//#define PI 3.1415
	float angle ;

	glColor3ub(red, green, blue);
	/*
	if (r < 60) {
		glPointSize(r * 2);
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
		glPointSize(1);
		return;
	}
	*/
	glDisable(GL_POLYGON_SMOOTH);
	glBegin( GL_POLYGON ) ;
	for ( int i = 0 ; i < 100 ; i++ )
	{
		angle = 2.f*M_PI*i/100.f;
		glVertex2f( x+r*cos(angle), y+r*sin(angle)) ;
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2.f * M_PI*i / 100.f;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font )
{
	int len, i ;

	glRasterPos2f( x, y );
	len = (int) strlen( string );
	for ( i =0; i<len; i++ )
	{
		glutBitmapCharacter( font, string[i]);
	}
}

void vprint2(int x, int y, float size, char *string) {
	va_list ap;
	//va_start(ap, string);
	char str[1024];
	//vsprintf_s(str, string, ap);
	sprintf(str, "%s", string);
	//va_end(ap);
	glPushMatrix();
		glTranslatef(x, y, 0);
		glScalef(size, size, 1);
		
		int len, i;
		len = (int)strlen(str);
		for (i = 0; i<len; i++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
			//glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
		}
	glPopMatrix();
}

//info square at bottom
void infoSquare()
{
	int height = -(WINDOW_HEIGHT/2) + 25;
	// info square
	glColor4ub(45, 45, 45, 255);
	glBegin(GL_QUADS);
	glVertex2i(-(WINDOW_WIDTH / 2), height);
	glVertex2i(+(WINDOW_WIDTH / 2), height);
	glVertex2i(+(WINDOW_WIDTH / 2), height - 25);
	glVertex2i(-(WINDOW_WIDTH / 2), height - 25);
	glEnd();
	//print message
	char buf[1024];
	sprintf(buf, "F1 (Red): %s   F2 (Green): %s    F3 (Blue): %s", blight[0] ? "ON" : "OFF", blight[1] ? "ON" : "OFF", blight[2] ? "ON" : "OFF");
	glColor4ub(255, 255, 255, 255);
	print(-(WINDOW_WIDTH / 2) + 20, height - 15, buf, GLUT_BITMAP_HELVETICA_12);
}

#define CLAMP(x) (x < 0)?0:(x > 1)? 1: x

// k is the circle index (0 to 2)
void getColor(vec2_t point, int k, float & red, float & green, float & blue)
{
	vec2_t N = unit(sub(point, circles[k].center));
	vec2_t L1 = sub(lightPosition[0], circles[k].center);
	vec2_t L2 = sub(lightPosition[1], circles[k].center);
	vec2_t L3 = sub(lightPosition[2], circles[k].center);
	
	float attenuation1 = 1.0 / (1.0 + (mag(L1)*mag(L1)));
	float attenuation2 = 1.0 / (1.0 + (mag(L2)*mag(L2)));
	float attenuation3 = 1.0 / (1.0 + (mag(L3)*mag(L3)));

	L1 = unit(L1);
	L2 = unit(L2);
	L3 = unit(L3);

	//float diffuse1 = attenuation1*dot(N, L1);
	//float diffuse2 = attenuation2*dot(N, L2);
	//float diffuse3 = attenuation3*dot(N, L3);

	float diffuse1 = dot(N, L1);
	float diffuse2 = dot(N, L2);
	float diffuse3 = dot(N, L3);


	if (blight[0])
		red = CLAMP(diffuse1);
	if (blight[1])
		green = CLAMP(diffuse2);
	if (blight[2])
		blue = CLAMP(diffuse3);
}



void checkCollide()
{
	int limitX = WINDOW_WIDTH/2 - 5;
	int limitY = WINDOW_HEIGHT/2 - 5;
	for (int k = 0; k < 3; k++)
	{
		if (lightPosition[k].x > limitX)	//right side
		{
			lightVector[k] = direction(DEGREE2RAD(randomValue(100, 260)));	//90 and 270
		}
		else if (lightPosition[k].x < -limitX)	//left side
		{
			lightVector[k] = direction(DEGREE2RAD(randomValue(280, 440)));	//270 to 450 (360 + 90)
		}
		else if (lightPosition[k].y > limitY)	//bottom side
		{
			lightVector[k] = direction(DEGREE2RAD(randomValue(270, 350)));	//180 to 360
		}
		else if (lightPosition[k].y < -limitY)	//bottom side
		{
			lightVector[k] = direction(DEGREE2RAD(randomValue(10, 80)));	//0 to 90
		}
	}
}

//
// To display onto window using OpenGL commands
//
void display()
{
	//
	// clear window to black
	//
	glClearColor( 0, 0 , 0 , 0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glLoadIdentity();
	
	infoSquare();
	
	//light's points
	
	glPointSize(10);
	glBegin(GL_POINTS);
	for (int k = 0; k < 3; k++)
	{
		if (blight[k])
		{
			glColor3fv(lightColor[k]);
			glVertex2f(lightPosition[k].x, lightPosition[k].y);
			lightPosition[k] = add(lightPosition[k], mul(0.1, lightVector[k]));
		}
	}
	glEnd();
	glPointSize(1);

	checkCollide();
	
	float r, g, b;
	vec2_t temp;
	for (int k = 0; k < 3; k++)
	{
		glBegin(GL_TRIANGLE_FAN);
			glColor3f(0, 0, 0);
			glVertex2f(circles[k].center.x, circles[k].center.y);
			for (int i = -1; i < 100; i++)
			{
				float angle = 2 * M_PI*i / 100;
				temp.x = circles[k].center.x + circles[k].radius*cos(angle);
				temp.y = circles[k].center.y + circles[k].radius*sin(angle);
				getColor(temp, k, r, g, b);
				glColor3f(r, g, b);
				glVertex2f(temp.x, temp.y);
			}
		glEnd();	
		
		//dynamic
		circles[k].radius += fincrement[k];
		if (circles[k].radius > MAX_RADIUS)
			fincrement[k] = -INCREMENT[k];
		else if (circles[k].radius < MIN_RADIUS)
			fincrement[k] = +INCREMENT[k];
	}

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
   if ( key == 27 )
	   exit(0);
    // to refresh the window it calls display() function
   glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
   if ( key == 27 )
	   exit(0);
    
    // to refresh the window it calls display() function
   glutPostRedisplay() ;
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown( int key, int x, int y )
{
	// Write your codes here.
	 // to refresh the window it calls display() function
	switch (key)
	{
	case GLUT_KEY_F1:
		blight[0] = !blight[0];
			break;
	case GLUT_KEY_F2:
		blight[1] = !blight[1];
		break;
	case GLUT_KEY_F3:
		blight[2] = !blight[2];
		break;
	}
	glutPostRedisplay();
}


//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp( int key, int x, int y )
{
  	// Write your codes here.
	 // to refresh the window it calls display() function
	glutPostRedisplay() ;
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick( int button, int stat, int x, int y )
{
	 // to refresh the window it calls display() function
	glutPostRedisplay() ; 
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize( int w, int h )
{
	glViewport( 0,0,w,h) ;
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -w/2, w/2, -h/2, h/2, -1, 1);
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();

}

void onMoveDown( int x, int y ) {
	 // to refresh the window it calls display() function	
	glutPostRedisplay() ;
}

void onMove( int x, int y ) {
	 // to refresh the window it calls display() function
    glutPostRedisplay() ;
}

#if TIMER_ON == 1
void onTimer( int v ) {
	 
	glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;

	 // to refresh the window it calls display() function
	glutPostRedisplay() ; // display()

}
#endif

void Init() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	
	//lights
	blight[0] = blight[1] = blight[2] = true; 

	lightPosition[0].x = 0;
	lightPosition[0].y = (WINDOW_HEIGHT / 2) - 5;
	lightVector[0] = direction(DEGREE2RAD(randomValue(190, 350)));	// between 180 and 360

	lightPosition[1].x = -(WINDOW_WIDTH / 2) + 5;
	lightPosition[1].y = 0;
	lightVector[1] = direction(DEGREE2RAD(randomValue(190, 440)));	// 280 to 450

	lightPosition[2].x = 0;
	lightPosition[2].y = -(WINDOW_HEIGHT / 2) + 5;
	lightVector[2] = direction(DEGREE2RAD(randomValue(190, 350)));	// between 180 and 360
	
	//circles
	circles[0].center.x = 0;
	circles[0].center.y = -100;
	circles[0].radius = 50;

	circles[1].center.x = -200;
	circles[1].center.y = +100;
	circles[1].radius = 50;

	circles[2].center.x = +200;
	circles[2].center.y = +100;
	circles[2].radius = 50;

	//dynamic size
	fincrement[0] = INCREMENT[0];
	fincrement[1] = INCREMENT[1];
	fincrement[2] = INCREMENT[2];
}

int main( int argc, char *argv[] )
{
	glutInit(&argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow( "Template File" ) ;

	glutDisplayFunc( display ) ;
	glutIdleFunc(display);
    glutReshapeFunc( onResize );
	//
	// keyboard registration
	//
	glutKeyboardFunc( onKeyDown ) ;
	glutSpecialFunc( onSpecialKeyDown ) ;

	glutKeyboardUpFunc( onKeyUp ) ;
	glutSpecialUpFunc( onSpecialKeyUp ) ;

	//
	// mouse registration
	//
    glutMouseFunc( onClick) ;
	glutMotionFunc( onMoveDown ) ;
	glutPassiveMotionFunc( onMove ) ; 
	
    #if  TIMER_ON == 1
	// timer event
	glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
	#endif

	Init();
	
	glutMainLoop();
	return 0;
}
