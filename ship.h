#ifndef SHIP
#define SHIP

typedef struct {
  float x;
  float y;
  float z;
  float xRotation;
  float yRotation;
  float zRotation;
  float vertices[23][3];
  float normals[23][3];
} ship_t;

void initShip(ship_t *s, float x, float y, float z);

void setShipPosition(ship_t* s, float x, float y, float z);

void drawShip(ship_t *s);

void rotateShip(ship_t *s, float x, float y, float z);

#endif
