#include "sun.h"
#include "guLib/glUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <math.h>

void initSun(sun_t *p, float x, float y, float z, float rad,
                           float xAngle, float yAngle, float rotSpeed,
                           GLuint texture) {
  p->x = x;
  p->y = y;
  p->z = z;
  p->radius = rad;
  p->xAngle = xAngle;
  p->yAngle = yAngle;
  p->rotationSpeed = rotSpeed;
  p->textureDesc = texture;
}

void _sunCycle(sun_t *s) {
  s->rotation += s->rotationSpeed;
  if (s->rotation >= 360) {
    s->rotation = 0;
  }
}

void drawSun(sun_t *s) {
  _sunCycle(s);
  
  glPushMatrix();
  
  glTranslatef(s->x, s->y, s->z);
  
  gu_xRotate(s->xAngle);
  gu_yRotate(s->yAngle);
  gu_zRotate(s->rotation);
  
  glBindTexture(GL_TEXTURE_2D, s->textureDesc);
  
  int sliceAndStack;
  if (s->radius <= 1000) {
    sliceAndStack = 32;
  } else {
    sliceAndStack = 64;
  }
  
  GLfloat emission[4] = {1, 1, 1, 1};
  GLfloat ambAndDiffAndSpec[4] = {0, 0, 0, 1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambAndDiffAndSpec);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambAndDiffAndSpec);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ambAndDiffAndSpec);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);
  glMaterialfv(GL_FRONT, GL_EMISSION, emission);
  gu_drawSphere(s->radius, sliceAndStack, 1, 1);
  glMaterialfv(GL_FRONT, GL_EMISSION, ambAndDiffAndSpec); //On remet à 0
  
  glPopMatrix();
  
  //On indique qu'on remet la texture par défaut (rien)
  glBindTexture(GL_TEXTURE_2D, 0);
}

void moveSun(sun_t *s, float x, float y, float z) {
  s->x = x;
  s->y = y;
  s->z = z;
}

