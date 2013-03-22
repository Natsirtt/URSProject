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

void __shipSetVertex(ship_t *s, int vertex, float x, float y, float z) {
  s->vertices[vertex][0] = x;
  s->vertices[vertex][1] = y;
  s->vertices[vertex][2] = z;
}

void __shipSetNormal(ship_t *s, int vertex, vector_t *normal) {
  s->normals[vertex][0] = normal->x;
  s->normals[vertex][1] = normal->y;
  s->normals[vertex][2] = normal->z;
}

void __shipModelize(ship_t *s) {
  __shipSetVertex(s, A, 7., -.75, 0.); /*A*/
  __shipSetVertex(s, B, 7., .75, 0.); /*B*/
  __shipSetVertex(s, C, 6.75, .5, .5); /*C*/
  __shipSetVertex(s, D, 6.75, -.5, .5); /*D*/
  __shipSetVertex(s, E, 6.75, 0., .5); /*E*/
  __shipSetVertex(s, F, 5.5, 0., 1.); /*F*/
  __shipSetVertex(s, G, 2.5, 2.5, .5); /*G*/
  __shipSetVertex(s, H, 2.5, -2.5, .5); /*H*/
  __shipSetVertex(s, I, 1.75, -2., 1.5); /*I*/
  __shipSetVertex(s, J, 3., 0., 1.5); /*J*/
  __shipSetVertex(s, K, 1.75, 2., 1.5); /*K*/
  __shipSetVertex(s, L, -4., -2, 1.5); /*L*/
  __shipSetVertex(s, M, -4., 2., 1.5); /*M*/
  __shipSetVertex(s, N, -5.5, -2.5, .5); /*N*/
  __shipSetVertex(s, O, -5.5, 2.5, .5); /*O*/
  __shipSetVertex(s, P, -6., -1., .5); /*P*/
  __shipSetVertex(s, Q, -6., 1., .5); /*Q*/
  __shipSetVertex(s, R, 2.5, -5., -.5); /*R*/
  __shipSetVertex(s, S, 2.5, 5., -.5); /*S*/
  __shipSetVertex(s, T, 2., -5., -.5); /*T*/
  __shipSetVertex(s, U, 2., 5., -.5); /*U*/
  __shipSetVertex(s, V, -4.5, -3.5, -.5); /*V*/
  __shipSetVertex(s, W, -4.5, 3.5, -.5); /*W*/
}

void __shipGetVertexVector(vector_t *vBuff, ship_t *s, int vertex) {
  vBuff->x = s->vertices[vertex][0];
  vBuff->y = s->vertices[vertex][1];
  vBuff->z = s->vertices[vertex][2];
}

//Calcule et applique la normale de v2 par rapport à v1 et v3
void __shipProduceOneNormal(ship_t *s, int v1, int v2, int v3) {
  vector_t normal;
  vector_t vec1;
  vector_t vec2;
  vector_t vec3;
  __shipGetVertexVector(&vec1, s, v1);
  __shipGetVertexVector(&vec2, s, v2);
  __shipGetVertexVector(&vec3, s, v3);
  gu_normal(&normal, vec1, vec2, vec3);
  __shipSetNormal(s, v2, &normal);
}

void __shipProduceNormals(ship_t *s) {
  
  //TODO : calculer la normale de chaque vertex
  
  __shipProduceOneNormal(s, B, A, D);
  __shipProduceOneNormal(s, A, B, C);
  __shipProduceOneNormal(s, B, C, E);
  __shipProduceOneNormal(s, A, D, E);
  __shipProduceOneNormal(s, C, E, F);
  __shipProduceOneNormal(s, E, F, H);
  __shipProduceOneNormal(s, F, G, C);
  __shipProduceOneNormal(s, D, H, F);
}

void initShip(ship_t *s, float x, float y, float z) {
  s->x = x;
  s->y = y;
  s->z = z;
  rotateShip(s, 0, 0, 0);
  __shipModelize(s);
  __shipProduceNormals(s);
}

void setShipPosition(ship_t *s, float x, float y, float z) {
  s->x = x;
  s->y = y;
  s->z = z;
}

void __shipDrawVertex(ship_t *s, int vert) {
  glNormal3f(s->normals[vert][0], s->normals[vert][1], s->normals[vert][2]);
  glVertex3f(s->vertices[vert][0], s->vertices[vert][1], s->vertices[vert][2]);
}

void __shipDrawTriangle(ship_t *s, int v1, int v2, int v3) {
  glBegin(GL_TRIANGLES);
  __shipDrawVertex(s, v1);
  __shipDrawVertex(s, v2);
  __shipDrawVertex(s, v3);
  glEnd();
}

void __shipDrawQuad(ship_t *s, int v1, int v2, int v3, int v4) {
  glBegin(GL_QUADS);
  __shipDrawVertex(s, v1);
  __shipDrawVertex(s, v2);
  __shipDrawVertex(s, v3);
  __shipDrawVertex(s, v4);
  glEnd();
}

void __shipDrawPolygon(ship_t *s, int totalVertexNb, int v, ...) {
  //Liste des arguments
  va_list args;
  //Initialisation depuis le dernier parametre connu
  va_start(args, v);

  glBegin(GL_POLYGON);
  __shipDrawVertex(s, v);

  for (int i = 0; i < totalVertexNb - 1; i++) {
    __shipDrawVertex(s, va_arg(args, int));
  }
  glEnd();

  va_end(args);
}

void drawShip(ship_t *s) {
  glDisable(GL_CULL_FACE);
  glPushMatrix();
  glTranslatef(s->x, s->y, s->z);
  glRotatef(s->xRotation, 1., 0., 0.);
  glRotatef(s->yRotation, 0., 1., 0.);
  glRotatef(s->zRotation, 0., 0., 1.);

  GLfloat diff[4] = {.8, .8, .8, 1};
  GLfloat amb[4] = {.2, .2, .2, 1};
  int specCoeff = 0;
  GLfloat spec[4] = {specCoeff, specCoeff, specCoeff, 1};

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 80);

  //Nez (sous le pare brise)
  __shipDrawQuad(s, D, E, F, H);
  __shipDrawQuad(s, E, C, G, F);
  
  //Pare brise
  __shipDrawQuad(s, H, F, J, I);
  __shipDrawQuad(s, F, G, K, J);
  
  //Haut
  __shipDrawPolygon(s, 5, J, K, M, L, I);
  //Bas
  __shipDrawQuad(s, D, C, G, H);
  __shipDrawPolygon(s, 6, H, G, O, Q, P, N);
  
  //Cotés
  __shipDrawQuad(s, H, I, L, N);
  __shipDrawQuad(s, K, G, O, M);
  
  //Arrière
  __shipDrawTriangle(s, L, P, N);
  __shipDrawTriangle(s, M, O, Q);
  __shipDrawQuad(s, L, M, Q, P);
  
  //Ailes
  __shipDrawQuad(s, H, D, A, R);
  __shipDrawQuad(s, A, B, C, D);
  __shipDrawQuad(s, B, S, G, C);
  __shipDrawQuad(s, G, U, W, O);
  __shipDrawQuad(s, H, N, V, T);
  
  glEnable(GL_CULL_FACE);
  glPopMatrix();
}

void rotateShip(ship_t* s, float x, float y, float z) {
  s->xRotation += x;
  s->yRotation += y;
  s->zRotation += z;
}
