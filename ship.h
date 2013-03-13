#ifndef SHIP
#define SHIP

typedef struct {
  float x;
  float y;
  float z;
} ship;

void shipSetPosition(ship *s, float x, float y, float z);

void shipDraw(ship *s);

#endif
