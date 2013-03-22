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
} planet_t;

void initPlanet(planet_t *p, float x, float y, float z, float rad,
                           float xAngle, float yAngle, float rotSpeed,
                           GLint shininess, GLuint texture);

void movePlanet(planet_t *p, float x, float y, float z);

void drawPlanet(planet_t *p);

int goToPlanet(planet_t *p, camCamera *cam, float speed);

#endif
