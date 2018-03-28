// CS3241Lab3.cpp : Defines the entry point for the console application.
#include "math.h"
#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#include "glut.h"
#define M_PI 3.141592654
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

#define Store_SIZE    300
#define PI          3.14159265358979323846

// global variable
#define DEFAULT_EYEX 0.0;
#define DEFAULT_EYEY 0.0;
#define DEFAULT_EYEZ 0.0;
#define DEFAULT_CENTERX 0.0;
#define DEFAULT_CENTERY 0.0;
#define DEFAULT_CENTERZ -1.0;
#define DEFAULT_UPX 0.0;
#define DEFAULT_UPY 1.0;
#define DEFAULT_UPZ 0.0;


bool m_Smooth = false;
bool m_Highlight = false;
bool animate_camera = false;
bool disco_light = false;
GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
GLfloat field_of_view = 40.0;
GLfloat near = 1.0;
GLfloat far = 30.0;
GLfloat intervalDis = 0.9;
GLfloat far_intervalDis = 5;
GLfloat intervalFovy = 5;
GLfloat x_translation = 0.0;
GLfloat aspect_ratio = 1.0;
GLdouble eyex = DEFAULT_EYEX;
GLdouble eyey = DEFAULT_EYEY;
GLdouble eyez = DEFAULT_EYEZ;
GLdouble centerx = DEFAULT_CENTERX;
GLdouble centery = DEFAULT_CENTERY;
GLdouble centerz = DEFAULT_CENTERZ;
GLdouble upx = DEFAULT_UPX;
GLdouble upy = DEFAULT_UPY;
GLdouble upz = DEFAULT_UPZ;

int mouseButton = 0;
int moving, startx, starty;

#define NO_OBJECT 4;
int current_object = 2;

using namespace std;

void setupLighting(){
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	// Lights, material properties
	GLfloat	ambientProperties[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat	diffuseProperties[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat	specularProperties[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPosition[] = { -100.0f, 100.0f, 100.0f, 1.0f };
    
    if (disco_light){
        int value = fmod((glutGet(GLUT_ELAPSED_TIME) * 0.3), 768);
        GLfloat r = 0, g = 0 , b = 0;
        if (value < 256){
            r = value;
            g = 255 - value;
            b = 255;
        } else if (value < 512) {
            value = value - 256;
            r = 255;
            g = value;
            b = 255 - value;
        } else if (value < 768) {
            value = value - 512;
            r = 255 - value;
            g = 255;
            b = value;
        }
        r = r / 255.0;
        g = g / 255.0;
        b = b / 255.0;
        GLfloat    newSpecularProperties[] = { r, g, b, 1.0f };
        for (int i = 0; i < 4; i++) {
            specularProperties[i] = newSpecularProperties[i];
        }
    }
    
	glClearDepth(1.0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

int random(int m){
    return rand()%m;
}

void drawStar(){
    glPushMatrix();
    glTranslated(0, 0, 0);
    glColor3f(1, 1, 1);
    glPointSize(6.5);
    glBegin(GL_POINTS);
    for(int i=0; i<=10; i++){
        glVertex3i(-20+random(40), -20+random(40), -20+random(40));
    }
    glEnd();
    glPopMatrix();
}

void drawSphere(double r){
	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	float no_shininess = 0.0f;
    float shininess = 120.0f;
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight)
	{
		// your codes for highlight here
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }
	else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}
	int i, j;
	int n = 20;
	for (i = 0; i<n; i++)
		for (j = 0; j<2 * n; j++)
			if (m_Smooth)
			{
				glBegin(GL_POLYGON);

				// the normal of each vertex is actaully its own coordinates normalized for a sphere
				// your normal here
                glNormal3d(sin(i*M_PI / n)*cos(j*M_PI / n), cos(i*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				// your normal here
                glNormal3d(sin((i + 1)*M_PI / n)*cos(j*M_PI / n), cos((i + 1)*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				// your normal here
                glNormal3d(sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				// your normal here
                glNormal3d(sin(i*M_PI / n)*cos((j + 1)*M_PI / n), cos(i*M_PI / n)*cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			}
			else	{
				glBegin(GL_POLYGON);
				// Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
				glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			}
}

void drawCube(){
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float shininess = 120.0f;
    float mat_ambient[] = { 0.6f, 0.7f, 0.9f, 1.0f };
    float mat_diffuse[] = { 0.7f, 0.6f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    if (m_Highlight)
    {
        // your codes for highlight here
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }
    else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    int i;
    glBegin(GL_POLYGON);
    glNormal3d(0, -1, 0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3d(0, 1, 0);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glEnd();
    for(i=0; i<4; i++){
        glRotated(90, 0, 1, 0);
        glPushMatrix();
        glNormal3d(0, 0, -1);
        glBegin(GL_POLYGON);
        glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(0.5, -0.5, -0.5);
        glVertex3f(-0.5, -0.5, -0.5);
        glVertex3f(-0.5, 0.5, -0.5);
        glEnd();
        glPopMatrix();
    }
}

void drawCone(GLdouble baseRadius, GLdouble topRadius,
               GLdouble height, GLint slices, GLint stacks){
    GLint i,j;
    GLfloat angle, length, radius_distance;
    GLfloat zBottom, zTop;
    GLfloat topAndBottom_normal, side_normal;
    GLfloat Botto_radius, Top_radius;
    GLfloat sinStore[Store_SIZE], cosStore[Store_SIZE];
    GLfloat ver_SinStore[Store_SIZE], ver_CosStore[Store_SIZE];
    GLfloat face_SinStore[Store_SIZE], face_CosStore[Store_SIZE];
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float shininess = 120.0f;
    
    if (m_Highlight)
    {
        // your codes for highlight here
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }
    else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }

    int verStore, faceStore;
    verStore = faceStore = 0;
    if(m_Smooth){
        // Store is vertices' normals
        verStore = 1;
    }else{
        // Store3 is faces' normals
        faceStore = 1;
    }

    // Compute length for the normal vector
    radius_distance = baseRadius - topRadius;
    length = sqrt(radius_distance*radius_distance + height*height);
    topAndBottom_normal = radius_distance / length;
    side_normal = height / length;
    
    for (i = 0; i < slices; i++) {
        angle = 2 * PI * i / slices;
        if (verStore) {
                ver_SinStore[i] = side_normal * sin(angle);
                ver_CosStore[i] = side_normal * cos(angle);
        }
        // Store is the vertex locations
        sinStore[i] = sin(angle);
        cosStore[i] = cos(angle);
    }
    
    if (faceStore) {
        for (i = 0; i < slices; i++) {
            angle = 2 * PI * (i-0.5) / slices;
            face_SinStore[i] = side_normal * sin(angle);
            face_CosStore[i] = side_normal * cos(angle);
        }
    }
    
    sinStore[slices] = sinStore[0];
    cosStore[slices] = cosStore[0];
    if (verStore) {
        ver_SinStore[slices] = ver_SinStore[0];
        ver_CosStore[slices] = ver_CosStore[0];
    }
    if (faceStore) {
        face_SinStore[slices] = face_SinStore[0];
        face_CosStore[slices] = face_CosStore[0];
    }
    
    for (j = 0; j < stacks; j++) {
        zBottom = j * height / stacks;
        zTop = (j + 1) * height / stacks;
        Botto_radius = baseRadius - radius_distance * ((float) j / stacks);
        Top_radius = baseRadius - radius_distance * ((float) (j + 1) / stacks);
        
        glBegin(GL_QUAD_STRIP);
        for (i = 0; i <= slices; i++) {
            if(m_Smooth) {
                glNormal3f(ver_SinStore[i], ver_CosStore[i], topAndBottom_normal);
            }else{
                glNormal3f(face_SinStore[i], face_CosStore[i], topAndBottom_normal);
            }
            glVertex3f(Botto_radius * sinStore[i], Botto_radius * cosStore[i], zBottom);
            glVertex3f(Top_radius * sinStore[i], Top_radius * cosStore[i], zTop);
        }
        glEnd();
    }
    zBottom = 0;
    glBegin(GL_POLYGON);
    glNormal3d(0, 0, -1);
    for (i = 0; i <= slices; i++) {
        glVertex3f(baseRadius * sinStore[i], baseRadius * cosStore[i], zBottom);
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    glNormal3d(0, 0, 1);
    for (i = 0; i <= slices; i++) {
        glVertex3f(topRadius * sinStore[i], topRadius * cosStore[i], zTop);
    }
    glEnd();
}

void drawWave(double r){
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float shininess = 120.0f;
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    
    if (m_Highlight){
        // your codes for highlight here
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }else{
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    int i, j;
    int n = 20;
    for (i = 0; i < n/3; i++)
        for (j = 0; j < n; j++)
            if (m_Smooth)
            {
                glBegin(GL_POLYGON);
                glNormal3d(sin(i*M_PI / n)*cos(j*M_PI / n), cos(i*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
                glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                glNormal3d(sin((i + 1)*M_PI / n)*cos(j*M_PI / n), cos((i + 1)*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
                glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                glNormal3d(sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n));
                glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
                glEnd();
            }
            else    {
                glBegin(GL_POLYGON);
                glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n));
                glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
                glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
                glEnd();
            }
}

void exploreTheUniverse(){
    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_ambient[] = { 0.70588f, 0.70588f, 0.70588f, 1.0f };
    float mat_diffuse[] = { 0.2f, 0.2f, 0.1f, 1.0f };
    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float shininess = 120.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    if (m_Highlight)
    {
        // your codes for highlight here
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }
    else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
    }
    
    glPushMatrix();//1
    glScaled(0.4, 0.4, 0.4);
    //Body
    glPushMatrix();
    glPushMatrix();
    drawCone(0.90f, 0.90f, 2.00f, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0, 0, -1.8);
    drawCone(0.90f, 0.90f, 1.30f, 16, 16);
    glPopMatrix();
    //Head
    float mat_ambient6[] = { 0.7, 0.67, 0.68, 1.0f };
    float mat_diffuse6[] = { 0.2f, 0.2f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient6);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse6);
    glTranslated(0, 0, -2.2);
    drawCone(0.0f, 0.90f, 0.40f, 16, 16);
    glPopMatrix();
    
    //Wings
    glPushMatrix();
    glTranslated(2.5, 0, -0.40);
    glScaled(2.4, 0.15, 1.5);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslated(-2.5, 0, -0.40);
    glScaled(2.4, 0.15, 1.5);
    drawCube();
    glPopMatrix();
    //joint
    float mat_ambient2[] = { 0.7529, 0.7529, 0.7529, 1.0f };
    float mat_diffuse2[] = { 0.7f, 0.6f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
    glPushMatrix();
    glTranslated(0, -0.7, -1.0);
    drawCone(0.10f, 0.10f, 1.90f, 16, 16);
    glTranslated(0, 0.7, 0);
    drawCone(0.10f, 0.10f, 1.90f, 16, 16);
    glPushMatrix();
    glTranslated(0.7, 0, 0);
    drawCone(0.10f, 0.10f, 1.90f, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-0.7, 0, 0);
    drawCone(0.10f, 0.10f, 1.90f, 16, 16);
    glPopMatrix();
    glTranslated(0, 0.7, 0);
    drawCone(0.10f, 0.10f, 1.90f, 16, 16);
    glPopMatrix();
    float mat_ambient3[] = { 0.588235f, 0.588235f, 0.588235f, 1.0f };
    float mat_diffuse3[] = { 0.7f, 0.6f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient3);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse3);
    glPushMatrix();
    glTranslated(0, 0, 0.2);
    glRotated(90, 0, 1, 0);
    glTranslated(1.0, 0, 0);
    drawCone(0.07f, 0.07f, 1.60f, 16, 16);
    glPushMatrix();
    glTranslated(-0.9, 0, 0);
    drawCone(0.07f, 0.07f, 1.60f, 16, 16);
    glPopMatrix();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(0, 0, 0.2);
    glRotated(-90, 0, 1, 0);
    glTranslated(-1.0, 0, 0);
    drawCone(0.07f, 0.07f, 1.60f, 16, 16);
    glPushMatrix();
    glTranslated(0.9, 0, 0);
    drawCone(0.07f, 0.07f, 1.60f, 16, 16);
    glPopMatrix();
    glPopMatrix();
    
    glPushMatrix();
    drawCone(0.07f, 0.07f, 2.80f, 16, 16);
    glPopMatrix();
    
    float mat_ambient5[] = { 1.0f, 0.8f, 0.4f, 1.0f };
    float mat_diffuse5[] = { 0.7f, 0.6f, 0.4f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient5);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse5);
    glPushMatrix();
    glTranslated(0, 0, 2.8);
    drawSphere(0.07);
    glPopMatrix();
    
    float mat_ambient4[] = { 0.8, 0.7, 0.6, 1.0f };
    float mat_diffuse4[] = { 0.7f, 0.6f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient4);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse4);
    glPushMatrix();
    glTranslated(0, 0, 2.0);
    drawSphere(0.250f);
    glPopMatrix();
    glPopMatrix();
}

void waveAnimation(){
    float spinSpeed = 0;
    clock_t tick = clock();
    float mat_ambient4[] = { 1.0, 0.9, 0.7, 1.0f };
    float mat_diffuse4[] = { 0.7f, 0.6f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient4);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse4);
    glPushMatrix();
    spinSpeed = 360 - (float)tick/400000*200;
    glRotated(spinSpeed, 0, 0, 1);
    glTranslated(0, 0, -0.5);
    drawWave(1.8);
    glPopMatrix();
    glPopMatrix();
}

void drawRing(GLfloat r, GLfloat R, GLint nsides, GLint rings){
    int i, j;
    GLfloat theta, phi, theta1;
    GLfloat cosTheta, sinTheta;
    GLfloat cosTheta1, sinTheta1;
    GLfloat ringDelta, sideDelta;
    
    ringDelta = 2.0 * M_PI / rings;
    sideDelta = 2.0 * M_PI / nsides;
    
    theta = 0.0;
    cosTheta = 1.0;
    sinTheta = 0.0;
    for (i = rings - 1; i >= 0; i--) {
        theta1 = theta + ringDelta;
        cosTheta1 = cos(theta1);
        sinTheta1 = sin(theta1);
        glBegin(GL_QUAD_STRIP);
        phi = 0.0;
        for (j = nsides; j >= 0; j--) {
            GLfloat cosPhi, sinPhi, dist;
            
            phi += sideDelta;
            cosPhi = cos(phi);
            sinPhi = sin(phi);
            dist = R + r * cosPhi;
            
            glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
            glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
            glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
            glVertex3f(cosTheta * dist, -sinTheta * dist,  r * sinPhi);
        }
        glEnd();
        theta = theta1;
        cosTheta = cosTheta1;
        sinTheta = sinTheta1;
    }
}

void output(int x, int y, float r, float g, float b, char *string)
{
    glColor3f( r, g, b );
    glRasterPos2f(x, y);
    int len, i;
    len = (int)strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}
//  In honor of Dr.Hawking
void HawkingsTelescope(){
    float lengthSpeed = 0;
    clock_t tick = clock();
    lengthSpeed = 0.40 * sin((float)tick/400000*1.8);
    output(-1.5, 1.8, 1, 1, 0.8, "ALL IN HONOR OF GREAT DR.HAWKING!");
    glPushMatrix();
    glScaled(0.7, 0.7, 0.7);
    glPushMatrix();
    float mat_ambient[] = { 150/255, 150/255, 150/255, 1.0f };
    float mat_diffuse[] = { 0.7f, 0.6f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glPushMatrix();
    glTranslated(0.0, 0.0, 1.4);
    drawSphere(0.38);
    glPopMatrix();
    drawSphere(0.4);
    drawCone(0.4, 0.4, 1.7, 16, 16);
    glPushMatrix();
    glTranslated(0, 0, -1.1);
    drawCone(0.3, 0.3, 1.1, 16, 16);
    glPushMatrix();
    glTranslated(0, 0, -1.0);
    drawCone(0.25, 0.25, 1.0, 16, 16);
    glPushMatrix();
    glTranslated(0, 0, -0.80);
    glTranslated(0, 0, lengthSpeed);
    drawCone(0.2, 0.2, 1.3, 16, 16);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    
    float mat_ambientRing[] = { 0.9, 0.8, 0.5, 1.0f };
    float mat_diffuseRing[] = { 0.8f, 0.8f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambientRing);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseRing);
    glPushMatrix();
    glScaled(0.7, 0.7, 0.7);
    glPushMatrix();
    glTranslated(0, 0, 0.1);
    drawRing(0.15, 0.29, 16, 16);
    glPushMatrix();
    glTranslated(0, 0, -1.15);
    drawRing(0.15, 0.18, 16, 16);
    glPushMatrix();
    glTranslated(0, 0, -1.0);
    drawRing(0.12, 0.145, 16, 16);
    glPushMatrix();
    float mat_ambientRing_head[] = { 1.0, 0.8, 0.5, 1.0f };
    float mat_diffuseRing_head[] = { 0.8f, 0.8f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambientRing_head);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseRing_head);
    glTranslated(0, 0, -0.80);
    glTranslated(0, 0, lengthSpeed);
    drawRing(0.09, 0.145, 16, 16);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

void display(void){
    //Add Projection tool and Camera Movement
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(field_of_view, aspect_ratio, near, far);
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0, 0, -6);
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glScalef(zoom, zoom, zoom);
    
    if (animate_camera) {
        double angle = fmod((glutGet(GLUT_ELAPSED_TIME) * 0.001), 360);
        double x = cos(angle);
        double y = sin(angle);
        gluLookAt(x, y, eyez, centerx, centery, centerz, upx, upy, upz);
    } else {
        gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
    }
    
    float mat_ambient_sphere[] = { 0.5f, 0.4f, 0.3f, 1.0f };
    float mat_diffuse_sphere[] = { 0.3f, 0.6f, 0.5f, 1.0f };
    float mat_ambient_cone[] = { 0.0f, 0.0f, 0.3f, 1.0f };
    float mat_diffuse_cone[] = { 0.3f, 0.6f, 0.5f, 1.0f };
	switch (current_object) {
	case 0:
        glClearColor(0, 0, 0.150, 1.0);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_sphere);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_sphere);
		drawSphere(1);
		break;
	case 1:
		// second primitive object here
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_cone);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_cone);
        drawCone(0.60f, 0.0f, 1.0f, 16, 16);
		break;
	case 2:
		// first composite object
        glClearColor(0, 0, 0.15, 1.0);
        drawStar();
        exploreTheUniverse();
        waveAnimation();
		break;
	case 3:
		// second composite object
        glClearColor(1.0, 1.0, 1.0, 1.0);
        HawkingsTelescope();
		break;
	default:
		break;
	};
	glPopMatrix();
	glutSwapBuffers();
}

void idle(){
    // update animation
    glutPostRedisplay();
}


void resetCamera(){
    eyez = 0.0;
    eyey = 0.0;
    eyex = 0.0;
    zoom = 1.0;
    angle = 0;
    angle2 = 0;
    field_of_view = 40.0;
    x_translation = 0.0;
    near = 1.0;
    far = 30.0;
	return;
}

void setCameraBestAngle() {
    switch (current_object) {
        case 0:
            break;
        case 1:
            angle = 0;
            angle2 = -65;
            break;
        case 2:
            angle = 30;
            angle2 = 20;
            eyez = 1.60;
            eyex = -3.7;
            eyey = 2.50;
//            upy = 1.0;
            break;
        case 3:
            eyex = 0.2;
            eyey = 1.6;
            eyez = 0.3;
            centerx = -1.2;
//            centery = 6.5;
            break;
    }
	return;
}

void keyboard(unsigned char key, int x, int y){
    // add additional commands here to change Field of View and movement
	switch (key) {
	case 'p':
	case 'P':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'w':
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'v':
	case 'V':
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 's':
	case 'S':
		m_Smooth = !m_Smooth;
		break;
	case 'h':
	case 'H':
		m_Highlight = !m_Highlight;
		break;

	case 'n':
		// To adjust Near
            near -= intervalDis;
            if (near <= 0){
                near = 0.1;
            }
		break;

	case 'N':
            near += far_intervalDis;
		break;

	case 'f':
		// To adjust Far
            far -= far_intervalDis;
            if(far <= 0){
                far = 0.1;
            }
		break;

	case 'F':
            far += intervalDis;
		break;

	case 'o':
		// To adjust Fovy
            field_of_view += intervalFovy;
		break;

	case 'O':
            field_of_view -= intervalFovy;
		break;

	case 'r':
		resetCamera();
		break;

	case 'R':
        resetCamera();
		setCameraBestAngle();
		break;

    case 'a':
    case 'A':
        animate_camera = !animate_camera;
        break;
    
    case 'd':
    case 'D':
        disco_light = !disco_light;
        break;
            
	case '1':
	case '2':
	case '3':
	case '4':
		current_object = key - '1';
		break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
	if(state == GLUT_DOWN){
		mouseButton = button;
		moving = 1;
		startx = x;
		starty = y;
	}
	if(state == GLUT_UP){
		mouseButton = button;
		moving = 0;
	}
}

void motion(int x, int y){
	if(moving){
		if (mouseButton == GLUT_LEFT_BUTTON)
		{
			angle = angle + (x - startx);
			angle2 = angle2 + (y - starty);
		}
		else zoom += ((y - starty)*0.01);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv){
	cout << "CS3241 Chen Yinfang Lab 3" << endl << endl;

	cout << "1-4: Draw different objects" << endl;
	cout << "S: Toggle Smooth Shading" << endl;
	cout << "H: Toggle Highlight" << endl;
	cout << "W: Draw Wireframe" << endl;
	cout << "P: Draw Polygon" << endl;
	cout << "V: Draw Vertices" << endl;
	cout << "n, N: Reduce or increase the distance of the near plane from the camera" << endl;
	cout << "f, F: Reduce or increase the distance of the far plane from the camera" << endl;
	cout << "o, O: Reduce or increase the distance of the povy plane from the camera" << endl;
	cout << "r: Reset camera to the initial parameters when the program starts" << endl;
	cout << "R: Change camera to another setting that is has the best viewing angle for your object" << endl;
	cout << "ESC: Quit" << endl << endl;
	cout << "Left mouse click and drag: rotate the object" << endl;
	cout << "Right mouse click and drag: zooming" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("CS3241 Chen Yinfang Lab 3");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(display);
    glutIdleFunc(idle);
	glMatrixMode(GL_PROJECTION);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	setupLighting();
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glutMainLoop();

	return 0;
}

/* Last Version of Cone */
////  Triangle surface
//class tri_face{
//public:
//    float p1[3] = { 0.0f, 0.0f, 0.0f };
//    float p2[3] = { 0.0f, 0.0f, 0.0f };
//    float p3[3] = { 0.0f, 0.0f, 0.0f };
//    float normal[3] = { 0.0f, 0.0f, 0.0f };
//    float normalLength;
//};
////  Cross Product Func
//void cross_product(tri_face& tri){
//    float x, y, z;
//    x = tri.p2[0] - tri.p1[0];
//    y = tri.p2[1] - tri.p1[1];
//    z = tri.p2[2] - tri.p1[2];
//    //    float vector[3] = { x, y, z };
//    float x1, y1, z1;
//    x1 = tri.p3[0] - tri.p1[0];
//    y1 = tri.p3[1] - tri.p1[1];
//    z1 = tri.p3[2] - tri.p1[2];
//    //    float vector2[3] = { x1, y1, z1 };
//    tri.normal[0] = y*z1 - z*y1;
//    tri.normal[1] = z*x1 - x*z1;
//    tri.normal[2] = x*y1 - y*x1;
//    tri.normalLength = sqrt(tri.normal[0]*tri.normal[0] + tri.normal[1]*tri.normal[1] + tri.normal[2]*tri.normal[2]);
//    tri.normal[0] /= -tri.normalLength;
//    tri.normal[1] /= -tri.normalLength;
//    tri.normal[2] /= -tri.normalLength;
//}
//
//void drawCone(){
//    float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//    float mat_ambient[] = { 0.7f, 0.3f, 0.3f, 1.0f };
//    float mat_diffuse[] = { 0.7f, 0.6f, 0.5f, 1.0f };
//    float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
//    float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
//    float no_shininess = 0.0f;
//    float shininess = 120.0f;
//    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
//
//    if (m_Highlight)
//    {
//        // your codes for highlight here
//        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
//    }
//    else {
//        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
//        glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
//    }
//    GLfloat x0, y0, x, y, x1, y1, x2, y2, angle, R;
//    GLfloat delta = M_PI / 8.0f;
//    R = 0.6;
//    if(m_Smooth){   // normal of the vertex
//        glBegin(GL_POLYGON);
//        for(angle = 0.0f; angle < (2.0f * M_PI) + delta; angle += delta){
//            x0 = R * sin(angle - delta);
//            y0 = R * cos(angle - delta);
//            x2 = R * sin(angle+2*delta);
//            y2 = R * cos(angle+2*delta);
//            x = R * sin(angle);
//            y = R * cos(angle);
//            x1 = R * sin(angle + delta);
//            y1 = R * cos(angle + delta);
//            glNormal3d(0, 0, 1);
//            glVertex3f(0.0f, 0.0f, 1.0f);   //  first given vertex is the vertex of the cone
//            //            glNormal3d(-(y1-y) * 0.5, -(x-x1) * 0.5, (-1-(x*y1-x1*y)) * 0.5);
//            tri_face tri0, tri, tri2;
//            //  tri0
//            tri0.p1[0] = x0;
//            tri0.p1[1] = y0;
//            tri0.p1[2] = 0.0f;
//            tri0.p2[0] = x;
//            tri0.p2[1] = y;
//            tri0.p2[2] = 0.0f;
//            tri0.p3[0] = 0;
//            tri0.p3[1] = 0;
//            tri0.p3[2] = 1.0f;
//            cross_product(tri0);
//            //  tri
//            tri.p1[0] = x;
//            tri.p1[1] = y;
//            tri.p1[2] = 0.0f;
//            tri.p2[0] = x1;
//            tri.p2[1] = y1;
//            tri.p2[2] = 0.0f;
//            tri.p3[0] = 0;
//            tri.p3[1] = 0;
//            tri.p3[2] = 1.0f;
//            cross_product(tri);
//            //  tri2
//            tri2.p1[0] = x1;
//            tri2.p1[1] = y1;
//            tri2.p1[2] = 0.0f;
//            tri2.p2[0] = x2;
//            tri2.p2[1] = y2;
//            tri2.p2[2] = 0.0f;
//            tri2.p3[0] = 0;
//            tri2.p3[1] = 0;
//            tri2.p3[2] = 1.0f;
//            cross_product(tri2);
//            //  tri normal vector
//            float nx, ny, nz;
//            nx = (tri.normal[0] + tri2.normal[0]) / 2;
//            ny = (tri.normal[1] + tri2.normal[1]) / 2;
//            nz = (tri.normal[2] + tri2.normal[2]) / 2;
//            float nx0, ny0, nz0;
//            nx0 = (tri.normal[0] + tri0.normal[0]) / 2;
//            ny0 = (tri.normal[1] + tri0.normal[1]) / 2;
//            nz0 = (tri.normal[2] + tri0.normal[2]) / 2;
//            glNormal3d(x, y, 0);
//            glNormal3d(nx0, ny0, nz0);
//            glVertex3f(x, y, 0.0f);
//            //            glNormal3d(x1, y1, 0);
//            glNormal3d(nx, ny, nz);
//            glVertex3f(x1, y1, 0.0f);
//        }
//        glEnd();
//        glBegin(GL_POLYGON);   //  bottom
//        glNormal3d(0, 0, -1);
//        for(angle = 0; angle < (2.0f * M_PI); angle += (M_PI / 8.0f))
//        {
//            x = R * sin(angle);
//            y = R * cos(angle);
//            //  glNormal3d((x+x1)/3, (y+y1)/3, -1);
//            glVertex3f(x, y, 0.0f);
//        }
//        glEnd();
//    }else{
//        //  flat: normal of the polygon
//        glBegin(GL_POLYGON);
//        for(angle = 0.0f; angle < (2.0f * M_PI); angle += delta)
//        {
//            x = R * sin(angle);
//            y = R * cos(angle);
//            x1 = R * sin(angle + delta);
//            y1 = R * cos(angle + delta);
//            //            glNormal3d((x+x1)/3, (y+y1)/3, 1/3);
//            //            glNormal3d(-(y1-y), -(x-x1), -(x*y1-x1*y));
//            tri_face tri;
//            tri.p1[0] = x;
//            tri.p1[1] = y;
//            tri.p1[2] = 0.0f;
//            tri.p2[0] = x1;
//            tri.p2[1] = y1;
//            tri.p2[2] = 0.0f;
//            tri.p3[0] = 0;
//            tri.p3[1] = 0;
//            tri.p3[2] = 1.0f;
//            cross_product(tri);
//            //  normal vector
//            float nx, ny, nz;
//            nx = tri.normal[0];
//            ny = tri.normal[1];
//            nz = tri.normal[2];
//            glNormal3d(nx, ny, nz);
//            glVertex3f(0.0f, 0.0f, 1.0f);   //  first given vertex is the vertex of the cone
//            glVertex3f(x, y, 0.0f);
//            glVertex3f(x1, y1, 0.0f);
//        }
//        glEnd();
//        glBegin(GL_POLYGON);
//        glNormal3d(0, 0, -1);
//        for(angle = 0; angle < (2.0f * M_PI); angle += (M_PI / 8.0f)){
//            x = R * sin(angle);
//            y = R * cos(angle);
//            //            glNormal3d((x+x1)/3, (y+y1)/3, -1);
//            glVertex3f(x, y, 0.0f);
//        }
//        glEnd();
//    }
//}



