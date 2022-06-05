#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
using namespace std;

#define MAX_DRAW_MODE 20
#define MAX_CONTROL_POINTS 100
#define HEAD_HIGHT 1
#define ARM_HIGHT -.7


int cpX[MAX_CONTROL_POINTS];
int cpY[MAX_CONTROL_POINTS];

enum MODE {ROTATE, ROTATE1, ROTATE2};
static float ex = 0.0;
static float ey = 0.0;
static float ez = 0.0;
float input1, input2, input3, input4, input5, input6 = 0;
static float rotate[3] = {0, 0, 0};
static float rotatea[3] = {0, 0, 0};
static float scale[3] = {0.5, 0.5, 0.5};
static float translate[3] = {0, 0, 0};
static float axisofX[6] = {3, 4, 2, 2, 2, 3};
static float axisofY[6] = {-1, 0, 1, 2, 3, 4};
static float fov = 60.0;
static MODE mode = ROTATE;
float increment = 0.0f;
int* locatSide;
int* locatUp;
const int faces = 100;
static float rotationalangleAC[faces];
static float rotationalangleHI[faces];
static float rotationalangleACU[faces];
static float rotationalangleHIU[faces];
static int theCurveAngle = 10;
static float variableangle = static_cast<float>(theCurveAngle);
static float valueSet = 1/variableangle;
int switcher = 0;

static float WIDTH = 1000.0;
static float HEIGHT = 1000.0;

void init(void){
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glColor3f(1.0f, 0.0f, 0.0f);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
}

float buildSet(float locatSide[], float m, int n){
  int sectionSize = n;
  if(sectionSize>1){
    float sectionout[sectionSize- 1];
    for(int b = 0; b < sectionSize - 1; b++){
      float variableOne = (1 - m)*locatSide[b];
      float variableTwo = m * locatSide[b + 1];
      sectionout[b] = variableOne + variableTwo;
    }
    return buildSet(sectionout, m , n-1);
  }
  else if(sectionSize<= 1){
    return locatSide[0];
  }
  else{
    return locatSide[0];
  }
}

void creationOfbez(float locatSide[], float locatUp[]){
  for (float u = 0.0; u < 1.0; u = u+valueSet){
    glBegin(GL_QUADS);
       glVertex3f( buildSet(axisofX, u, 6) + 0.01, buildSet(axisofY, u, 6) + 0.01, 0);
       glVertex3f( buildSet(axisofX, u, 6) - 0.01, buildSet(axisofY, u, 6) - 0.01, 0);
       glVertex3f( buildSet(axisofX, u, 6) - 0.01, buildSet(axisofY, u, 6) + 0.01, 0);
       glVertex3f( buildSet(axisofX, u, 6) + 0.01, buildSet(axisofY, u, 6) - 0.01, 0);
    glEnd();
  }
}

void BezierVase(){
  float sectpval[theCurveAngle];
  float sectuval[theCurveAngle];
  int p = 0;

  for(float k = 0.0; k < 1.0; k = k + valueSet){
    sectpval[p] = buildSet(axisofX, k, 6);
    sectuval[p] = buildSet(axisofY, k, 6);
    p += 1;
  }

  for(int v = 0; v < faces; v++){
    float degreeofrot = v * 2 * 3.14/faces;
    float xVal = sectpval[0] * cos(degreeofrot + 45 * 3.1415926535/180);
    float zVal = sectpval[0] * sin(degreeofrot + 45 * 3.1415926535/180);
    float iVal = sectpval[1] * cos(degreeofrot + 45 * 3.1415926535/180);
    float jVal = sectpval[1] * sin(degreeofrot + 45 * 3.1415926535/180);
    rotationalangleAC[v] = xVal;
    rotationalangleHI[v] = zVal;
    rotationalangleACU[v] = iVal;
    rotationalangleHIU[v] = jVal;
  }

  glBegin(GL_TRIANGLE_FAN);
  for(int k = 0; k < faces; k++){
    glVertex3f(rotationalangleAC[k], -1, rotationalangleHI[k]);
  }
  glEnd();

  for(int vinc = 0; vinc < theCurveAngle-1; vinc++){

    glBegin(GL_QUADS);

    for(int hinc = 0; hinc < faces-1; hinc++){

      glColor3f(1 , 0, 1);

      for(int z = 0; z < faces; z++){
        float degreeofrot = z * 2 * 3.14/faces;
        float xVal = sectpval[vinc] * cos(degreeofrot + 45 * 3.1415926535/180);
        float zVal = sectpval[vinc] * sin(degreeofrot + 45 * 3.1415926535/180);
        float iVal = sectpval[vinc+1] * cos(degreeofrot + 45 * 3.1415926535/180);
        float jVal = sectpval[vinc+1] * sin(degreeofrot + 45 * 3.1415926535/180);
        rotationalangleAC[z] = xVal;
        rotationalangleHI[z] = zVal;
        rotationalangleACU[z] = iVal;
        rotationalangleHIU[z] = jVal;
      }
      float arto = rotationalangleHI[hinc];
      float artq = rotationalangleHI[hinc+1];
      float artn = rotationalangleHIU[hinc];
      float artp = rotationalangleHIU[hinc+1];
      float artv = rotationalangleAC[hinc+1];
      float artt = rotationalangleAC[hinc];
      float arty = rotationalangleACU[hinc];
      float art = rotationalangleACU[hinc+1];
      float artg = sectuval[vinc+1];
      float artd = sectuval[vinc];

      glVertex3f(art,artg,artp);
      glVertex3f(artv,artd,artq);
      glVertex3f(artt,artd,arto);
      glVertex3f(arty,artg,artn);

    }
      float drawt = sectuval[vinc];
      float drawy = sectuval[vinc+1];
      float drawg = rotationalangleAC[0];
      float drawe = rotationalangleAC[faces-1];
      float drawf = rotationalangleACU[0];
      float drawm = rotationalangleACU[faces-1];
      float drawj = rotationalangleHI[0];
      float drawh = rotationalangleHI[faces-1];
      float drawp = rotationalangleHIU[0];
      float drawz = rotationalangleHIU[faces-1];

      glVertex3f(drawg,drawt,drawj);
      glVertex3f(drawe, drawt,drawh);
      glVertex3f(drawm,drawy,drawz);
      glVertex3f(drawf,drawy,drawp);
  }
}

void Face(GLfloat A[], GLfloat B[], GLfloat C[], GLfloat D[]){
  glBegin(GL_POLYGON);
  glVertex3fv(A);
  glVertex3fv(B);
  glVertex3fv(C);
  glVertex3fv(D);
  glEnd();
}

void Cube(GLfloat V0[],GLfloat V1[],GLfloat V2[],GLfloat V3[],GLfloat V4[],GLfloat V5[],GLfloat V6[],GLfloat V7[]){
  glColor3f(0.0f, 10.0f, 0.0f);
  Face(V0,V1,V2,V3);

  glColor3f(0.0f, 0.0f, 10.0f);
  Face(V4,V5,V6,V7);

  glColor3f(10.0f, 0.0f, 0.0f);
  Face(V0,V3,V7,V4);

  glColor3f(0.0f, 10.0f, 10.0f);
  Face(V1,V2,V6,V5);

  glColor3f(10.0f, 0.0f, 10.0f);
  Face(V0,V1,V5,V4);

  glColor3f(10.0f, 10.0f, 0.0f);
  Face(V3,V2,V6,V7);
}

void sphere(){

  glTranslatef(0.0, HEAD_HIGHT, 0.0);
  GLUquadric *quad = gluNewQuadric();
  gluSphere(quad, .55, 30, 30);
}

void cylinder(GLfloat reach, GLfloat range){
    glColor3f(4.0f, 0.0f, 10.0f);
    GLfloat rotationalpoint = 0;
    GLfloat orientationofax = 0;
    GLfloat Positionvariable = 0;
    GLfloat faceValue = 0.1;
    float math = 0.0;

    glBegin(GL_QUAD_STRIP);
    Positionvariable = 0;
        while( Positionvariable < 2*3.1415926535 ) {
            rotationalpoint = reach * cos(Positionvariable);
            orientationofax = reach * sin(Positionvariable);
            glVertex3f(rotationalpoint, orientationofax , range);
            glVertex3f(rotationalpoint, orientationofax , math);
            Positionvariable = Positionvariable + faceValue;
        }
        glVertex3f(reach, math, range);
        glVertex3f(reach, math, math);
    glEnd();

    glBegin(GL_POLYGON);
    Positionvariable = 0;
        while( Positionvariable < 2*3.1415926535 ) {
            rotationalpoint = reach * cos(Positionvariable);
            orientationofax = reach * sin(Positionvariable);
            glVertex3f(rotationalpoint, orientationofax , range);
            Positionvariable = Positionvariable + faceValue;
        }
        glVertex3f(reach, math, range);
    glEnd();
}

void leftarmRotate(){
  glRotatef(rotatea[0], 1, 0, 0);
  glRotatef(rotatea[1], 0, 1, 0);
  glRotatef(rotatea[2], 0, 0, 1);
  cylinder(.2,1.0);
}

void display(){
  if(switcher == 0){
  glEnable(GL_DEPTH_TEST);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   glRotatef(rotate[0], 1, 0, 0);
   glRotatef(rotate[1], 0, 1, 0);
   glRotatef(rotate[2], 0, 0, 1);
   glTranslatef(translate[0], translate[1], translate[2]);
   glScalef(scale[0], scale[1], scale[2]);

   BezierVase();
   glPushMatrix();
   glTranslatef(1, 0, 0);
   glColor3f(0.3, 0.8, 0.0);

   creationOfbez(axisofX, axisofY);
   glPopMatrix();
   glutSwapBuffers();
 }
 if(switcher == 1){
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   glRotatef(rotate[0], 1, 0, 0);
   glRotatef(rotate[1], 0, 1, 0);
   glRotatef(rotate[2], 0, 0, 1);
   glTranslatef(translate[0], translate[1], translate[2]);
   glScalef(scale[0], scale[1], scale[2]);

   glBegin(GL_QUADS);

   GLfloat V[8][3] =
   {
     {-0.5, 0.5, 0.5},
     { 0.5, 0.5, 0.5},
     { 0.5,-0.5, 0.5},
     {-0.5,-0.5, 0.5},

     {-0.5, 0.5,-0.5},
     { 0.5, 0.5,-0.5},
     { 0.5,-0.5,-0.5},
     {-0.5,-0.5,-0.5},
   };

   Cube(V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7]);
   sphere();
   glRotatef(90, 0, 1, 0);
   glTranslatef(0, -.7, -1.5);
   cylinder(.2,1.0);
   glRotatef(-90, 0, 1, 0);
   glRotatef(90, 1, 0, 0);
   glTranslatef(1.28, 0, .65);
   cylinder(.2,1.0);
   glTranslatef(.45, 0, 0);
   cylinder(.2,1.0);
   glRotatef(-90, 1, 0, 0);
   glRotatef(90, 0, 1, 0);
   glTranslatef(-.91, 0, -1);
   glTranslatef(.92, .6, 1.2);
   leftarmRotate();
   glEnd();

  glPopMatrix();
  glutSwapBuffers();
 }
}

void Keyboard(unsigned char key, int x, int y){
  switch (key)
  {
    case 'x':
      rotate[0] += 1;
    break;

    case 'y':
      rotate[1] += 1;
    break;

    case 'z':
      rotate[2] += 1;
    break;

    case 'j':
      scale[0] += .01;
    break;

    case 'k':
      scale[1] += .01;
    break;

    case 'l':
      scale[2] += .01;
    break;

    case 'u':
      scale[0] -= .01;
    break;

    case 'i':
      scale[1] -= .01;
    break;

    case 'o':
      scale[2] -= .01;
    break;

    case 'w':
      translate[1] += .01;
    break;

    case 'a':
      translate[0] -= .01;
    break;

    case 's':
      translate[1] -= .01;
    break;

    case 'd':
      translate[0] += .01;
    break;

    case 'n':
      translate[2] += .01;
    break;

    case 'm':
      translate[2] -= .01;
    break;

    case '1':
      switcher = 0;
    break;

    case '2':
      switcher = 1;
    break;

    case '3':
    rotatea[0] -= 1 ;
    break;

    case '4':
    rotatea[1] -= 1 ;
    break;

    case '5':
    rotatea[0] += 1 ;
    break;

    case '6':
    rotatea[1] += 1 ;
    break;

    case ' ':
    for(int i = 0; i < 3; i++){
      rotate[i] = translate[i] = rotatea[i] = 0.0;
      scale[i] = 0.5;
    }
    break;

    case 27:
    case 'Q':
    case 'q':
      exit(0);
    default:
      break;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  display();
  }
  glutPostRedisplay();
}

void campos(int key, int x, int y){
  float mult;
  int flag = 0;
  switch (key)
  {
    case GLUT_KEY_LEFT:
    ex += .01;
    flag = 1.0;
    break;

    case GLUT_KEY_RIGHT:
      ex -= .01;
      flag = 1.0;
    break;

    case GLUT_KEY_UP:
      ey -= .01;
      flag = 1;
    break;

    case GLUT_KEY_DOWN:
      ey += .01;
      flag = 1;
    break;

    case GLUT_KEY_F1:
      ez += .01;
      flag = 1;
    break;

    case GLUT_KEY_F2:
      ez -= .01;
      flag = 1;
    break;

    case GLUT_KEY_F3:
      fov += 1;
      flag = 1;
    break;

    case GLUT_KEY_F4:
      fov -= 1;
      flag = 1;
    break;

    case GLUT_KEY_F11:
      ex = 0.0;
      ey = 0.0;
      ez = 0.0;
      fov = 60.0;
      flag = 1;
      axisofX[0] = 3;
      axisofX[1] = 4;
      axisofX[2] = 2;
      axisofX[3] = 2;
      axisofX[4] = 2;
      axisofX[5] = 3;
    break;

    case GLUT_KEY_F5:
    axisofX[0] += .5 ;
    break;

    case GLUT_KEY_F6:
    axisofX[1] += 1.5 ;
    break;

    case GLUT_KEY_F7:
    axisofX[2] += .5 ;
    break;

    case GLUT_KEY_F8:
    axisofX[3] += .5 ;
    break;

    case GLUT_KEY_F9:
    axisofX[4] += .5 ;
    break;

    case GLUT_KEY_F10:
    axisofX[5] += .5 ;
    break;
  }
  if(flag){
    glViewport(0, 0, (GLsizei) WIDTH, (GLsizei) HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (GLfloat) WIDTH/(GLfloat) HEIGHT, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ex, ey, ez + 5.0, ex, ey, ez, 0.0, 1.0, 0.0);
  }
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  display();
  glutPostRedisplay();
}

void reshape (int w, int h){
    HEIGHT = h;
    WIDTH = w;
   glViewport(0, 0, (GLsizei) WIDTH, (GLsizei) HEIGHT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(fov, (GLfloat) WIDTH/(GLfloat) HEIGHT, 1.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(ex, ey, ez + 5.0, 0.0, ex, ey, ez, 1.0, 0.0);
}

int main(int C, char *V[]){
   glutInit(&C, V);
   glutInitWindowSize (WIDTH, HEIGHT);
   glutInitWindowPosition (250, 50);
   glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutCreateWindow ("Colored Cube");
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(Keyboard);
   glutSpecialFunc(campos);

   glutMainLoop();
   return 0;
}

CC		= g++
C		= cpp
H             = h
CFLAGS        = -Wall -g
LDFLAGS       = -lm

CFLAGS		= -g
LFLAGS		= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lOpenImageIO -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS   = -L /usr/lib64/ -lglut -lGL -lGLU -lOpenImageIO -lm
  endif
endif

all: clean cg04.o vase

vase: cg04.o
		${CC} ${CFLAGS} -o vase cg04.o ${LDFLAGS}

clean:
	rm -rf *.o
	rm -rf vase
