// CS3241Lab1.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>

/* Include header files depending on platform */
#ifdef _WIN32
	#include "glut.h"
	#define M_PI 3.14159265358
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

using namespace std;

float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;
float lineWidth = 0.2;

int border[3] = {0, 0, 0};


//Line Method
void glLine(float x1, float y1, float x2, float y2){
    glBegin(GL_LINE_STRIP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

//Triangle Method
void glTri(float x1, float y1, float x2, float y2, float x3, float y3, int MODE){
    glBegin(MODE);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

//Rectangle Method
void glRect(float leftX, float rightX, float topY, float bottomY, float MODE){
    glBegin(MODE);
    glVertex2f(leftX, bottomY);
    glVertex2f(rightX, bottomY);
    glVertex2f(rightX, topY);
    glVertex2f(leftX, topY);
    glEnd();
}

//Roud Rectangle Method
void glRoundRect(int centerX, int centerY, int width, int height, float cirR, int MODE){
    float PI_HALF = M_PI / 2;
    float divide = 80.0;
    double tx, ty;
    glBegin(MODE);
    //Four kinds of operators for Quadrant
    int opX[4]={1, -1, -1, 1};
    int opY[4]={1, 1, -1, -1};
    //Counter
    float x = 0;
    //Value added to x each loop
    float part = 1 / divide;
    int w = width / 2 - cirR;
    int h = height / 2 - cirR;
    //Loop
    for(x=0; x<5; x+=part){
        //Calculate the radiant
        double rad = PI_HALF * x;
        //Calculate the coordinate value
        tx = cirR * cos(rad) + opX[(int)x % 4] * w + centerX;
        ty = cirR * sin(rad) + opY[(int)x % 4] * h + centerY;
        glVertex2f(tx, ty);
    }
    glEnd();
}

//void glFillROundRect()

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

//Draw a rectangle with frame and color
void glRectWithFrameAndColor(float leftX, float rightX, float topY, float bottomY,int border[3], int color[3]){
    glColor3ub(border[0], border[1], border[2]);
    glRect(leftX, rightX, topY, bottomY, GL_LINE_LOOP);
    glColor3ub(color[0], color[1], color[2]);
    glRect(leftX+lineWidth/2, rightX-lineWidth/2, topY+lineWidth
           /2, bottomY-lineWidth/2, GL_POLYGON);
}

void tittle(){
    glLineWidth(5.0);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-8, 9);
    glVertex2f(-8.7, 5.5);
    glVertex2f(-6.3, 5.6);
    glEnd();
    
    glLineWidth(5.0);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-5.3, 5.5);
    glVertex2f(-4.6, 9);
    glVertex2f(-3, 5.4);
    glEnd();
    
    glLineWidth(5.0);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-5.3, 5.5);
    glVertex2f(-4.6, 9);
    glVertex2f(-3, 5.4);
    glEnd();
    //A
    glLine(-5, 7, -3.75, 7);
    glLine(-2, 5.4, 0.5, 5.5);
    //Z
    glLine(-2, 5.4, 0.6, 9);
    glLine(-1.75, 8.9, 0.6, 9);
    //Y
    glLine(2, 9, 3, 7.5);
    glLine(4.2, 9, 3, 7.5);
    glLine(3, 5, 3, 7.5);
}

void flight_Frame(){
    glLineWidth(2.0);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(4.2, -1);
    glVertex2f(4.2, 1.15);
    glVertex2f(5.3, 0.88);
    glVertex2f(5.3, -0.85);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(4.2, 1.15);
    glVertex2f(5.1, 3.88);
    glVertex2f(3.7, 3.88);
    glVertex2f(2.0, 1.15);
    glEnd();
    
    //back of plane
    glBegin(GL_LINE_STRIP);
    glVertex2f(2.0, 1.15);
    glVertex2f(-0.55, 1.15);
    glEnd();

    //belly of plane
    glBegin(GL_LINE_STRIP);
    glVertex2f(4.1, -1.15);
    glVertex2f(-4.2, -1.15);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex2f(-4.2, 1.45);
    glVertex2f(-4.2, -0.5);
    glEnd();
    
    
    //Tail of plane
    glLineWidth(2.1);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-1.5, 0);
    glVertex2f(1.5, -0.6);
    glVertex2f(0, 0.1);
    glEnd();
    
    //Oval of air pro
    glPushMatrix();
    glScalef(1, 1.4, 1);
    glTranslated(0.2, 0.4, 0);
    glRotatef(33, -4.5, -1, 0);
    glCircle(-4.5, -1, 0.3, GL_LINE_LOOP);
    glPopMatrix();
    
}

void flight_Body(){
//    glColor3f(0.6, 0.7, 0.8);
    glColor3f(0.9, 0.8, 0.5);
    glRoundRect(0, 0, 10, 4.0, 0.2, GL_POLYGON);
   
    glPushMatrix();
    glColor3f(0.5, 0.4, 0.4);
    glBegin(GL_POLYGON);
    glVertex2f(4.2, -1);
    glVertex2f(4.2, 1.15);
    glVertex2f(5.3, 0.88);
    glVertex2f(5.3, -0.85);
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
//    glColor3f(0.6, 0.7, 0.8);
    glColor3f(0.9, 0.8, 0.5);
    glBegin(GL_POLYGON);
    glVertex2f(4.2, 1.15);
    glVertex2f(5.1, 3.88);
    glVertex2f(3.7, 3.88);
    glVertex2f(2.0, 1.15);
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.5, 0.4, 0.4);
    glScalef(1, 1.4, 1);
    glTranslated(0.2, 0.4, 0);
    glRotatef(33, -4.5, -1, 0);
    glCircle(-4.5, -1, 0.3, GL_POLYGON);
    glPopMatrix();
    glRoundRect(-3, 2.5, 3.5, 1, 0.15, GL_POLYGON);
    
    glPushMatrix();
    glColor3f(1, 1, 1);
    glRoundRect(7.5, 0, 4, 2, 0.2, GL_POLYGON);
    glPushMatrix();
    glRotated(8, 0, 0, 1);
    glRoundRect(7.5, 0, 4, 2, 0.2, GL_POLYGON);
    glPopMatrix();
    glPushMatrix();
    glRotated(-8, 0, 0, 1);
    glRoundRect(7.5, 0, 4, 2, 0.2, GL_POLYGON);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
    

    
}

// Bazier
GLfloat ctrlpoints[4][3] = {
    { -6, 1.2, 0 }, { 0, 3.2, 0 }, { 1, 3.2, 0}, { 2, 1.2, 0 }
};

GLfloat ctrlpoints_2[4][3] = {
    { -6, 1.2, 0 }, { -4, -2, 0 }, { -1, -2, 0}, { -2, 1.2, 0 }
};

GLfloat ctrlpoints_3[4][3] = {
    { -6, 1.2, 0 }, { -7, 5, 0 }, { -4, 5, 0}, { -2, 1.2, 0 }
};

GLfloat ctrlpoints_4[4][3] = {
    { -6, 1.2, 0 }, { -4, -0.2, 0 }, { -3, -0.2, 0}, { -1.65, 1.2, 0 }
};

void display_B(void)
{
    int i;
    glLoadIdentity();
//    glColor3f(0.6, 0.7, 0.8);
//    glColor3f(0.9, 0.8, 0);
    glColor3f(0.9, 0.8, 0.5);
    
    //Head
    glPushMatrix();
    glPointSize(5.0);
    glTranslated(-2.5, 0, 0);
    
    //Upper Head
    glBegin(GL_POLYGON);
    for (i = 0; i <= 40; i++)
        glEvalCoord1f((GLfloat)i / 40);   //Create four control points
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i <= 40; i++)
        glEvalCoord1f((GLfloat)i / 40);   //Create four control points
    glEnd();
    
    //Lower Head
//    glColor3f(0.8, 0.5, 0);
//    glColor3f(0.7, 0.5, 0.4);
    glColor3f(0.9, 0.8, 0.4);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints_4[0][0]);
    glBegin(GL_POLYGON);
    for (i = 0; i <= 40; i++)
        glEvalCoord1f((GLfloat)i / 40);   //Create four control points
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i <= 40; i++)
        glEvalCoord1f((GLfloat)i / 40);   //Create four control points
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINE_STRIP);
    glVertex2d(-5.9, 1.15);
    glVertex2d(-1.7, 1.15);
    glEnd();
    glPopMatrix();

    //Eye
    glPushMatrix();
    glColor3f(1, 1, 1);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints_2[0][0]);
    glTranslated(-1.8, 1.6, 0);
    glScalef(0.23, 0.23, 1);
    glBegin(GL_POLYGON);
    for (i = 0; i <= 40; i++)
        glEvalCoord1f((GLfloat)i / 40);   //根据4个控制点坐标的参数化插值
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i <= 40; i++)
        glEvalCoord1f((GLfloat)i / 40);   //根据4个控制点坐标的参数化插值
    glEnd();
    
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints_3[0][0]);
    glBegin(GL_POLYGON);
    for (i = 0; i <= 40; i++)
        glEvalCoord1f((GLfloat)i / 40);   //根据4个控制点坐标的参数化插值
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    for (i = 0; i <= 40; i++)
        glEvalCoord1f((GLfloat)i / 40);   //根据4个控制点坐标的参数化插值
    glEnd();
    glPopMatrix();
    
    glColor3f(0, 0, 0);
    glCircle(-2.73, 1.88, 0.18, GL_POLYGON);

}

void apple(){
    glPushMatrix();
    glTranslated(3.9, 2.8, 0);
    glScalef(0.13, 0.13, 1);
    glBegin(GL_POLYGON);
    int n = 40;
    float r = 2.0;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            glColor3ub(168, 216, 185);
            float u1 = j * 2 * M_PI / n;
            float u2 = (j + 1) * 2 * M_PI / n;
            float v = i * 2 * M_PI / n;
            float x1 = r * (cos(v + M_PI / 2) + 2 * sin(v / 2) * cos(u1));
            float y1 = r * (sin(v + M_PI / 2) + 2 * sin(v / 2) * sin(u1));
            float x2 = r * (cos(v + M_PI / 2) + 2 * sin(v / 2) * cos(u2));
            float y2 = r * (sin(v + M_PI / 2) + 2 * sin(v / 2) * sin(u2));
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        }
    }
    glEnd();
    glPopMatrix();
}

void apple_2(){
    glPushMatrix();
    glTranslated(-0.88, 7.3, 0);
    glScalef(0.13, 0.13, 1);
    glBegin(GL_POLYGON);
    int n = 40;
    float r = 2.0;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            glColor3f(0, 0, 0);
            float u1 = j * 2 * M_PI / n;
            float u2 = (j + 1) * 2 * M_PI / n;
            float v = i * 2 * M_PI / n;
            float x1 = r * (cos(v + M_PI / 2) + 2 * sin(v / 2) * cos(u1));
            float y1 = r * (sin(v + M_PI / 2) + 2 * sin(v / 2) * sin(u1));
            float x2 = r * (cos(v + M_PI / 2) + 2 * sin(v / 2) * cos(u2));
            float y2 = r * (sin(v + M_PI / 2) + 2 * sin(v / 2) * sin(u2));
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        }
    }
    glEnd();
    glPopMatrix();
}

GLfloat x1 = 4;
GLfloat x2 = 5;
GLfloat interval = 0.3;
void missile(){

    glRect(x1, x2, -4, -6, GL_POLYGON);
}

void TimerFunciton(int value){
    x1 = x1 - interval;
    x2 = x2 - interval;
    glutPostRedisplay();
    glutTimerFunc(2000, TimerFunciton, 1);
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);	
	glTranslatef(tx, ty, 0);	
	glRotatef(alpha, 0, 0, 1);
	
	//draw your stuff here
    tittle();
    flight_Body();
    flight_Frame();
    glPushMatrix();
//    glColor3f(0.6, 0.7, 0.8);
    glColor3f(0.9, 0.8, 0.5);
    glBegin(GL_POLYGON);
    glVertex2f(4, 0);
    glVertex2f(3, 0);
    glVertex2f(4.8, -0.6);
    glEnd();
    glColor3f(0, 0, 0);
    glLineWidth(2.2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(4, 0);
    glVertex2f(3, 0);
    glVertex2f(4.8, -0.6);
    glEnd();
    glPopMatrix();
    display_B();
    apple();
    apple_2();
//    glColor3f(0.6, 0.7, 0.8);
    glColor3f(0.9, 0.8, 0.5);
    glRect(-0.2, 0.2, 0.2, -0.2, GL_POLYGON);
    missile();
    

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.8f,0.0f,0.0f);
    glVertex3f(0.6f,0.0f,0.0f);
    glVertex3f(0.7f,0.2f,0.0f);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.1f,0.0f,0.0f);
    glVertex3f(0.1f,0.0f,0.0f);
    glVertex3f(0.0f,0.2f,0.0f);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.6f,0.0f,0.0f);
    glVertex3f(0.8f,0.0f,0.0f);
    glVertex3f(0.7f,0.2f,0.0f);
    glEnd();
    
    glutSwapBuffers();
    
	glPopMatrix();
	glFlush ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor (0.7, 0.9, 1.0, 1);
	glShadeModel (GL_SMOOTH);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
//    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints_2[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
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
			
		case 27:
			exit(0);
		break;

		default:
		break;
	}
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 1\n\n";
	cout<<"+++++CONTROL BUTTONS+++++++\n\n";
	cout<<"Scale Up/Down: Q/E\n";
	cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
	cout<<"Move Up/Down: W/S\n";
	cout<<"Move Left/Right: Z/C\n";
	cout <<"ESC: Quit\n";

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
//    glutMouseFunc(mouse);
    
//    glutTimerFunc(20, TimerFunciton, 1);
//    glutIdleFunc(missile);
	glutKeyboardFunc(keyboard);
    glutMainLoop();

	return 0;
}
