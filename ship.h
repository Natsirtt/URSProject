#ifndef SHIP
#define SHIP

#include <GL/gl.h>

#define VERTICES_NB 39

typedef struct {
  float x;
  float y;
  float z;
  float speed;
  float xRotation;
  float yRotation;
  float zRotation;
  float vertices[VERTICES_NB][3];
  GLuint glassTex;
  GLuint alumTex;
  //float normals[23][3];
} ship_t;

void initShip(ship_t *s, float x, float y, float z);

void setShipPosition(ship_t* s, float x, float y, float z);

void drawShip(ship_t *s);

void rotateShip(ship_t *s, float x, float y, float z);

void setShipSpeed(ship_t *s, float speed);

float getShipSpeed(ship_t *s);

void shipLifeCycle(ship_t *s);

#endif
