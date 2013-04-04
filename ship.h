#ifndef SHIP
#define SHIP

#include <GL/gl.h>

#include "guLib/glUtils.h"

#define VERTICES_NB 39

typedef struct {
  float x;
  float y;
  float z;
  float speed;
  float xRot;
  float yRot;
  float zRot;
  float vertices[VERTICES_NB][3];
  GLuint glassTex;
  GLuint alumTex;
  //float normals[23][3];
} ship_t;

void initShip(ship_t *s, float x, float y, float z);

void setShipPosition(ship_t* s, float x, float y, float z);

void drawShip(ship_t *s);

void rotateShip(ship_t* s, float xAngle, float yAngle, float zAngle);

void changeShipSpeed(ship_t *s, float newSpeed);

#endif
