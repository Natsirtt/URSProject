#include "planet.h"
#include "guLib/glUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <math.h>

void initPlanet(planet *p, float x, float y, float z, float rad,
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

void _planetCycle(planet *p) {
  p->rotation += p->rotationSpeed;
  if (p->rotation >= 360) {
    p->rotation = 0;
  }
}

void drawPlanet(planet *p) {
  _planetCycle(p);
  
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

void movePlanet(planet *p, float x, float y, float z) {
  p->x = x;
  p->y = y;
  p->z = z;
}

float _sqr(float x) {
  return x * x;
}

float distanceToPlanet(planet *p, float x, float y, float z) {
  return sqrt(_sqr(p->x - x) + _sqr(p->y - y) + _sqr(p->z - z));
}

int goToPlanet(planet *p, camCamera *cam, float speed) {
  camVector newVector;
  newVector.x = cam->eye.x;
  newVector.y = cam->eye.y;
  newVector.z = cam->eye.z;
  
  camVector newLookingVector;
  newLookingVector.x = cam->forward.x;
  newLookingVector.y = cam->forward.y;
  newLookingVector.z = cam->forward.z;
  
  //Regard
  if (p->x <= newLookingVector.x) {
    newLookingVector.x += cam->sensibilite;
  } else if (p->x >= newLookingVector.x) {
    newLookingVector.x -= cam->sensibilite;
  }
  
  if (p->y <= newLookingVector.y) {
    newLookingVector.y += cam->sensibilite;
  } else if (p->y >= newLookingVector.y) {
    newLookingVector.y -= cam->sensibilite;
  }
  
  if (p->z <= newLookingVector.z) {
    newLookingVector.z += cam->sensibilite;
  } else if (p->z >= newLookingVector.z) {
    newLookingVector.z -= cam->sensibilite;
  }
  
  //Déplacement
  if (p->x > newVector.x) {
    newVector.x += speed;
  } else if (p->x < newVector.x) {
    newVector.x -= speed;
  }
  
  if (p->y > newVector.y) {
    newVector.y += speed;
  } else if (p->y < newVector.y) {
    newVector.y -= speed;
  }
  
  if (p->z > newVector.z) {
    newVector.z += speed;
  } else if (p->z < newVector.z) {
    newVector.z -= speed;
  }
  
  camFixePositionWithoutUp(cam, newVector.x, newVector.y, newVector.z,
                           newLookingVector.x, newLookingVector.y, newLookingVector.z);
  camLookAt(cam);
  
  if (distanceToPlanet(p, newVector.x, newVector.y, newVector.z) <= p->radius + 1000) {
    return 0;
  }
  return 1;
}
