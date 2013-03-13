#ifndef SUN_T
#define SUN_T

#include <GL/gl.h>

typedef struct {
  float x;
  float y;
  float z;
  float radius;
  float xAngle;
  float yAngle;
  float rotationSpeed;
  float rotation;
  GLuint textureDesc;
} sun_t;

void initSun(sun_t *p, float x, float y, float z, float rad,
                           float xAngle, float yAngle, float rotSpeed,
                           GLuint texture);

void _sunCycle(sun_t *s);

void drawSun(sun_t *s);

void moveSun(sun_t *s, float x, float y, float z);

#endif
