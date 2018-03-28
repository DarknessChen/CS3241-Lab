// CS3241Lab2.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>
#include <time.h>

/* Include header files depending on platform */

// populate stars fade and glow
// complex orbits, elliptical or off-center orbits
// smooth orbit?
#ifdef _WIN32
	#include "glut.h"
	#define M_PI 3.14159
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

using namespace std;

#define numStars 100
#define numPlanets 9

class planet
{
public:
	float distFromRef;
	float angularSpeed;
	GLfloat color[3];
	float size;
	float angle;

	planet()
	{
		distFromRef = 0;
		angularSpeed = 0;
		color[0] = color[1] = color[2] = 0;
		size = 0;
		angle = 0;
	}
};

float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;
planet planetList[numPlanets];
int flag = 0;
int flag_mode = 0;
int i = 0.01;
int pi = 3.141592653;

int Xtimes = 400000;

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-20, 20, -20, 20, -20, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor (0.0, 0.0, 0.3, 1.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Arc Method
void glArc(double x, double y, double start_angle, double end_angle, double  radius, int MODE){
    glBegin(MODE);
    double delta_angle = M_PI / 180;
    for(double i = start_angle; i <= end_angle; i+=delta_angle){
        double vx = x + radius * cos(i);
        double vy = y + radius * sin(i);
        glVertex2f(vx, vy);
    }
    glEnd();
}

//Circle Method
void glCircle(double x, double y, double radius, int MODE){
    glArc(x, y, 0, 2 * M_PI, radius, MODE);
}

void output(int x, int y, float r, float g, float b, char *string)
{
    glColor3f( r, g, b );
    glRasterPos2f(x, y);
    int len, i;
    len = (int)strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
    }
}

void drawSun(){
    glPushMatrix();
    glColor3f(1, 0.7, 0.5);
    glCircle(0, 0, 2.5, GL_POLYGON);
    glPopMatrix();
}
void drawPlanet(planet p){
    glPushMatrix();
    glColor3f(p.color[0], p.color[1], p.color[2]);
    glCircle(0, 0, p.size, GL_POLYGON);
    glPopMatrix();
}

int random(int m){
    return rand()%m;
}

void drawStar(){
    glColor3f(1, 1, 1);
    glPointSize(2.5);
    glBegin(GL_POINTS);
    for(int i=0; i<=7; i++){
        glVertex2i(-20+random(40), -20+random(40));
    }
    glEnd();
}

void draw(){
    clock_t tick = clock();
    
    glPushMatrix();
    glTranslatef(-2.6, 0, 0);
    float speedText = 360 - (float)tick/Xtimes * 360;
    float distFromRef = 0.2;
    glRotated(speedText, 0, 0, 1);
    glTranslatef(distFromRef, distFromRef, 0);
    output(-1.5, 0, 1, 1, 0.8, "THIS IS UNIVERSE!");
    glPopMatrix();
    
    //Planet 1, 2, 3
    glPushMatrix();
    planet planet_1;
    planet_1.distFromRef = 5;
    planet_1.angularSpeed = 360 - (float)tick/Xtimes * 360;
    glRotated(planet_1.angularSpeed, 0, 0, 1);
    glTranslatef(0, planet_1.distFromRef, 0);
    planet_1.color[0] = 0.5;
    planet_1.color[1] = 1;
    planet_1.color[2] = 0.8;
    planet_1.size = 1;
    drawPlanet(planet_1);
    
    glPushMatrix();
    planet planet_2;
    planet_2.distFromRef = 1.5;
    planet_2.angularSpeed = 360 - (float)tick/Xtimes * 1000;
    glRotated(planet_2.angularSpeed, 0, 0, 1);
    glTranslatef(0, planet_2.distFromRef, 0);
    planet_2.color[0] = 0.6;
    planet_2.color[1] = 0;
    planet_2.color[2] = 0.1;
    planet_2.size = 0.2;
    drawPlanet(planet_2);
    glPopMatrix();
    
    glPushMatrix();
    planet planet_3;
    planet_3.distFromRef = 2.2;
    planet_3.angularSpeed = 360 - (float)tick/Xtimes * 200;
    glRotated(planet_3.angularSpeed, 0, 0, 1);
    glTranslatef(0, planet_3.distFromRef, 0);
    planet_3.color[0] = 1;
    planet_3.color[1] = 0.5;
    planet_3.color[2] = 0.6;
    planet_3.size = 0.25;
    drawPlanet(planet_3);
    glPopMatrix();
    glPopMatrix();
    
    //Planet 4, 5, 6
    glPushMatrix();
    planet planet_4;
    planet_4.distFromRef = 9.5;
    planet_4.angularSpeed = 360 - (float)tick/Xtimes * 600;
    glRotated(planet_4.angularSpeed, 0, 0, 1);
    glTranslatef(planet_4.distFromRef, 0, 0);
    planet_4.color[0] = 1;
    planet_4.color[1] = 0.7;
    planet_4.color[2] = 0.9;
    planet_4.size = 1.4;
    drawPlanet(planet_4);
    
    glPushMatrix();
    planet planet_5;
    planet_5.distFromRef = 2.1;
    planet_5.angularSpeed = 360 - (float)tick/Xtimes * 1000;
    glRotated(planet_5.angularSpeed, 0, 0, 1);
    glTranslatef(planet_5.distFromRef, 0, 0);
    planet_5.color[0] = 1;
    planet_5.color[1] = 0.9;
    planet_5.color[2] = 0.6;
    planet_5.size = 0.4;
    drawPlanet(planet_5);
    glPopMatrix();

    glPushMatrix();
    planet planet_6;
    planet_6.distFromRef = 2.85;
    planet_6.angularSpeed = 360 - (float)tick/Xtimes * 200;
    glRotated(planet_6.angularSpeed, 0, 0, 1);
    glTranslatef(planet_6.distFromRef, 0, 0);
    planet_6.color[0] = 0.9;
    planet_6.color[1] = 1;
    planet_6.color[2] = 0.8;
    planet_6.size = 0.25;
    drawPlanet(planet_6);
    glPopMatrix();
    glPopMatrix();
    
    //Planet 7
    glPushMatrix();
    planet planet_7;
    planet_7.distFromRef = 15;
    planet_7.angularSpeed = 360 - (float)tick/Xtimes * 400;
    glRotated(planet_7.angularSpeed, 0, 0, 1);
    glTranslatef(planet_7.distFromRef, 0, 0);
    planet_7.color[0] = 0;
    planet_7.color[1] = 0.7;
    planet_7.color[2] = 1;
    planet_7.size = 2;
    drawPlanet(planet_7);
    glPushMatrix();
    planet planet_7_1;
    planet_7_1.distFromRef = 1;
    planet_7_1.angularSpeed = 360 - (float)tick/Xtimes * 4;
    planet_7_1.angle += planet_7_1.angularSpeed;
    glRotated(planet_7.angularSpeed, 0, 0, 1);
    glTranslatef(0, 5+2.0*cos(2*pi*planet_7_1.angle), 0);
    planet_7_1.color[0] = 0.7;
    planet_7_1.color[1] = 0.6;
    planet_7_1.color[2] = 0;
    planet_7_1.size = 1;
    drawPlanet(planet_7_1);
    glPopMatrix();
    glPopMatrix();
    
    
    //Planet 8
    glPushMatrix();
    planet planet_8;
    planet_8.distFromRef = 15;
    planet_8.angularSpeed = 360 - (float)tick/Xtimes * 15;
    planet_8.angle += planet_8.angularSpeed;
//    glRotated(planet_8.angularSpeed, 0, 0, 1);
    glTranslatef(1.5*planet_8.distFromRef*cos(planet_8.angle), 0.45*planet_8.distFromRef*sin(planet_8.angle), 0);
//    glScalef(0.6, 1.3, 1);
    planet_8.color[0] = 1;
    planet_8.color[1] = 1;
    planet_8.color[2] = 0.3;
    planet_8.size = 0.8;
    drawPlanet(planet_8);
    glPopMatrix();
    
    //Planet 9
    glPushMatrix();
    planet planet_9;
    planet_9.distFromRef = 15;
    planet_9.angularSpeed = 360 - (float)tick/Xtimes * 25;
    planet_9.angle += planet_9.angularSpeed;
    //    glRotated(planet_8.angularSpeed, 0, 0, 1);
    glTranslatef(0.4*planet_9.distFromRef*cos(planet_9.angle), 1.3*planet_9.distFromRef*sin(planet_9.angle), 0);
    //    glScalef(0.6, 1.3, 1);
    planet_9.color[0] = 0.7;
    planet_9.color[1] = 0.8;
    planet_9.color[2] = 0.9;
    planet_9.size = 0.4;
    drawPlanet(planet_9);
    glPopMatrix();
}

void timerMode(){
    
    time_t current_time = time (NULL);
    struct tm * timeinfo = localtime(&current_time);
    printf("Now time is:%s", asctime(timeinfo));
    //Hour
    glPushMatrix();
    planet planet_1;
    planet_1.distFromRef = 5.7;
    planet_1.angularSpeed = 360 - ((float)timeinfo->tm_hour/12) * 360;
    //    glTranslatef(planet_1.distFromRef, planet_1.distFromRef, 0);
    glRotated(planet_1.angularSpeed, 0, 0, 1);
    glTranslatef(0, planet_1.distFromRef, 0);
    planet_1.color[0] = 0.5;
    planet_1.color[1] = 1;
    planet_1.color[2] = 0.8;
    planet_1.size = 1.5;
    drawPlanet(planet_1);
    glPopMatrix();
    //Min
    glPushMatrix();
    planet planet_4;
    planet_4.distFromRef = 8.8;
    planet_4.angularSpeed = 360 - (float)timeinfo->tm_min/60*360;
    //    glTranslatef(planet_1.distFromRef, planet_1.distFromRef, 0);
    glRotated(planet_4.angularSpeed, 0, 0, 1);
    glTranslatef(0, planet_4.distFromRef, 0);
    planet_4.color[0] = 1;
    planet_4.color[1] = 0.7;
    planet_4.color[2] = 0.9;
    planet_4.size = 1.1;
    drawPlanet(planet_4);
    glPopMatrix();
    //Sec
    glPushMatrix();
    planet planet_7;
    planet_7.distFromRef = 15;
    planet_7.angularSpeed = 360 - (float)timeinfo->tm_sec/60*360;
    //    glTranslatef(planet_1.distFromRef, planet_1.distFromRef, 0);
    glRotated(planet_7.angularSpeed, 0, 0, 1);
    glTranslatef(0, planet_7.distFromRef, 0);
    planet_7.color[0] = 0;
    planet_7.color[1] = 0.7;
    planet_7.color[2] = 1;
    planet_7.size = 1.1;
    drawPlanet(planet_7);
    glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//controls transformation
	glScalef(k, k, k);	
	glTranslatef(tx, ty, 0);	
	glRotatef(alpha, 0, 0, 1);
	//draw stuff here!
    drawSun();
    drawStar();
    if(flag_mode == 0){
        draw();
    }else{
        timerMode();
    }
	glPopMatrix();
	glFlush ();
}




void idle()
{
	//update animation here
	glutPostRedisplay();	//after updating, draw the screen again
}

void keyboard (unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {

		case 'a':
			alpha+=10;
			glutPostRedisplay();
		break;

		case 'd':
			alpha-=10;
			glutPostRedisplay();
		break;

		case 'q':
			k+=0.1;
			glutPostRedisplay();
		break;

		case 'e':
			if(k>0.1)
				k-=0.1;
			glutPostRedisplay();
		break;

		case 'z':
			tx-=0.1;
			glutPostRedisplay();
		break;

		case 'c':
			tx+=0.1;
			glutPostRedisplay();
		break;

		case 's':
			ty-=0.1;
			glutPostRedisplay();
		break;

		case 'w':
			ty+=0.1;
			glutPostRedisplay();
		break;
		
        case 'm':
            if(flag == 0){
                flag_mode += 1;
                flag += 1;
            }else{
                flag_mode -= 1;
                flag -= 1;
            }
            glutPostRedisplay();
            break;
            
		case 27:
			exit(0);
		break;

		default:
		break;
	}
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 2\n\n";
	cout<<"+++++CONTROL BUTTONS+++++++\n\n";
	cout<<"Scale Up/Down: Q/E\n";
	cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
	cout<<"Move Up/Down: W/S\n";
	cout<<"Move Left/Right: Z/C\n";
	cout <<"ESC: Quit\n";

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (700, 700);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);	
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
