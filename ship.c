#include "ship.h"

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

#include "guLib/glUtils.h"

#define SHIP_L 20.
#define SHIP_H 4.
#define SHIP_P 40.

void shipSetPosition(ship *s, float x, float y, float z) {
  s->x = x;
  s->y = y;
  s->z = z;
}

void shipDraw(ship *s) {
  glPushMatrix();
  glTranslatef(s->x, s->y, s->z);
  gu_drawPara(SHIP_L, SHIP_H, SHIP_P);
  glPopMatrix();
}
