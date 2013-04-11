/**
 * @file camera.h
 * @brief Gestion minimale d'une caméra subjective.
 *
 * On peut avancer, tourner, lever la tête et se déplacer de côté.
 */

#ifndef __CAMERA_H
#define __CAMERA_H

#include <GL/gl.h>

typedef struct {
  GLfloat x;
  GLfloat y;
  GLfloat z;
} camVector;

typedef struct {
  camVector pos;
  camVector forward;
  camVector up;
  float sensibilite;
} camCamera;

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Positionne l'observateur dans la matrice MODELVIEW.
   * En sortie, MODELVIEW ne contient que le positionnement de
   * la caméra.
   */
  void camLookAt(const camCamera * camera);

  /**
   * Initialise la position de la caméra à sa position par défaut
   * (position en (0, 0, 0), regard vers (0, 0, -1) et up (0, 1, 0))
   */
  void camInit(camCamera * camera);
  
  /**
   * Positionne la caméra (mais ne modifie pas la matrice MODELVIEW)
   * sans changer le vecteur UP.
   */
  void camFixePositionWithoutUp(camCamera * camera,
          float eyex, float eyey, float eyez,
          float centerx, float centery, float centerz);
  
  /**
   * Positionne uniquement la position de l'oeil la caméra.
   */
  void camFixeEye(camCamera *camera, float eyex, float eyey, float eyez);

  /**
   * Fixe la sensibilité de la caméra aux déplacements.
   * @param sensibilite la sensibilité, plus elle est élevée,
   * plus le déplacement sera grand.
   */
  void camFixeSensibilite(camCamera * camera, float sensibilite);

  /**
   * Positionne la caméra (mais ne modifie pas la matrice MODELVIEW).
   * Les paramètres sont les mêmes que pour gluLookAt.
   */
  void camFixePosition(camCamera * camera,
		     float eyex, float eyey, float eyez,
		     float centerx, float centery, float centerz,
		     float upx, float upy, float upz);
  /**
   * Avance la caméra dans la direction où elle regarde.
   * @param dep un indicateur de l'importance du déplacement.
   */
  void camAvance(camCamera * camera, float dep);

  /**
   * Déplace la caméra sur son côté.
   * @param dep indique le déplacement à réaliser,
   * (une valeur < 0 indique qu'on va à gauche et > 0 à droite)
   */
  void camPasDeCote(camCamera * camera, float dep);
  
  /**
   * Déplace la caméra a la verticale.
   * @param dep indique le déplacement à réaliser,
   * (une valeur < 0 indique qu'on va vers le haut et > 0 vers le bas)
   */
  void camUp(camCamera * camera, float dep);

  /**
   * Lève ou abaisse le regard de la caméra.
   * La rotation s'effectue selon le repère local à la caméra.
   * @param dep indique l'angle de rotation.
   */
  void camLeve(camCamera * camera, float dep);

  /**
   * Tourne la caméra à droite ou à gauche.
   * La rotation s'effectue selon le repère universel
   * et on considère que le sol est à l'horizontal
   * (rotation autour de l'axe y du repère universel).
   * @param dep indique l'angle de rotation.
   */
  void camTourne(camCamera * camera, float dep);

  /**
   * Raccourci pour
   * @code
   * camTourne(camera, depTourne);
   * camLeve(camera, depLeve);
   * @endcode
   */
  void camTourneEtLeve(camCamera * camera, float depTourne, float depLeve);

#ifdef __cplusplus
}
#endif

#endif // __CAMERA_H
