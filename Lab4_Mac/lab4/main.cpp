// CS3241Lab1.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>
#include <vector>

/* header files for xcode */
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>

/* header files for lab machine */
//#include "GL\glut.h"

#define MAXPTNO 1000
#define NLINESEGMENT 256
#define NOBJECTONCURVE 8
#define JOINTPOINTNUM 4

using namespace std;

// Global variables that you can use
struct Point{
	int x,y;
};

// Storage of control points
int nPt = 0;
Point ptList[MAXPTNO];
Point ptFirstPointsList[MAXPTNO];

// Display options
bool displayControlPoints = true;
bool displayControlLines = true;
bool displayTangentVectors = false;
bool displayObjects = false;
bool C1Continuity = false;

Point newSecondPoint(Point p4, Point p3){
    Point result = {0, 0};
    result.x = 2 * p4.x - p3.x;
    result.y = 2 * p4.y - p3.y;
    return result;
}

float bezierCoe(int i, float t, int n){
    float coef = 1.0;
    if (i > 0){
        for (int b = 1; b <= i; b++){
            coef *= ((float)(n - (i - b)) / b);
        }
    }
    return coef * pow(1 - t, n - i) * pow(t, i);
}

void drawRightArrow(){
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0,0);
    glVertex2f(100,0);
    glVertex2f(95,5);
    glVertex2f(100,0);
    glVertex2f(95,-5);
    glEnd();
}

void drawTangentArrow(vector<Point> ctrlPoints, float i, int n) {
    float dx = 0, dy = 0, rotateAngle = 0;
    Point targetPoint = {0, 0};
    for(int j = 0; j < n - 1 ; j++) {
        float coeff = bezierCoe(j, i, n - 2);
        dx += coeff * (n - 1) * (ctrlPoints.at(j + 1).x - ctrlPoints.at(j).x);
        dy += coeff * (n - 1) * (ctrlPoints.at(j + 1).y - ctrlPoints.at(j).y);
    }
    for(int j = 0; j < n ; j++) {
        float coe = bezierCoe(j, i, n - 1);
        Point vertex = ctrlPoints.at(j);
        targetPoint.x += coe * vertex.x;
        targetPoint.y += coe * vertex.y;
    }
    rotateAngle = atan2(dy, dx);
    glPushMatrix();
    glTranslatef(targetPoint.x, targetPoint.y, 0);
    glRotatef(rotateAngle*180/M_PI, 0, 0, 1);
    drawRightArrow();
    glPopMatrix();
}

void drawPointsInCurve(vector<Point> contralPoints){
    unsigned long n = contralPoints.size();
    glColor3f(1.0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (float i = 0.0; i <= 1.0; i += (1/(float)NLINESEGMENT)){
        Point bezierPoint = {0, 0};
        for(int j = 0; j < n ; j++){
            float coeff = bezierCoe(j, i, (int)(n - 1));
            Point curr = contralPoints.at(j);
            bezierPoint.x += coeff * curr.x;
            bezierPoint.y += coeff * curr.y;
        }
        glVertex2d(bezierPoint.x, bezierPoint.y);
    }
    glEnd();
    
    if(displayTangentVectors){
        for (float i = 0.0; i <= 1.0; i += (1/(float)NOBJECTONCURVE)){
            drawTangentArrow(contralPoints, i, (int)n);
        }
    }
}

void drawBezierCurve(){
    vector<Point> contralPoints;
    for(int i = 0 ; i < nPt; i++){
        contralPoints.push_back(ptList[i]);
        if(contralPoints.size() == JOINTPOINTNUM){
            printf("Pt %d Drawing with %lu points\n", i, contralPoints.size());
            drawPointsInCurve(contralPoints);
            contralPoints.clear();
            contralPoints.push_back(ptList[i]);    // Put the first point of the next current curve into the ptList
        }
    }
    glPointSize(1);
}


void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
    vector<Point> bezierCurve;
	if(displayControlPoints){
		glPointSize(5);
		glBegin(GL_POINTS);
		for(int i=0; i<nPt; i++){
            bezierCurve.push_back(ptList[i]);
            glColor3f(0,0,0);
            if (C1Continuity && ((i+1) > 4) && bezierCurve.size() == 2){
                glColor3f(0.86, 0.86, 0.86);
                glVertex2d(ptList[i].x,ptList[i].y);
//                ptFirstPointsList[i] = ptList[i];
                glColor3f(1.0, 0, 0);   // Red
                ptList[i] = newSecondPoint(ptList[i - 1], ptList[i - 2]);
            }
            if((!C1Continuity) && ((i+1) > 4) && bezierCurve.size() == 2){
                ptList[i] = ptFirstPointsList[i];
            }
            if (bezierCurve.size() == JOINTPOINTNUM){
                bezierCurve.clear();
                bezierCurve.push_back(ptList[i]);
            }
            glVertex2d(ptList[i].x,ptList[i].y);
		}
		glEnd();
		glPointSize(1);
	}
    
	if(displayControlLines){
        glBegin(GL_LINE_STRIP);
        for(int i=0;i<nPt; i++){
            glColor3f(0,1,0);
            glVertex2d(ptList[i].x,ptList[i].y);
        }
        glEnd();
        drawBezierCurve();
	}
	glPopMatrix();
	glutSwapBuffers ();
}

void reshape (int w, int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void){
	glClearColor (1.0,1.0,1.0, 1.0);
}

void readFile(){
    
	std::ifstream file;
    file.open("savefile.txt");
	file >> nPt;
    
	if(nPt>MAXPTNO){
		cout << "Error: File contains more than the maximum number of points." << endl;
		nPt = MAXPTNO;
	}
    
	for(int i=0;i<nPt;i++){
		file >> ptList[i].x;
		file >> ptList[i].y;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void writeFile(){
	std::ofstream file;
    file.open("savefile.txt");
    file << nPt << endl;
    
	for(int i=0;i<nPt;i++){
		file << ptList[i].x << " ";
		file << ptList[i].y << endl;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void keyboard (unsigned char key, int x, int y){
	switch (key) {
		case 'r':
		case 'R':
			readFile();
            break;
            
		case 'w':
		case 'W':
			writeFile();
            break;
            
		case 'T':
		case 't':
			displayTangentVectors = !displayTangentVectors;
            break;
            
		case 'o':
		case 'O':
			displayObjects = !displayObjects;
            break;
            
		case 'p':
		case 'P':
			displayControlPoints = !displayControlPoints;
            break;
            
		case 'L':
		case 'l':
			displayControlLines = !displayControlLines;
            break;
            
		case 'C':
		case 'c':
			C1Continuity = !C1Continuity;
            break;
            
		case 'e':
		case 'E':
            nPt = 0;
            break;
            
		case 'Q':
		case 'q':
			exit(0);
            break;
            
		default:
            break;
	}
    
	glutPostRedisplay();
}



void mouse(int button, int state, int x, int y){
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};
	if((button == MOUSE_LEFT_BUTTON)&&(state == GLUT_UP)){
		if(nPt==MAXPTNO)
		{
			cout << "Error: Exceeded the maximum number of points." << endl;
			return;
		}
		ptList[nPt].x=x;
		ptList[nPt].y=y;
        ptFirstPointsList[nPt].x = x;
        ptFirstPointsList[nPt].y = y;
		nPt++;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv){
	cout<<"CS3241 Lab 4"<< endl<< endl;
	cout << "Left mouse click: Add a control point"<<endl;
	cout << "Q: Quit" <<endl;
	cout << "P: Toggle displaying control points" <<endl;
	cout << "L: Toggle displaying control lines" <<endl;
	cout << "E: Erase all points (Clear)" << endl;
	cout << "C: Toggle C1 continuity" <<endl;	
	cout << "T: Toggle displaying tangent vectors" <<endl;
	cout << "O: Toggle displaying objects" <<endl;
	cout << "R: Read in control points from \"savefile.txt\"" <<endl;
	cout << "W: Write control points to \"savefile.txt\"" <<endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS3241 Assignment 4");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
    
	return 0;
}
