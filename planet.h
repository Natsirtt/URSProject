#ifndef PLANET_TYPE
#define PLANET_TYPE

#include <GL/gl.h>
#include "guLib/camera.h"

typedef struct PLANET {
  float x;
  float y;
  float z;
  float radius;
  float xAngle;
  float yAngle;
  float rotationSpeed;
  float rotation;
  GLint shininess;
  GLuint textureDesc;
} planet;

void initPlanet(planet *p, float x, float y, float z, float rad,
                           float xAngle, float yAngle, float rotSpeed,
                           GLint shininess, GLuint texture);

void movePlanet(planet *p, float x, float y, float z);

void drawPlanet(planet *p);

int goToPlanet(planet *p, camCamera *cam, float speed);

#endif
