#include "ship.h"

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

#include "guLib/glUtils.h"

#define VERTICES_NB 23

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
#define I 8
#define J 9
#define K 10
#define L 11
#define M 12
#define N 13
#define O 14
#define P 15
#define Q 16
#define R 17
#define S 18
#define T 19
#define U 20
#define V 21
#define W 22

void __setVertex(ship_t *s, int vertex, float x, float y, float z) {
  s->vertices[vertex][0] = x;
  s->vertices[vertex][1] = y;
  s->vertices[vertex][2] = z;
}

void initShip(ship_t *s, float x, float y, float z) {
  s->x = x;
  s->y = y;
  s->z = z;
  rotateShip(s, 0, 0, 0);
  __setVertex(s, A, 7., -.75, 0.); /*A*/
  __setVertex(s, B, 7., .75, 0.); /*B*/
  __setVertex(s, C, 6.75, .5, .5); /*C*/
  __setVertex(s, D, 6.75, -.5, .5); /*D*/
  __setVertex(s, E, 6.75, 0., .5); /*E*/
  __setVertex(s, F, 5.5, 0., 1.); /*F*/
  __setVertex(s, G, 2.5, 2.5, .5); /*G*/
  __setVertex(s, H, 2.5, -2.5, .5); /*H*/
  __setVertex(s, I, 1.75, -2., 1.5); /*I*/
  __setVertex(s, J, 3., 0., 1.5); /*J*/
  __setVertex(s, K, 1.75, 2., 1.5); /*K*/
  __setVertex(s, L, -4., -2, 1.5); /*L*/
  __setVertex(s, M, -4., 2., 1.5); /*M*/
  __setVertex(s, N, -5.5, -2.5, .5); /*N*/
  __setVertex(s, O, -5.5, 2.5, .5); /*O*/
  __setVertex(s, P, -6., -1., .5); /*P*/
  __setVertex(s, Q, -6., 1., .5); /*Q*/
  __setVertex(s, R, 2.5, -5., -.5); /*R*/
  __setVertex(s, S, 2.5, 5., -.5); /*S*/
  __setVertex(s, T, 2., -5., -.5); /*T*/
  __setVertex(s, U, 2., 5., -.5); /*U*/
  __setVertex(s, V, -4.5, -3.5, -.5); /*V*/
  __setVertex(s, W, -4.5, 3.5, -.5); /*W*/
}

void setShipPosition(ship_t *s, float x, float y, float z) {
  s->x = x;
  s->y = y;
  s->z = z;
}

void __drawVertex(ship_t *s, int vert) {
  glVertex3f(s->vertices[vert][0], s->vertices[vert][1], s->vertices[vert][2]);
}
void drawShip(ship_t *s) {
  glDisable(GL_CULL_FACE);
  glPushMatrix();
  glTranslatef(s->x, s->y, s->z);
  glRotatef(s->xRotation, 1., 0., 0.);
  glRotatef(s->yRotation, 0., 1., 0.);
  glRotatef(s->zRotation, 0., 0., 1.);

  /*glBegin(GL_QUADS); {
    glVertex2f(-1, 1);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 1);
  }
  glEnd();*/
  
  GLfloat diff[4] = {.8, .8, .8, 1};
  GLfloat amb[4] = {.2, .2, .2, 1};
  int specCoeff = 0;
  GLfloat spec[4] = {specCoeff, specCoeff, specCoeff, 1};
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 80);
  
  glBegin(GL_QUADS); {
    vector_t norm;
    
    __drawVertex(s, D);
    __drawVertex(s, E);
    __drawVertex(s, F);
    __drawVertex(s, H);
    
    __drawVertex(s, E);
    __drawVertex(s, C);
    __drawVertex(s, G);
    __drawVertex(s, F);
  }
  glEnd();
  
  glBegin(GL_POLYGON);{
    __drawVertex(s, J);
    __drawVertex(s, K);
    __drawVertex(s, M);
    __drawVertex(s, L);
    __drawVertex(s, I);
  }glEnd();
  glEnable(GL_CULL_FACE);
  glPopMatrix();
}

void rotateShip(ship_t* s, float x, float y, float z) {
  s->xRotation += x;
  s->yRotation += y;
  s->zRotation += z;
}
