#ifndef SHIP
#define SHIP

#include <GL/gl.h>

#include "guLib/glUtils.h"

#define VERTICES_NB 39

typedef struct {
  vector_t pos;
  vector_t forward;
  vector_t up;
  float speed;
  float vertices[VERTICES_NB][3];
  GLuint glassTex;
  GLuint alumTex;
  //float normals[23][3];
} ship_t;

void initShip(ship_t *s, float x, float y, float z);

void setShipPosition(ship_t* s, float posx, float posy, float posz,
                                float forwardx, float forwardy, float forwardz,
                                float upx, float upy, float upz);

void shipForward(ship_t *s, float step);

void shipLookAt(const ship_t *s);

void drawShip(ship_t *s);

void rotateShip(ship_t* s, float xAngle, float yAngle, float zAngle);

void changeShipSpeed(ship_t *s, float newSpeed);

#endif
