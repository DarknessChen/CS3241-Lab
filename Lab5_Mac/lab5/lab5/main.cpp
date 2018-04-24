// CS3241Lab5.cpp
#include <cmath>
#include <iostream>
#include "vector3D.h"
#include <chrono>
#include <float.h>

/* Include header files depending on platform */
#ifdef _WIN32
#include "glut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

#define WINWIDTH 600
#define WINHEIGHT 400
int NUM_OBJECTS = 4;

#define MAX_RT_LEVEL 50
#define NUM_SCENE 2

float* pixelBuffer = new float[WINWIDTH * WINHEIGHT * 3];

class Ray { // a ray that start with "start" and going in the direction "dir"
public:
    Vector3 start, dir;
};

class RtObject {
    
public:
    virtual double intersectWithRay(Ray, Vector3& pos, Vector3& normal) = 0; // return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
    // Materials Properties
    double ambiantReflection[3] ;
    double diffusetReflection[3] ;
    double specularReflection[3] ;
    double speN = 300;
    
    
};

class Sphere : public RtObject {
    
    Vector3 center_;
    double r_;
public:
    Sphere(Vector3 c, double r) { center_ = c; r_ = r; };
    Sphere() {};
    void set(Vector3 c, double r) { center_ = c; r_ = r; };
    double intersectWithRay(Ray, Vector3& pos, Vector3& normal);
};

class Cube : public RtObject {
    Vector3 basePoint;
    double length;
public:
    Cube(Vector3 c, double l) { basePoint = c; length = l;};
    Cube() {};
    void set(Vector3 c, double l) { basePoint = c; length = l;};
    double intersectWithRay(Ray, Vector3& pos, Vector3& normal);
};

class Plane : public RtObject {
    // the plane of any point v such that v . n = d
    Vector3 n_;
    double d_;
public:
    Plane(Vector3 n, double d) { n_ = n; d_ = d; };
    void set(Vector3 n, double d) { n_ = n; d_ = d; };
    double intersectWithRay(Ray, Vector3& pos,
                            Vector3& normal);
};

double Plane::intersectWithRay(Ray r, Vector3& intersection,
                               Vector3& normal)
{
    double t = (d_ - dot_prod(r.start, n_)) / (dot_prod(r.dir, n_));
    
    intersection = r.start + r.dir * t;
    if (dot_prod(r.dir, n_) > 0)
        normal = -n_;
    else
        normal = n_;
    return t;
}

RtObject **objList; // The list of all objects in the scene

// Global Variables
// Camera Settings
Vector3 cameraPos(0,0,-500);

// assume the the following two vectors are normalised
Vector3 lookAtDir(0,0,1);
Vector3 upVector(0,1,0);
Vector3 leftVector(1, 0, 0);
float focalLen = 500;

// Light Settings

Vector3 lightPos(900,1000,-1500);
double ambiantLight[3] = { 0.4, 0.4, 0.4 };
double diffuseLight[3] = { 0.7, 0.7, 0.7 };
double specularLight[3] = { 0.5, 0.5, 0.5 };


double bgColor[3] = { 0.1,0.1,0.4 };

int sceneNo = 0;

double lightDistance(Vector3 intersection, Vector3& lightDir ) {
    double distance_light;
    distance_light = sqrt(pow((lightPos.x[0] - intersection.x[0]), 2) + pow((lightPos.x[1] - intersection.x[1]), 2) + pow((lightPos.x[2] - intersection.x[2]), 2));
    lightDir = lightPos - intersection;
    lightDir.normalize();
    return distance_light;
}

void viewerDir(Ray r, Vector3 intersection, Vector3& V_dir) {
    //V_dir = r.start - intersection;
    V_dir = cameraPos - intersection;
    V_dir.normalize();
}

bool createShadow(int targetObject, Vector3 coloredPoint){
    Ray rayFromLight;
    int i, firstBlockObj = targetObject;
    rayFromLight.start = lightPos;
    rayFromLight.dir = coloredPoint - lightPos;
    double deltaShadow;
    double mint = DBL_MAX;
    Vector3 intersection, normal;
    
    for (i = 0; i < NUM_OBJECTS; i++)
    {
        if (((deltaShadow = objList[i]->intersectWithRay(rayFromLight, intersection, normal)) > 0) && (deltaShadow < mint))
        {
            mint = deltaShadow;
            firstBlockObj = i;
        }
    }
    
    if (firstBlockObj != targetObject) {
        return true;
    }else{
        return false;
    }
}

double Sphere::intersectWithRay(Ray r, Vector3& intersection, Vector3& normal)
// return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
{
    // Step 1
    // alpha, beta, gamma
    double alpha, beta, gamma;
    alpha = dot_prod(r.dir, r.dir);
    beta = 2 * dot_prod(r.dir, (r.start - center_));
    gamma = ((r.start - center_).length()) * ((r.start - center_).length()) - (r_ * r_);
    
    double equ_delta;
    equ_delta = beta * beta - 4 * alpha * gamma;
    if (equ_delta <= 0 || alpha == 0) {
        // no intersection
        return -1;
    }else {
        // pick the nearer intersection
        double t;
        t = (double)(-sqrt(equ_delta) - beta) / (double)(2 * alpha);
        if (t > 0) {
            intersection = r.start + r.dir * t;
            normal = intersection - center_;
            // remember to normalize
            normal.normalize();
            return t;
        }else {
            // if t <= 0, then there is no intersection (behind the eye)
            return -1;
        }
    }
    return -1;
}

double Cube::intersectWithRay(Ray r, Vector3& intersection, Vector3& normal){
    double store[6], alpha[6], beta[6];
    //  init
    for (int i = 0; i < 6; i++) {
        store[i] = 0;
        alpha[i] = 0;
        beta[i] = 0;
    }
    //  Plane1
    double alpha1, beta1, t1;
    store[0] = t1 = -(r.start.x[1] - basePoint.x[1]) / r.dir.x[1];
    alpha[0] = alpha1 = (t1 * r.dir.x[0] + r.start.x[0] - basePoint.x[0]) / length;
    beta[0] = beta1 = (t1 * r.dir.x[2] + r.start.x[2] - basePoint.x[2]) / length;
    //  Plane2 is the opposite side of plane1
    double alpha2, beta2, t2;
    store[1] = t2 = -(r.start.x[1] - basePoint.x[1] + 1) / r.dir.x[1];  //  Y axis
    alpha[1] = alpha2 = (t2 * r.dir.x[0] + r.start.x[0] - basePoint.x[0]) / length;    //  X
    beta[1] = beta2 = (t2 * r.dir.x[2] + r.start.x[2] - basePoint.x[2]) / length;     //  Z
    
    //  Plane3
    double alpha3, beta3, t3;
    store[2] = t3 = -(r.start.x[0] - basePoint.x[0]) / r.dir.x[0];
    alpha[2] = alpha3 = (t3 * r.dir.x[1] + r.start.x[1] - basePoint.x[1]) / length;
    beta[2] = beta3 = (t3 * r.dir.x[2] + r.start.x[2] - basePoint.x[2]) / length;
    //  Plane4
    double alpha4, beta4, t4;
    store[3] = t4 = -(r.start.x[0] - basePoint.x[0] + 1) / r.dir.x[0];
    alpha[3] = alpha4 = (t4 * r.dir.x[1] + r.start.x[1] - basePoint.x[1]) / length;
    beta[3] = beta4 = (t4 * r.dir.x[2] + r.start.x[2] - basePoint.x[2]) / length;
    
    //  Plane5
    double alpha5, beta5, t5;
    store[4] = t5 = -(r.start.x[2] - basePoint.x[2]) / r.dir.x[2];
    alpha[4] = alpha5 = (t5 * r.dir.x[0] + r.start.x[0] - basePoint.x[0]) / length;
    beta[4] = beta5 = (t5 * r.dir.x[1] + r.start.x[1] - basePoint.x[1]) / length;
    //  Plane6
    double alpha6, beta6, t6;
    store[5] = t6 = -(r.start.x[2] - basePoint.x[2] + 1) / r.dir.x[2];
    alpha[5] = alpha6 = (t6 * r.dir.x[0] + r.start.x[0] - basePoint.x[0]) / length;
    beta[5] = beta6 = (t6 * r.dir.x[1] + r.start.x[1] - basePoint.x[1]) / length;
    
    double flag_min_pos = -1;
    int flag_num = -1;
    for (int i = 0; i < 6; i++) {
        if (store[i] > 0 && (flag_min_pos < 0 || store[i] < flag_min_pos) && alpha[i] <= 1 && alpha[i] >= 0 && beta[i] >= 0 && beta[i] <= 1) {
            flag_min_pos = store[i];
            flag_num = i;
        }
    }
    
    if (flag_min_pos <= 0 || flag_num < 0) {
        return -1;
    }else {
        intersection = r.start + r.dir * flag_min_pos;  //  *t
        int XYZ = flag_num / 2;   //  0 or 1 or 2
        int PorN = flag_num % 2;  //  0 or 1
        if (XYZ == 0) {
            //  Y
            normal.x[0] = 0;
            normal.x[1] = pow(-1, PorN-1);
            normal.x[2] = 0;
            return flag_min_pos;
        }else if (XYZ == 1) {
            //  X
            normal.x[0] = pow(-1, PorN-1);
            normal.x[1] = 0;
            normal.x[2] = 0;
            return flag_min_pos;
        }else if (XYZ == 2) {
            //  Z
            normal.x[0] = 0;
            normal.x[1] = 0;
            normal.x[2] = -pow(-1, PorN-1);
            return flag_min_pos;
        }
        
    }
    return -1;
}


int level = 0;
int fromObj = -1;

void rayTrace(Ray ray, double& r, double& g, double& b, int fromObj, int level)
{
    // Step 4
    if (level >= MAX_RT_LEVEL) {
        return ;
    }
    int goBackGround = 1, i = 0;
    
    Vector3 intersection, normal;
    Vector3 lightV;
    Vector3 viewV;
    Vector3 lightReflectionV;
    Vector3 rayReflectionV;
    
    Ray newRay;
    double mint = DBL_MAX, t;
    
    
    for (i = 0; i < NUM_OBJECTS; i++)
    {
        if (((t = objList[i]->intersectWithRay(ray, intersection, normal)) > 0) && (t < mint) && (fromObj != i))
        {
            mint = t;
            // Step 2
            // PIE Ambiant
            double r_ambiant, g_ambiant, b_ambiant;
            r_ambiant = objList[i]->ambiantReflection[0] * ambiantLight[0];
            g_ambiant = objList[i]->ambiantReflection[1] * ambiantLight[1];
            b_ambiant = objList[i]->ambiantReflection[2] * ambiantLight[2];
            
            // Step 3
            // PIE Diffuse
            double r_diffuse, g_diffuse, b_diffuse;
            double lightDis, f_att, user_KA, user_KB, user_KC;
            //Vector3 lightDirection;
            user_KA = user_KB = user_KC = 0.00000027;
            lightDis = lightDistance(intersection, lightV);
            f_att = 1.0 / (user_KA + user_KB * lightDis + user_KC * lightDis * lightDis);
            double NL = dot_prod(normal, lightV);
            r_diffuse = f_att * objList[i]->diffusetReflection[0] * diffuseLight[0] * NL;
            g_diffuse = f_att * objList[i]->diffusetReflection[1] * diffuseLight[1] * NL;
            b_diffuse = f_att * objList[i]->diffusetReflection[2] * diffuseLight[2] * NL;
            if (r_diffuse < 0) {
                r_diffuse = 0;
            }
            if (g_diffuse < 0) {
                g_diffuse = 0;
            }
            if (b_diffuse < 0) {
                b_diffuse = 0;
            }
            
            //PIE Specular
            double r_specular, g_specular, b_specular;
            Vector3 R_vec = (normal * (2 * NL)) - lightV;
            //Here, the V vector's original position is not the viewer's (not cameraPos) position
            //Vector3 V_vec;
            viewerDir(ray, intersection, viewV);
            double RVn;
            RVn = pow(dot_prod(R_vec, viewV), objList[i]->speN);
            r_specular = f_att * objList[i]->specularReflection[0] * specularLight[0] * RVn;
            g_specular = f_att * objList[i]->specularReflection[1] * specularLight[1] * RVn;
            b_specular = f_att * objList[i]->specularReflection[2] * specularLight[2] * RVn;
            
            
            Vector3 r_ray;
            double r_reflect, g_reflect, b_reflect;
            r_reflect = g_reflect = b_reflect = 0.0;
            r_ray = normal * dot_prod(normal, -ray.dir) * 2 + ray.dir;
            newRay.start = intersection;
            newRay.dir = r_ray;
            
            rayTrace(newRay, r_reflect, g_reflect, b_reflect, i, level+1);
            
            
            double reflectionweight = (objList[i]->speN - 50) / 1000.0;
            if (i == 1) {
                r = r_ambiant + r_diffuse + r_specular;
                g = g_ambiant + g_diffuse + g_specular;
                b = b_ambiant + b_diffuse + b_specular;
            }else {
                r = r_ambiant + r_diffuse + r_specular + r_reflect * reflectionweight;
                g = g_ambiant + g_diffuse + g_specular + g_reflect * reflectionweight;
                b = b_ambiant + b_diffuse + b_specular + b_reflect * reflectionweight;
            }
            
            if (createShadow(i, intersection)) {
                r -= 0.3;
                g -= 0.3;
                b -= 0.3;
            }
            
            goBackGround = 0;
        }
    }
    
    if (goBackGround)
    {
        r = bgColor[0];
        g = bgColor[1];
        b = bgColor[2];
    }
    
}


void drawInPixelBuffer(int x, int y, double r, double g, double b)
{
    pixelBuffer[(y*WINWIDTH + x) * 3] = (float)r;
    pixelBuffer[(y*WINWIDTH + x) * 3 + 1] = (float)g;
    pixelBuffer[(y*WINWIDTH + x) * 3 + 2] = (float)b;
}

void renderScene()
{
    int x, y;
    Ray ray;
    double r, g, b;
    
    cout << "Rendering Scene " << sceneNo << " with resolution " << WINWIDTH << "x" << WINHEIGHT << "........... ";
    long long int time1 = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count(); // marking the starting time
    
    ray.start = cameraPos;
    
    Vector3 vpCenter = cameraPos + lookAtDir * focalLen;  // viewplane center
    Vector3 startingPt = vpCenter + leftVector * (-WINWIDTH / 2.0) + upVector * (-WINHEIGHT / 2.0);
    Vector3 currPt;
    
    for(x=0;x<WINWIDTH;x++)
        for (y = 0; y < WINHEIGHT; y++)
        {
            currPt = startingPt + leftVector*x + upVector*y;
            ray.dir = currPt-cameraPos;
            ray.dir.normalize();
            
            rayTrace(ray, r, g, b, fromObj, level);
            
            drawInPixelBuffer(x, y, r, g, b);
        }
    
    long long int time2 = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count(); // marking the ending time
    
    cout << "Done! \nRendering time = " << time2 - time1 << "ms" << endl << endl;
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_DOUBLEBUFFER);
    glDrawPixels(WINWIDTH, WINHEIGHT, GL_RGB, GL_FLOAT, pixelBuffer);
    glutSwapBuffers();
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


void setScene(int i = 0)
{
    if (i > NUM_SCENE)
    {
        cout << "Warning: Invalid Scene Number" << endl;
        return;
    }
    
    if (i == 0)
    {
        objList[0] = new Sphere(Vector3(-130, 80, 120), 100);
        objList[1] = new Sphere(Vector3(130, -80, -80), 100);
        objList[2] = new Sphere(Vector3(-130, -80, -80), 100);
        objList[3] = new Sphere(Vector3(130, 80, 120), 100);
        
        objList[0]->ambiantReflection[0] = 0.1;
        objList[0]->ambiantReflection[1] = 0.4;
        objList[0]->ambiantReflection[2] = 0.4;
        objList[0]->diffusetReflection[0] = 0;
        objList[0]->diffusetReflection[1] = 1;
        objList[0]->diffusetReflection[2] = 1;
        objList[0]->specularReflection[0] = 0.2;
        objList[0]->specularReflection[1] = 0.4;
        objList[0]->specularReflection[2] = 0.4;
        objList[0]->speN = 300;
        
        objList[1]->ambiantReflection[0] = 0.6;
        objList[1]->ambiantReflection[1] = 0.6;
        objList[1]->ambiantReflection[2] = 0.2;
        objList[1]->diffusetReflection[0] = 1;
        objList[1]->diffusetReflection[1] = 1;
        objList[1]->diffusetReflection[2] = 0;
        objList[1]->specularReflection[0] = 0.0;
        objList[1]->specularReflection[1] = 0.0;
        objList[1]->specularReflection[2] = 0.0;
        objList[1]->speN = 50;
        
        objList[2]->ambiantReflection[0] = 0.1;
        objList[2]->ambiantReflection[1] = 0.6;
        objList[2]->ambiantReflection[2] = 0.1;
        objList[2]->diffusetReflection[0] = 0.1;
        objList[2]->diffusetReflection[1] = 1;
        objList[2]->diffusetReflection[2] = 0.1;
        objList[2]->specularReflection[0] = 0.3;
        objList[2]->specularReflection[1] = 0.7;
        objList[2]->specularReflection[2] = 0.3;
        objList[2]->speN = 650;
        
        objList[3]->ambiantReflection[0] = 0.3;
        objList[3]->ambiantReflection[1] = 0.3;
        objList[3]->ambiantReflection[2] = 0.3;
        objList[3]->diffusetReflection[0] = 0.7;
        objList[3]->diffusetReflection[1] = 0.7;
        objList[3]->diffusetReflection[2] = 0.7;
        objList[3]->specularReflection[0] = 0.6;
        objList[3]->specularReflection[1] = 0.6;
        objList[3]->specularReflection[2] = 0.6;
        objList[3]->speN = 650;
        
    }
    
    if (i == 1)
    {
        // Step 5
        objList[0] = new Sphere(Vector3(100, 0, -300), 35);
        objList[1] = new Sphere(Vector3(-40, 0, -250), 20);
        objList[2] = new Sphere(Vector3(0, -250, -100), 200);
        //    objList[3] = new Sphere(Vector3(130, 80, 120), 100);
        objList[3] = new Cube(Vector3(50, -20, 50), 90);
        objList[4] = new Plane(Vector3(0.866, 0, -0.5), -1000);
        objList[5] = new Plane(Vector3(-0.866, 0, -0.5), -1000);
        
        objList[0]->ambiantReflection[0] = 0.1;
        objList[0]->ambiantReflection[1] = 0.4;
        objList[0]->ambiantReflection[2] = 0.4;
        objList[0]->diffusetReflection[0] = 0;
        objList[0]->diffusetReflection[1] = 1;
        objList[0]->diffusetReflection[2] = 1;
        objList[0]->specularReflection[0] = 0.2;
        objList[0]->specularReflection[1] = 0.4;
        objList[0]->specularReflection[2] = 0.4;
        objList[0]->speN = 300;
        
        objList[1]->ambiantReflection[0] = 0.6;
        objList[1]->ambiantReflection[1] = 0.6;
        objList[1]->ambiantReflection[2] = 0.2;
        objList[1]->diffusetReflection[0] = 1;
        objList[1]->diffusetReflection[1] = 1;
        objList[1]->diffusetReflection[2] = 0;
        objList[1]->specularReflection[0] = 0.0;
        objList[1]->specularReflection[1] = 0.0;
        objList[1]->specularReflection[2] = 0.0;
        objList[1]->speN = 50;
        
        objList[2]->ambiantReflection[0] = 0.1;
        objList[2]->ambiantReflection[1] = 0.6;
        objList[2]->ambiantReflection[2] = 0.1;
        objList[2]->diffusetReflection[0] = 0.1;
        objList[2]->diffusetReflection[1] = 1;
        objList[2]->diffusetReflection[2] = 0.1;
        objList[2]->specularReflection[0] = 0.3;
        objList[2]->specularReflection[1] = 0.7;
        objList[2]->specularReflection[2] = 0.3;
        objList[2]->speN = 650;
        
        objList[3]->ambiantReflection[0] = 0.3;
        objList[3]->ambiantReflection[1] = 0.3;
        objList[3]->ambiantReflection[2] = 0.3;
        objList[3]->diffusetReflection[0] = 0.7;
        objList[3]->diffusetReflection[1] = 0.7;
        objList[3]->diffusetReflection[2] = 0.7;
        objList[3]->specularReflection[0] = 0.6;
        objList[3]->specularReflection[1] = 0.6;
        objList[3]->specularReflection[2] = 0.6;
        objList[3]->speN = 500;
        
        objList[4]->ambiantReflection[0] = 1;
        objList[4]->ambiantReflection[1] = 1;
        objList[4]->ambiantReflection[2] = 1;
        objList[4]->diffusetReflection[0] = 1;
        objList[4]->diffusetReflection[1] = 1;
        objList[4]->diffusetReflection[2] = 1;
        objList[4]->specularReflection[0] = 1;
        objList[4]->specularReflection[1] = 1;
        objList[4]->specularReflection[2] = 1;
        objList[4]->speN = 500;
        
        objList[5]->ambiantReflection[0] = 1;
        objList[5]->ambiantReflection[1] = 1;
        objList[5]->ambiantReflection[2] = 1;
        objList[5]->diffusetReflection[0] = 1;
        objList[5]->diffusetReflection[1] = 1;
        objList[5]->diffusetReflection[2] = 1;
        objList[5]->specularReflection[0] = 1;
        objList[5]->specularReflection[1] = 1;
        objList[5]->specularReflection[2] = 1;
        objList[5]->speN = 500;
        
        
    }
}

void keyboard (unsigned char key, int x, int y)
{
    //keys to control scaling - k
    //keys to control rotation - alpha
    //keys to control translation - tx, ty
    switch (key) {
        case 's':
        case 'S':
            sceneNo = (sceneNo + 1 ) % NUM_SCENE;
            if (NUM_OBJECTS == 4) {
                NUM_OBJECTS += 2;
            } else{
                NUM_OBJECTS -= 2;
            }
            setScene(sceneNo);
            renderScene();
            glutPostRedisplay();
            break;
        case 'q':
        case 'Q':
            exit(0);
            
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    cout<<"<<CS3241 Lab 5>>\n\n"<<endl;
    cout << "S to go to next scene" << endl;
    cout << "Q to quit"<<endl;
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (WINWIDTH, WINHEIGHT);
    
    glutCreateWindow ("CS3241 Lab 5: Ray Tracing");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutKeyboardFunc(keyboard);
    
    objList = new RtObject*[NUM_OBJECTS];
    
    setScene(sceneNo);
    renderScene();
    
    glutMainLoop();
    
    for (int i = 0; i < NUM_OBJECTS; i++)
        delete objList[i];
    delete[] objList;
    
    delete[] pixelBuffer;
    
    return 0;
}
