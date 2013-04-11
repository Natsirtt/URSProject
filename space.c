#include "space.h"
#include "guLib/glUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <math.h>

void initSpace(space_t *s, float x, float y, float z, float rad,
                           float xAngle, float yAngle,
                           GLuint texture, int texRepeatCoeff) {
  s->x = x;
  s->y = y;
  s->z = z;
  s->radius = rad;
  s->xAngle = xAngle;
  s->yAngle = yAngle;
  s->textureDesc = texture;
  s->texRepeatCoeff = texRepeatCoeff;
}

void drawSpace(space_t *s) {
  //Désactivation du depth tests et de l'écriture dans le depth buffer pour la skybox
//  glDisable(GL_DEPTH_TEST);
//  glDepthMask(GL_FALSE);
  glDisable(GL_CULL_FACE);
  
  glPushMatrix();
  
  glTranslatef(s->x, s->y, s->z);
  
  gu_xRotate(s->xAngle);
  gu_yRotate(s->yAngle);
  
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glScalef(s->texRepeatCoeff, s->texRepeatCoeff, 0);
  glBindTexture(GL_TEXTURE_2D, s->textureDesc);
  glMatrixMode(GL_MODELVIEW);
  
  GLfloat emission[4] = {.7, .7, .7, .7};
  GLfloat ambAndDiffandSpec[4] = {0, 0, 0, 1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambAndDiffandSpec);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambAndDiffandSpec);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ambAndDiffandSpec);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);
  glMaterialfv(GL_FRONT, GL_EMISSION, emission);
  gu_drawSphere(s->radius, 16, 1, 0);
  glMaterialfv(GL_FRONT, GL_EMISSION, ambAndDiffandSpec); //Remise à 0
  
  glMatrixMode(GL_TEXTURE);
  glPopMatrix();
  
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  
  //On indique qu'on remet la texture par défaut (rien)
  glBindTexture(GL_TEXTURE_2D, 0);
  //Réactivation du depth buffer
//  glEnable(GL_DEPTH_TEST);
//  glDepthMask(GL_TRUE);
  glEnable(GL_CULL_FACE);
}

void moveSpace(space_t *p, float x, float y, float z) {
  p->x = x;
  p->y = y;
  p->z = z;
}
