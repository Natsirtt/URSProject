#include "camera.h"

#include <math.h>
#ifndef M_PI
#define M_PI 3.141592654
#endif

static void _camRotationForwardUP(camCamera * camera, float angle, camVector * vecteur);

void camLookAt(const camCamera * camera) {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* Equivalent de
  gluLookAt(camera->eye.x,
	    camera->eye.y,  
	    camera->eye.z,
	    camera->eye.x + camera->forward.x,
	    camera->eye.y + camera->forward.y,
	    camera->eye.z + camera->forward.z,
	    camera->up.x,  
	    camera->up.y, 
	    camera->up.z);
  **/

  // Le repère de l'observateur en coordonnées du repère universel
  camVector obs_x, obs_y, obs_z;
  
  obs_y = camera->up;

  obs_z.x = -camera->forward.x;
  obs_z.y = -camera->forward.y;
  obs_z.z = -camera->forward.z;

  obs_x.z = obs_y.x * obs_z.y - obs_y.y * obs_z.x;
  obs_x.x = obs_y.y * obs_z.z - obs_y.z * obs_z.y;
  obs_x.y = obs_y.z * obs_z.x - obs_y.x * obs_z.z;

  // Matrice de changement de repère :
  // Repère universel -> repère local à la caméra
  GLfloat mat[16] = {
    obs_x.x, obs_y.x, obs_z.x, 0,
    obs_x.y, obs_y.y, obs_z.y, 0,
    obs_x.z, obs_y.z, obs_z.z, 0,
    obs_x.x * -camera->eye.x + obs_x.y * -camera->eye.y + obs_x.z *-camera->eye.z,
    obs_y.x * -camera->eye.x + obs_y.y * -camera->eye.y + obs_y.z *-camera->eye.z,
    obs_z.x * -camera->eye.x + obs_z.y * -camera->eye.y + obs_z.z *-camera->eye.z,
    1
  };

  glMultMatrixf(mat);
  glPopAttrib();
}

void camInit(camCamera * camera) {
  camera->eye.x = 0;
  camera->eye.y = 0;
  camera->eye.z = 0;

  camera->forward.x =  0;
  camera->forward.y =  0;
  camera->forward.z = -1;
  
  camera->up.x = 0;
  camera->up.y = 1;
  camera->up.z = 0;

  camera->sensibilite = 0.3;
}

void camFixePositionWithoutUp(camCamera * camera,
          float eyex, float eyey, float eyez,
          float centerx, float centery, float centerz) {
  camFixePosition(camera, eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);
}

void camFixeSensibilite(camCamera * camera, float sensibilite) {
  camera->sensibilite = sensibilite;
}

void camFixePosition(camCamera * camera,
		     float eyex, float eyey, float eyez,
		     float centerx, float centery, float centerz,
		     float upx, float upy, float upz) {
  // La position de l'observateur
  camera->eye.x = eyex;
  camera->eye.y = eyey;
  camera->eye.z = eyez;

  // Le vecteur observateur->point visé
  camera->forward.x = centerx - eyex;
  camera->forward.y = centery - eyey;
  camera->forward.z = centerz - eyez;
  
  // On normalise le vecteur
  float norme = sqrt(pow(camera->forward.x, 2)
		     + pow(camera->forward.y, 2)
		     + pow(camera->forward.z, 2));
  camera->forward.x /= norme;
  camera->forward.y /= norme;
  camera->forward.z /= norme;

  // On calcule l'axe des abscisses du repère local
  camVector abscisses;
  abscisses.x = camera->forward.y * upz - camera->forward.z * upy;
  abscisses.y = camera->forward.z * upx - camera->forward.x * upz;
  abscisses.z = camera->forward.x * upy - camera->forward.y * upx;

  // On en déduit l'axe des ordonnées
  // (cela permet éventuellement de "redresser" le vecteur up)
  camera->up.x = abscisses.y * camera->forward.z - abscisses.z * camera->forward.y;
  camera->up.y = abscisses.z * camera->forward.x - abscisses.x * camera->forward.z;
  camera->up.z = abscisses.x * camera->forward.y - abscisses.y * camera->forward.x;

  norme = sqrt(pow(camera->up.x, 2)
	       + pow(camera->up.y, 2)
	       + pow(camera->up.z, 2));
  camera->up.x /= norme;
  camera->up.y /= norme;
  camera->up.z /= norme;
}

void camAvance(camCamera * camera, float dep) {
  camera->eye.x += dep * camera->sensibilite * camera->forward.x;
  camera->eye.y += dep * camera->sensibilite * camera->forward.y;
  camera->eye.z += dep * camera->sensibilite * camera->forward.z;
}

void camPasDeCote(camCamera * camera, float dep) {
  camVector vecDep;

  vecDep.z = camera->up.x * camera->forward.y - camera->up.y * camera->forward.x;
  vecDep.x = camera->up.y * camera->forward.z - camera->up.z * camera->forward.y;
  vecDep.y = camera->up.z * camera->forward.x - camera->up.x * camera->forward.z;

  camera->eye.x += -dep * camera->sensibilite * vecDep.x;
  camera->eye.y += -dep * camera->sensibilite * vecDep.y;
  camera->eye.z += -dep * camera->sensibilite * vecDep.z;
  
}

void camUp(camCamera * camera, float dep) {
  
}

void camTourne(camCamera * camera, float dep) {
  // On tourne par rapport à l'orientation du sol.
  // On considère ici que le sol est à l'horizontal.
  camVector vecRot = { 0, 1, 0};

  _camRotationForwardUP(camera, -dep * camera->sensibilite, &vecRot);
}

void camLeve(camCamera * camera, float dep) {
  camVector vecRot;

  vecRot.z = camera->up.x * camera->forward.y - camera->up.y * camera->forward.x;
  vecRot.x = camera->up.y * camera->forward.z - camera->up.z * camera->forward.y;
  vecRot.y = camera->up.z * camera->forward.x - camera->up.x * camera->forward.z;

  _camRotationForwardUP(camera, dep * camera->sensibilite, &vecRot);
}

void camTourneEtLeve(camCamera * camera, float depTourne, float depLeve) {
  camTourne(camera, depTourne);
  camLeve(camera, depLeve);
  
  //Normalisation de forward
  float norme = sqrt(pow(camera->forward.x, 2)
                   + pow(camera->forward.y, 2)
                   + pow(camera->forward.z, 2));
                     
  camera->forward.x /= norme;
  camera->forward.y /= norme;
  camera->forward.z /= norme;
  
  //Normalisation de up
  norme = sqrt(pow(camera->up.x, 2)
                + pow(camera->up.y, 2)
                + pow(camera->up.z, 2));
  
  camera->up.x /= norme;
  camera->up.y /= norme;
  camera->up.z /= norme;
}

/**
 * On applique une rotation aux vecteurs forward et up de la caméra.
 * @param angle l'angle de la rotation.
 * @param vecteur le vecteur de la rotation.
 */
static void _camRotationForwardUP(camCamera * camera, float angle, camVector * vecteur) {
  angle = angle * M_PI / 180;

  float c = cos(angle);
  float s = sin(angle);

  camVector newForward;
  camVector newUp;

  newForward.x = camera->forward.x * (vecteur->x * vecteur->x * (1 - c) + c)
    + camera->forward.y * (vecteur->x * vecteur->y * (1 - c) - vecteur->z * s)
    + camera->forward.z * (vecteur->x * vecteur->z * (1 - c) + vecteur->y * s);
  newUp.x = camera->up.x * (vecteur->x * vecteur->x * (1 - c) + c)
    + camera->up.y * (vecteur->x * vecteur->y * (1 - c) - vecteur->z * s)
    + camera->up.z * (vecteur->x * vecteur->z * (1 - c) + vecteur->y * s);

  newForward.y = camera->forward.x * (vecteur->y * vecteur->x * (1 - c) + vecteur->z * s)
    + camera->forward.y * (vecteur->y * vecteur->y * (1 - c) + c)
    + camera->forward.z * (vecteur->y * vecteur->z * (1 - c) - vecteur->x * s);
  newUp.y = camera->up.x * (vecteur->y * vecteur->x * (1 - c) + vecteur->z * s)
    + camera->up.y * (vecteur->y * vecteur->y * (1 - c) + c)
    + camera->up.z * (vecteur->y * vecteur->z * (1 - c) - vecteur->x * s);

  newForward.z = camera->forward.x * (vecteur->x * vecteur->z * (1 - c) - vecteur->y * s)
    + camera->forward.y * (vecteur->y * vecteur->z * (1 - c) + vecteur->x * s)
    + camera->forward.z * (vecteur->z * vecteur->z * (1 - c) + c);
  newUp.z = camera->up.x * (vecteur->x * vecteur->z * (1 - c) - vecteur->y * s)
    + camera->up.y * (vecteur->y * vecteur->z * (1 - c) + vecteur->x * s)
    + camera->up.z * (vecteur->z * vecteur->z * (1 - c) + c);

  camera->forward = newForward;
  camera->up = newUp;
}
