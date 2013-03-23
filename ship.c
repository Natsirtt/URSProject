#include "ship.h"

#include <stdlib.h>
#include <stdio.h>

#include "guLib/glUtils.h"

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
#define C2 23
#define D2 24
#define H2 25
#define N2 26
#define P2 27
#define Q2 28
#define O2 29
#define G2 30

void __shipSetVertex(ship_t *s, int vertex, float x, float y, float z) {
  s->vertices[vertex][0] = x;
  s->vertices[vertex][1] = y;
  s->vertices[vertex][2] = z;
}

void __shipModelize(ship_t *s) {
  __shipSetVertex(s, A, 8., -.75, 0.); /*A*/
  __shipSetVertex(s, B, 8., .75, 0.); /*B*/
  __shipSetVertex(s, C, 6.75, .5, .5); /*C*/
  __shipSetVertex(s, C2, 6.75, .5, 0.); /*C2*/
  __shipSetVertex(s, D, 6.75, -.5, .5); /*D*/
  __shipSetVertex(s, D2, 6.75, -.5, 0.); /*D2*/
  __shipSetVertex(s, E, 6.75, 0., .5); /*E*/
  __shipSetVertex(s, F, 5.5, 0., 1.); /*F*/
  __shipSetVertex(s, G, 2.5, 2.5, .5); /*G*/
  __shipSetVertex(s, G2, 2.5, 2.5, 0.); /*G2*/
  __shipSetVertex(s, H, 2.5, -2.5, .5); /*H*/
  __shipSetVertex(s, H2, 2.5, -2.5, 0.); /*H2*/
  __shipSetVertex(s, I, 1.75, -2., 1.5); /*I*/
  __shipSetVertex(s, J, 3., 0., 1.5); /*J*/
  __shipSetVertex(s, K, 1.75, 2., 1.5); /*K*/
  __shipSetVertex(s, L, -4., -2, 1.5); /*L*/
  __shipSetVertex(s, M, -4., 2., 1.5); /*M*/
  __shipSetVertex(s, N, -5.5, -2.5, .5); /*N*/
  __shipSetVertex(s, N2, -5.5, -2.5, 0.); /*N2*/
  __shipSetVertex(s, O, -5.5, 2.5, .5); /*O*/
  __shipSetVertex(s, O2, -5.5, 2.5, 0.); /*O2*/
  __shipSetVertex(s, P, -6., -1., .5); /*P*/
  __shipSetVertex(s, P2, -6., -1., 0.); /*P2*/
  __shipSetVertex(s, Q, -6., 1., .5); /*Q*/
  __shipSetVertex(s, Q2, -6., 1., 0.); /*Q2*/
  __shipSetVertex(s, R, 2.5, -5., -.5); /*R*/
  __shipSetVertex(s, S, 2.5, 5., -.5); /*S*/
  __shipSetVertex(s, T, 2., -5., -.5); /*T*/
  __shipSetVertex(s, U, 2., 5., -.5); /*U*/
  __shipSetVertex(s, V, -7, -5, -.5); /*V*/
  __shipSetVertex(s, W, -7, 5, -.5); /*W*/
}

void __shipGetVertexVector(vector_t *vBuff, ship_t *s, int vertex) {
  vBuff->x = s->vertices[vertex][0];
  vBuff->y = s->vertices[vertex][1];
  vBuff->z = s->vertices[vertex][2];
}

//Calcule la normale de v2 par rapport à v1 et v3 et la place dans vBuff.
//La normale est normalisée.

void __shipProduceOneNormal(ship_t *s, vector_t *vBuff, int v1, int v2, int v3) {
  vector_t vec1;
  vector_t vec2;
  vector_t vec3;
  __shipGetVertexVector(&vec1, s, v1);
  __shipGetVertexVector(&vec2, s, v2);
  __shipGetVertexVector(&vec3, s, v3);
  gu_normal(vBuff, vec1, vec2, vec3);
  gu_normalizeVector(vBuff);
  //__shipSetNormal(s, v2, &normal);
}

/*void __shipProduceNormals(ship_t *s) {
  
  //TODO : calculer la normale de chaque vertex
  
  __shipProduceOneNormal(s, D, A, B);
  __shipProduceOneNormal(s, A, B, C);
  __shipProduceOneNormal(s, B, C, D);
  __shipProduceOneNormal(s, C, D, A);
  
}*/

void initShip(ship_t *s, float x, float y, float z) {
  s->x = x;
  s->y = y;
  s->z = z;
  rotateShip(s, 0, 0, 0);
  __shipModelize(s);
  //__shipProduceNormals(s);

  //Initialisation des textures
  GLuint texDesc[2];
  gu_initTextures(texDesc, 2, 0, "kineticShield.png", "aluminium.png");

  s->glassTex = texDesc[0];
  s->alumTex = texDesc[1];
}

void setShipPosition(ship_t *s, float x, float y, float z) {
  s->x = x;
  s->y = y;
  s->z = z;
}

void __shipDrawVertex(ship_t *s, int vert) {
  glVertex3f(s->vertices[vert][0], s->vertices[vert][1], s->vertices[vert][2]);
}

void __shipSetNormal(ship_t *s, int v1, int v2, int v3) {
  vector_t normal;
  __shipProduceOneNormal(s, &normal, v3, v2, v1);
  glNormal3f(normal.x, normal.y, normal.z);
}

void __shipDrawTriangle(ship_t *s, int v1, int v2, int v3, float t1, float t2, float t3, float t4, float t5, float t6) {
  __shipSetNormal(s, v1, v2, v3);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(t1, t2);
  __shipDrawVertex(s, v1);
  glTexCoord2f(t3, t4);
  __shipDrawVertex(s, v2);
  glTexCoord2f(t5, t6);
  __shipDrawVertex(s, v3);
  glEnd();
}

void __shipDrawQuad(ship_t *s, int v1, int v2, int v3, int v4, float t1, float t2, float t3, float t4, float t5, float t6, float t7, float t8) {
  __shipSetNormal(s, v1, v2, v3);
  glBegin(GL_QUADS);
  glTexCoord2f(t1, t2);
  __shipDrawVertex(s, v1);
  glTexCoord2f(t3, t4);
  __shipDrawVertex(s, v2);
  glTexCoord2f(t5, t6);
  __shipDrawVertex(s, v3);
  glTexCoord2f(t7, t8);
  __shipDrawVertex(s, v4);
  glEnd();
}

void __shipDrawPolygon(ship_t *s, int totalVertexNb, int v1, int v2, int v3, ...) {
  //Liste des arguments
  va_list args;
  //Initialisation depuis le dernier parametre connu
  va_start(args, v3);

  __shipSetNormal(s, v1, v2, v3);
  glBegin(GL_POLYGON);
  __shipDrawVertex(s, v1);
  __shipDrawVertex(s, v2);
  __shipDrawVertex(s, v3);

  for (int i = 0; i < totalVertexNb - 3; i++) {
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

  GLfloat diff[4] = {.6, .6, .6, 1};
  GLfloat amb[4] = {.2, .2, .2, 1};
  int specCoeff = 1.;
  GLfloat spec[4] = {specCoeff, specCoeff, specCoeff, 1};

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 110);

  //Texture d'alu
  glBindTexture(GL_TEXTURE_2D, s->alumTex);

  //Nez (sous le pare brise)
  __shipDrawTriangle(s, D, E, H, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, E, F, H, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, E, C, G, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, E, G, F, .0, .0, 1., .0, 1., 1.);

  //Pare brise
  glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);

  //Lumières
  GLfloat spec2[4] = {.51, .63, .95, 1.};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, spec2);

  //Textures
  glBindTexture(GL_TEXTURE_2D, s->glassTex);

  __shipDrawTriangle(s, F, I, H, .5, 1., 1., .0, 1., .7);
  __shipDrawTriangle(s, F, J, I, .5, 1., .5, .0, 1., 0.);
  __shipDrawTriangle(s, F, K, J, .5, 1., .0, .0, .5, .0);
  __shipDrawTriangle(s, F, G, K, .5, 1., .0, .7, .0, .0);
  glPopAttrib();

  //Haut
  __shipDrawQuad(s, K, M, L, I, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawTriangle(s, J, K, I, .5, 1., .0, .0, .5, .0);
  //Bas
  //__shipDrawPolygon(s, 7, O, G, E, H, N, P, Q);
  __shipDrawPolygon(s, 8, O2, G2, B, A, H2, N2, P2, Q2);

  //Cotés
  __shipDrawQuad(s, H, I, L, N, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawQuad(s, K, G, O, M, .0, .0, 1., .0, 1., 1., .0, 1.);

  //Arrière
  __shipDrawTriangle(s, L, P, N, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, M, O, Q, .0, .0, 1., .0, 1., 1.);
  __shipDrawQuad(s, L, M, Q, P, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawQuad(s, N, P, P2, N2, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawQuad(s, P, Q, Q2, P2, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawQuad(s, Q, O, O2, Q2, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawTriangle(s, V, N, N2, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, O, W, O2, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, G, G2, U, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, G, S, G2, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, R, H, H2, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, T, H2, H, .0, .0, 1., .0, 1., 1.);
          
  //Ailes
  __shipDrawTriangle(s, R, A, D, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, R, D, H, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, B, S, C, .0, .0, 1., .0, 1., 1.);
  __shipDrawTriangle(s, C, S, G, .0, .0, 1., .0, 1., 1.);
  __shipDrawQuad(s, A, B, C, D, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawQuad(s, G, U, W, O, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawQuad(s, H, N, V, T, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawTriangle(s, R, H2, A, .0, .0, 1., .0, 1., 1.);
  __shipDrawQuad(s, T, V, N2, H2, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawQuad(s, U, G2, O2, W, .0, .0, 1., .0, 1., 1., .0, 1.);
  __shipDrawTriangle(s, S, B, G2, .0, .0, 1., .0, 1., 1.);


  glEnable(GL_CULL_FACE);
  glPopMatrix();
}

void rotateShip(ship_t* s, float x, float y, float z) {
  s->xRotation += x;
  s->yRotation += y;
  s->zRotation += z;
}
