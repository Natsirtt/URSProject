#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL_image.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>

#include "glUtils.h"

#define BUFF_LENGTH 4096

//La fonction de display, appelée en far-call
void (*displayFunction)(void);

void gu_SDLQuit(int exitSuccess) {
  SDL_Quit();
  if (exitSuccess) {
    exit(EXIT_SUCCESS);
  }
  exit(EXIT_FAILURE);
}


/**
 * Initialisation standard des lumières.
 */
void gu_initLights() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

/**
 * Initialisation de la SDL, création du contexte OpenGL et ouverture de la fenetre.
 */
SDL_Surface *gu_init_SDL(const char *title, int antialiasing) {
  SDL_Init(SDL_INIT_VIDEO);

  if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == -1) {
    fprintf(stderr, "Impossible d'activer le double buffer\n");
    gu_SDLQuit(0);
  }
  
  if (antialiasing > 0) {
    if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, antialiasing) == -1) {
      fprintf(stderr, "Impossible d'activer l'antialiasing (%d samples)", antialiasing);
    }
  }
  
  SDL_WM_SetCaption(title, NULL);

  SDL_Surface *surface;
  if ((surface = SDL_SetVideoMode(0, 0, 32, SDL_OPENGL | SDL_FULLSCREEN)) == NULL) {
    fprintf(stderr, "Impossible de passer en mode OpenGL : %s\n", SDL_GetError());
    gu_SDLQuit(0);
  }
  
  SDL_ShowCursor(SDL_DISABLE);
  SDL_WM_GrabInput(SDL_GRAB_ON);
    
  return surface;
}

/**
 * Initialisation des états du contexte OpenGL.
 */
void gu_init_GL(void) {
	glClearColor(0., 0., 0., 0.);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glMatrixMode(GL_MODELVIEW);
}


void gu_init_display(void (*display)(void)) {
  displayFunction = display;
}

/**
 * Affichage de la scène OpenGL.
 */
void gu_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glPushMatrix();
  displayFunction();
  glPopMatrix();
  
  SDL_GL_SwapBuffers();

  GLenum erreur;
  if ((erreur = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "Erreur OpenGL dans display : %s\n", gluErrorString(erreur));
  }
}

void gu_drawPara(float l, float h, float p) {
  float xNorm = l/2;
  float yNorm = h/2;
  float zNorm = p/2;
  glPushMatrix();
    //Première face
    glColor3ub(192, 192, 192);
    glBegin(GL_QUADS);
      glVertex3f(xNorm, yNorm, zNorm);
      glVertex3f(-xNorm, yNorm, zNorm);
      glVertex3f(-xNorm, -yNorm, zNorm);
      glVertex3f(xNorm, -yNorm, zNorm);
    glEnd();
    //Deuxième face
    glColor3ub(120, 120, 120);
    glBegin(GL_QUADS);
      glVertex3f(xNorm, yNorm, -zNorm);
      glVertex3f(xNorm, yNorm, zNorm);
      glVertex3f(xNorm, -yNorm, zNorm);
      glVertex3f(xNorm, -yNorm, -zNorm);
    glEnd();
    //Troisième face
    glColor3ub(192, 192, 192);
    glBegin(GL_QUADS);
      glVertex3f(-xNorm, yNorm, -zNorm);
      glVertex3f(xNorm, yNorm, -zNorm);
      glVertex3f(xNorm, -yNorm, -zNorm);
      glVertex3f(-xNorm, -yNorm, -zNorm);
    glEnd();
    glColor3ub(120, 120, 120);
    //Quatrième face
    glBegin(GL_QUADS);
      glVertex3f(-xNorm, yNorm, zNorm);
      glVertex3f(-xNorm, yNorm, -zNorm);
      glVertex3f(-xNorm, -yNorm, -zNorm);
      glVertex3f(-xNorm, -yNorm, zNorm);
    glEnd();
    //Cinquième face
    glColor3ub(240, 240, 240);
    glBegin(GL_QUADS);
      glVertex3f(xNorm, yNorm, -zNorm);
      glVertex3f(-xNorm, yNorm, -zNorm);
      glVertex3f(-xNorm, yNorm, zNorm);
      glVertex3f(xNorm, yNorm, zNorm);
    glEnd();
    //Sixième face
    glBegin(GL_QUADS);
      glVertex3f(xNorm, -yNorm, -zNorm);
      glVertex3f(-xNorm, -yNorm, -zNorm);
      glVertex3f(-xNorm, -yNorm, zNorm);
      glVertex3f(xNorm, -yNorm, zNorm);
    glEnd();
  glPopMatrix();
}

void gu_drawSphere(float radius, int sliceAndStack, int textured, int normalOutside) {
  glColor3f(1., 1., 1.);
  
  GLUquadric* params = gluNewQuadric();

  gluQuadricDrawStyle(params, GLU_FILL);
  if (!normalOutside) {
    gluQuadricOrientation(params, GLU_INSIDE);
  }
  gluQuadricNormals(params, GLU_SMOOTH);
  if (textured) {
    gluQuadricTexture(params, GL_TRUE);
  } else {
    gluQuadricTexture(params, GL_FALSE);
  }
  gluSphere(params, radius, sliceAndStack, sliceAndStack);
  gluDeleteQuadric(params);
}

void _gu_bindTexture(GLuint texture, const char *path, int repeat) {
  
    glBindTexture(GL_TEXTURE_2D, texture);

    // On fixe les paramètres de la texture,
    // i.e. comment on calcule les coordonnées pixels -> texels.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, /*GL_NEAREST*/ GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, /*GL_NEAREST*/ GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    
    if (repeat) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }
    
    // On charge l'image qui va nous servir de texture via la SDL
    SDL_Surface *image;
    image = IMG_Load(path);
    if(!image) {
      fprintf(stderr, "Impossible de charger l'image : %s\n", IMG_GetError());
      exit(EXIT_FAILURE);
    }

    // On verrouille l'image pendant qu'on l'utilise
    if (SDL_MUSTLOCK(image)) {
        SDL_LockSurface(image);
    }

    // On envoie le tableau contenant les texels de la texture à OpenGL
    if (image->format->BytesPerPixel == 3) {
      // L'image d'origine n'a que trois composantes
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
      //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->w, image->h, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    } else if (image->format->BytesPerPixel == 4) {
      // L'image d'origine à quatre composantes
      // (la dernière est le paramètre alpha qui va nous servir à gérer la transparence)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
      //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image->w, image->h, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
      /*if (res != 0) {
          
      }*/
    } else {
      fprintf(stderr, "Nombre de composants de l'image différent de 3 et 4\n");
      exit(EXIT_FAILURE);
    }
    
    // On libère les ressources occupées par l'image
    if (SDL_MUSTLOCK(image)) {
        SDL_UnlockSurface(image);
    }
    SDL_FreeSurface(image);

    GLenum erreur;
    if ((erreur = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "Erreur OpenGL dans setTexture : %s\n", gluErrorString(erreur));
    }
    glFlush();
}

/**
 * Charge les textures qui seront utilisées. texNb représente le nombre
 * de texture à charger (et donc la taille minimale du tableau tex, ainsi
 * que le nombre de paramètres suivants).
 */
void gu_initTextures(GLuint *tex, int texNb, int repeatTex, ...) {
  //Liste des arguments
  va_list args;
  //Initialisation, à partir du dernier paramètre connu
  va_start (args, repeatTex);
  
  //On génère des objets textures prêts à être complétés/paramétrés
  //dans le tableau des textures.
  glGenTextures(texNb, tex);
  
  char buff[BUFF_LENGTH];
  for (int i = 0; i < texNb; i++) {
    snprintf(buff, BUFF_LENGTH, "tex/%s", va_arg(args, char *));
    _gu_bindTexture(tex[i], buff, repeatTex);
  }
  
  va_end(args);
  glFlush();
}

/**
 * Fait une roation d'angle a sur l'axe des x.
 */
void gu_xRotate(float a) {
  glRotatef(a, 1., 0., 0.);
}

/**
 * Fait une roation d'angle a sur l'axe des y.
 */
void gu_yRotate(float a) {
  glRotatef(a, 0., 1., 0.);
}
 
/**
 * Fait une roation d'angle a sur l'axe des z.
 */
void gu_zRotate(float a) {
  glRotatef(a, 0., 0., 1.);
}

void gu_normal(vector_t *vBuff, vector_t v1, vector_t v2, vector_t v3) {
  vector_t u;
  u.x = v1.x - v2.x;
  u.y = v1.y - v2.y;
  u.z = v1.z - v2.z;
  
  vector_t v;
  v.x = v3.x - v2.x;
  v.y = v3.y - v2.y;
  v.z = v3.z - v2.z;
  
  vBuff->x = u.y * v.z - u.z * v.y;
  vBuff->y = u.z * v.x - u.x * v.z;
  vBuff->z = u.x * v.y - u.y * v.x;
}

float __gu_sqr(float x) {
  return x * x;
}

void gu_normalizeVector(vector_t *v) {
  float norm = sqrt(__gu_sqr(v->x) + __gu_sqr(v->y) + __gu_sqr(v->z));
  v->x = v->x / norm;
  v->y = v->y / norm;
  v->z = v->z / norm;
}
