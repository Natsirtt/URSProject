#include "planet.h"
#include "guLib/glUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <math.h>

void initPlanet(planet_t *p, float x, float y, float z, float rad,
                           float xAngle, float yAngle, float rotSpeed,
                           GLint shininess, GLuint texture) {
  p->x = x;
  p->y = y;
  p->z = z;
  p->radius = rad;
  p->xAngle = xAngle;
  p->yAngle = yAngle;
  p->rotationSpeed = rotSpeed;
  p->textureDesc = texture;
  p->shininess = shininess;
}

void __planetCycle(planet_t *p) {
  p->rotation += p->rotationSpeed;
  if (p->rotation >= 360) {
    p->rotation = 0;
  }
}

void drawPlanet(planet_t *p) {
  __planetCycle(p);
  
  glPushMatrix();
  //glLoadIdentity();
  
  glTranslatef(p->x, p->y, p->z);
  
  gu_xRotate(p->xAngle);
  gu_yRotate(p->yAngle);
  gu_zRotate(p->rotation);
  
  
  glBindTexture(GL_TEXTURE_2D, p->textureDesc);
  
  int sliceAndStack;
  if (p->radius <= 1000) {
    sliceAndStack = 32;
  } else {
    sliceAndStack = 64;
  }
  
  GLfloat diff[4] = {.8, .8, .8, 1};
  int specCoeff = 0;
  if (p->shininess > 0) {
    specCoeff = 1;
  }
  GLfloat spec[4] = {specCoeff, specCoeff, specCoeff, 1};
  GLfloat amb[4] = {.2, .2, .2, 1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, p->shininess);
  gu_drawSphere(p->radius, sliceAndStack, 1, 1);
  
  glPopMatrix();
  
  //On indique qu'on remet la texture par défaut (rien)
  glBindTexture(GL_TEXTURE_2D, 0);
}

void movePlanet(planet_t *p, float x, float y, float z) {
  p->x = x;
  p->y = y;
  p->z = z;
}

float __sqr(float x) {
  return x * x;
}

float distanceToPlanet(planet_t *p, float x, float y, float z) {
  return sqrt(__sqr(p->x - x) + __sqr(p->y - y) + __sqr(p->z - z));
}
