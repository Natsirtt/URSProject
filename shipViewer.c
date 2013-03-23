/**
 * A simple OpenGL program to display a ship. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <GL/glu.h>

#include "guLib/glUtils.h"
#include "guLib/sdlKeyUtils.h"
#include "ship.h"
#include "guLib/camera.h"
#include "sun.h"
#include "space.h"

keyBinder kb;
camCamera camera;
int continuer = 0;
int doMotion = 0;
float speed = 5.;
float rotSpeed = 2.;
//Acteurs
space_t space;
ship_t ship;
sun_t sun;

void reinitCam() {
  camFixePosition(&camera, 0, 0, 50, 0, 0, 0, 0, 1, 0);
  camLookAt(&camera);
}

void display() {
  GLfloat lightPos[4] = {sun.x, sun.y, sun.z, 1};
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  moveSpace(&space, camera.eye.x, camera.eye.y, camera.eye.z);
  drawSpace(&space);
  drawShip(&ship);
  drawSun(&sun);
}

void zoom() {
  camAvance(&camera, speed);
  camLookAt(&camera);
}

void dezoom() {
  camAvance(&camera, -speed);
  camLookAt(&camera);
}

void left() {
  camPasDeCote(&camera, -speed);
  camLookAt(&camera);
}

void right() {
  camPasDeCote(&camera, speed);
  camLookAt(&camera);
}

void rotateUp() {
  rotateShip(&ship, -rotSpeed, 0., 0.);
}

void rotateDown() {
  rotateShip(&ship, rotSpeed, 0., 0.);
}

void rotateLeft() {
  rotateShip(&ship, 0., 0., -rotSpeed);
}

void rotateRight() {
  rotateShip(&ship, 0., 0., rotSpeed);
}

void motion(SDL_MouseMotionEvent motion) {
  //fprintf(stderr, "debug: motion()\n");

  // Le WarpMouse(320, 320) génère également un évènement
  // motion qu'il ne faut pas prendre en compte.
  if (doMotion && (motion.x != 320 || motion.y != 320)) {
    camTourneEtLeve(&camera, motion.xrel, motion.yrel);
    camLookAt(&camera);
    SDL_WarpMouse(320, 320);
  } else {
    // La première fois xrel et yrel ne sont pas pertinents,
    // on ne déplace donc pas la caméra
    doMotion = 1;
  }
}

void eventCatcher(SDL_Event *event) {
  switch (event->type) {
    case SDL_KEYDOWN:
      skuKeyPressed(&kb, event->key.keysym.sym);
      continuer = !skuIsPressed(&kb, SDLK_ESCAPE);
      break;
    case SDL_KEYUP:
      skuKeyReleased(&kb, event->key.keysym.sym);
      break;
    case SDL_QUIT:
      continuer = 0;
    case SDL_MOUSEMOTION:
    {
      SDL_Event event_temp;
      // On récupère le dernier des évènements motion de la liste de évènements,
      // au cas où la machine ne soit pas assez rapide pour traiter le déplacement
      // de la souris assez vite.
      while (SDL_PeepEvents(&event_temp, 1, SDL_PEEKEVENT, SDL_MOUSEMOTIONMASK) > 0)
        SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK);
      motion(event->motion);
      break;
    }
      break;
  }
}

int main(int argc, char *argv[]) {
  SDL_Surface *surface = gu_init_SDL("URS Ship Viewer");
  gu_init_GL();
  gu_init_display(display);


  skuInitKeyBinder(&kb);
  skuBindKeyHandler(&kb, SDLK_BACKSPACE, reinitCam);
  skuBindKeyHandler(&kb, SDLK_UP, rotateUp);
  skuBindKeyHandler(&kb, SDLK_DOWN, rotateDown);
  skuBindKeyHandler(&kb, SDLK_LEFT, rotateLeft);
  skuBindKeyHandler(&kb, SDLK_RIGHT, rotateRight);
  skuBindKeyHandler(&kb, SDLK_z, zoom);
  skuBindKeyHandler(&kb, SDLK_s, dezoom);
  skuBindKeyHandler(&kb, SDLK_q, left);
  skuBindKeyHandler(&kb, SDLK_d, right);

  camInit(&camera);

  gu_initLights();

  GLfloat whiteLight[4] = {1, 1, 1, 1};
  GLfloat blackLight[4] = {0, 0, 0, 1};
  glLightfv(GL_LIGHT0, GL_AMBIENT, blackLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);

  glMatrixMode(GL_PROJECTION);
  gluPerspective(50, (float) surface->w / surface->h, 1, 10000);
  glMatrixMode(GL_MODELVIEW);
  reinitCam();

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  initShip(&ship, 0, 0, 0);
  initSun(&sun, 0, 30, 30, 3, 0, 0, 0, 0);
  GLuint texDesc;
  gu_initTextures(&texDesc, 1, 1, "space.png");
  initSpace(&space, 0, 0, 0, 10, 0, 0, texDesc, 10);

  continuer = 1;
  SDL_Event event;

  Uint32 startTime;
  Uint32 ellapsedTime = 0;
  int frameNb = 0;

  //Boucle principale
  while (continuer) {

    //Les FPS
    startTime = SDL_GetTicks();
    if (ellapsedTime >= 1000) {
      printf("\r~%d fps", frameNb);
      fflush(stdout);
      ellapsedTime = 0;
      frameNb = 0;
    }

    //Les events
    while (SDL_PollEvent(&event)) {
      eventCatcher(&event);
    }
    skuHandle(&kb);
    gu_display();
    frameNb++;

    ellapsedTime += SDL_GetTicks() - startTime;
  }
  //C'est finit, libération des ressources
  printf("\n");
  gu_SDLQuit(1);
  exit(EXIT_SUCCESS);
}
