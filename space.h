#ifndef SPACE_T
#define SPACE_T

#include <GL/gl.h>

typedef struct {
  float x;
  float y;
  float z;
  float radius;
  float xAngle;
  float yAngle;
  GLuint textureDesc;
  int texRepeatCoeff;
} space_t;

void initSpace(space_t *s, float x, float y, float z, float rad,
                           float xAngle, float yAngle,
                           GLuint texture, int texRepeatCoeff);

void drawSpace(space_t *s);

void moveSpace(space_t *p, float x, float y, float z);

#endif
