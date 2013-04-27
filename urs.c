#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "guLib/glUtils.h"
#include "guLib/sdlKeyUtils.h"
#include "guLib/camera.h"

#include "planet.h"
#include "sun.h"
#include "space.h"
#include "ship.h"

#define abs_val(a) (((a)>0)?(a):(-a))
#define sign(a) (((a)>0)?(1):(-1))

keyBinder kb;
camCamera camera;
planet_t planet1;
planet_t planet2;
planet_t moon;
planet_t autoPilotTarget;
space_t spaceSphere;
sun_t sun;
ship_t ship;
float maxSpeed;
int autoPilot = 0;

int continuer;
float rotationAngle = 0.;
int doMotion = 0;

void progQuit() {
  continuer = 0;
}

void accelerate();
void decelerate();

void handleSpeed() {
  if (skuIsPressed(&kb, SDLK_z)) {
    accelerate();
    if (getShipSpeed(&ship) > maxSpeed) {
      decelerate();
    }
  } else {
    decelerate();
  }
  shipLifeCycle(&ship);
}

void accelerate() {
  float shipSpeed = getShipSpeed(&ship);
  if (shipSpeed < maxSpeed) {
    setShipSpeed(&ship, shipSpeed * 1.1);
  }
}

void decelerate() {
  float shipSpeed = getShipSpeed(&ship);
  if (shipSpeed > 0.) {
    setShipSpeed(&ship, shipSpeed / 1.1);
  }
}

void increaseSpeed() {
  if (maxSpeed == 0.) {
    maxSpeed = 1.;
  }
  maxSpeed *= 1.1;
}

void decreaseSpeed() {
  if (maxSpeed <= 1.) {
    maxSpeed = 0.;
  }
  maxSpeed /= 1.1;
}

void camLookAtShip() {
  camFixePositionWithoutUp(&camera, ship.x, ship.y, ship.z + 30, camera.forward.x, camera.forward.y, camera.forward.z);
  camLookAt(&camera);
}

void reinitCam() {
  camFixePosition(&camera, 0, 0, 1010000, 0, 0, 0, 0, 1, 0);
  camLookAt(&camera);
}

void forward() {
  camAvance(&camera, getShipSpeed(&ship));
  camLookAt(&camera);
}

/*void translateLeft() {
  camPasDeCote(&camera, ...);
  camLookAt(&camera);
}*/

/*void translateRight() {
  camPasDeCote(&camera, ...);
  camLookAt(&camera);
}*/

void activeAutoPilot() {
  autoPilot = 1;
  autoPilotTarget = moon;
}

void doAutoPilot() {
  //autoPilot = goToPlanet(...);
}

void display() {
  GLfloat lightPos[4] = {sun.x, sun.y, sun.z, 1};
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  
  moveSpace(&spaceSphere, camera.eye.x, camera.eye.y, camera.eye.z);
  drawSpace(&spaceSphere);
  drawPlanet(&planet1);
  drawPlanet(&planet2);
  drawPlanet(&moon);
  drawShip(&ship);
  drawSun(&sun);
}

void motion(SDL_MouseMotionEvent motion) {
  //fprintf(stderr, "debug: motion()\n");
  
  // Le WarpMouse(320, 320) génère également un évènement
  // motion qu'il ne faut pas prendre en compte.
  if (doMotion && (motion.x != 320 || motion.y != 320)) {
//    camTourneEtLeve(&camera, motion.xrel, motion.yrel);
//    camLookAt(&camera);
    SDL_WarpMouse(320, 320);
  } else {
    // La première fois xrel et yrel ne sont pas pertinents,
    // on ne déplace donc pas la caméra
    doMotion = 1;
  }
}

/**
 * Gestion du clavier et de la souris.
 */
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
    case SDL_MOUSEMOTION: {
        SDL_Event event_temp;
        // On récupère le dernier des évènements motion de la liste de évènements,
        // au cas où la machine ne soit pas assez rapide pour traiter le déplacement
        // de la souris assez vite.
        while (SDL_PeepEvents(&event_temp, 1, SDL_PEEKEVENT, SDL_MOUSEMOTIONMASK)>0)
          SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK);
        motion(event->motion);
        break;
      }
    }
}

int main(int argc, char *argv[]) {
  SDL_Surface *screen;
  if (argc > 1) {
    screen = gu_init_SDL("URS - University of Rouen Starship", atoi(argv[1]));
  } else {
    screen = gu_init_SDL("URS - University of Rouen Starship", 0);
  }
  gu_init_GL();
  gu_init_display(display);
  
  skuInitKeyBinder(&kb);
  skuBindKeyHandler(&kb, SDLK_ESCAPE, progQuit);
//  skuBindKeyHandler(&kb, SDLK_z, forward);
//  skuBindKeyHandler(&kb, SDLK_q, translateLeft);
//  skuBindKeyHandler(&kb, SDLK_d, translateRight);
  skuBindKeyHandler(&kb, SDLK_UP, increaseSpeed);
  skuBindKeyHandler(&kb, SDLK_DOWN, decreaseSpeed);
//  skuBindKeyHandler(&kb, SDLK_SPACE, doAutoPilot);
//  skuBindKeyHandler(&kb, SDLK_BACKSPACE, reinitCam);
  
  camInit(&camera);
  
  gu_initLights();
  
  GLfloat whiteLight[4] = {1, 1, 1, 1};
  GLfloat blackLight[4] = {0, 0, 0, 1};
  glLightfv(GL_LIGHT0, GL_AMBIENT, blackLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
 
  glMatrixMode(GL_PROJECTION);
  gluPerspective(50, (float) screen->w / screen->h, 4, 1000000000);
  glMatrixMode(GL_MODELVIEW);
  
  initShip(&ship, 0, 0, 1010000);
  camLookAtShip();
//  reinitCam();
  
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  GLuint textures;
  gu_initTextures(&textures, 1, 1, "space.png");
  initSpace(&spaceSphere, 0, 0, -100, 10, 90, 100, textures, 10);
  
  gu_initTextures(&textures, 1, 0, "blueMap.jpg");
  initPlanet(&planet2, 100000, 0, 800000, 30000, 90, -25, .002, 0, textures);
  
  gu_initTextures(&textures, 1, 0, "snowyMap.png");
  initPlanet(&planet1, 0, 0, 1000000, 3000, 90, -25, .005, 0, textures);
  
  gu_initTextures(&textures, 1, 0, "moonMap.jpg");
  initPlanet(&moon, 50000, 0, 900000, 500, 90, 15, .001, 0, textures);
  
  gu_initTextures(&textures, 1, 0, "sunMap.png");
  initSun(&sun, 0, 0, 0, 100000, 60, 56, -.003, textures);

  continuer = 1;
  SDL_Event event;
    
  Uint32 startTime;
  Uint32 ellapseTime = 0;
  int frameNb = 0;
  // Boucle traitant les évènements de la fenetre
  while (continuer) {
    startTime = SDL_GetTicks();
    if (ellapseTime >= 1000) {
      printf("\r~%d fps", frameNb);
      fflush(stdout);
      ellapseTime = 0;
      frameNb = 0;
    }
    // On catch un éventuel évènement
    while (SDL_PollEvent(&event)) {
      // On traite l'évènement
      eventCatcher(&event);
    }
    skuHandle(&kb);
    handleSpeed();
    camLookAtShip();
    gu_display();
    frameNb++;
    ellapseTime += SDL_GetTicks() - startTime;
  }
  
  // C'est fini : on libère les ressources proprement
  printf("\n");
  gu_SDLQuit(1);  
  exit(EXIT_SUCCESS);
}
