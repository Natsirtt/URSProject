#ifndef GL_UTILS
#define GL_UTILS

#include <SDL/SDL.h>
#include <GL/gl.h>

typedef struct {
  float x;
  float y;
  float z;
} vector_t;

/**
 * Activation des lumières et de la lumière 0.
 */
void gu_initLights();

/**
 * Initialisation de la SDL, création du contexte OpenGL et ouverture de la fenetre en plein écran.
 * Renvoie la surface afin de connaitre le ratio de la fenêtre.
 */
SDL_Surface *gu_init_SDL(const char *title, int antialiasing);

/**
 * Initialisation des états du contexte OpenGL.
 */
void gu_init_GL();

void gu_frustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top,
                GLdouble nearVal, GLdouble farVal);

/**
 * Indique la fonction de display du programme.
 */
void gu_init_display(void (*display)(void));

/**
 * Affichage de la scène OpenGL.
 */
void gu_display();

/**
 * Quitte le programme en terminant correctement la SDL et en renvoyant
 * un EXIT_SUCCESS si exitSuccess != 0, un EXIT_FAILURE sinon.
 */
void gu_SDLQuit(int exitSuccess);

/**
 * Dessine un parallépipède de longueur l, hauteur h et profondeur p,
 * au niveau du repère.
 */
void gu_drawPara(float l, float h, float p);

/**
 * Dessine une sphere de rayon radius et paramétrée en couches (d'un nombre
 * de sliceAndStack). textured indique si on utilise une texture ou non.
 */
void gu_drawSphere(float radius, int sliceAndStack, int textured, int normalOutside);

/**
 * Charge les textures qui seront utilisées. texNb représente le nombre
 * de texture à charger (et donc la taille minimale du tableau tex, ainsi
 * que le nombre de paramètres suivants).
 */
void gu_initTextures(GLuint *tex, int texNb, int repeatTex, ...);

/**
 * Charge et compile le shader représenté par filename.
 */
GLuint gu_loadShader(const char *filename);

/**
 * Fait une roation d'angle a sur l'axe des x.
 */
void gu_xRotate(float a);

/**
 * Fait une roation d'angle a sur l'axe des y.
 */
void gu_yRotate(float a);
 
 /**
 * Fait une roation d'angle a sur l'axe des z.
 */
void gu_zRotate(float a);

/**
 * Calcul le produit scalaire de v2 et le range dans le buffer.
 */
void gu_normal(vector_t *vBuff, vector_t v1, vector_t v2, vector_t v3);

/**
 * Normalise le vecteur.
 */
void gu_normalizeVector(vector_t *v);

#endif
