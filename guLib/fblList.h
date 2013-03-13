#ifndef GU_FUNCTION_BIND_LIST
#define GU_FUNCTION_BIND_LIST

typedef struct FBL_CELL{
  int val;
  void (*keyHandler)(void);
  struct FBL_CELL *next;
} fblCell;

typedef struct {
  fblCell *data;
  int size;
} fblList;

/**
 * Initialise la liste afin qu'elle soit utilisable.
 * A utiliser sur toute nouvelle variable du type intList avant utilisation.
 */
void fbl_initList(fblList *l);

/**
 * Insert une cellule avec la valeur value en tête de liste.
 */
void fbl_beginingInsert(fblList *l, int value, void (*keyHandler)(void));

/**
 * Change le pointeur de fonction pour cette valeur.
 * Assertion d'entrée : l != NULL && contains(l, value)
 */
void fbl_changeHandler(fblList *l, int value, void (*keyHandler)(void));

/**
 * Supprime la première occurence de cellule à valeur value.
 * Ne fait rien si aucune cellule ne contient cette valeur.
 */
void fbl_deleteValue(fblList *l, int value);

/**
 * Retourne 1 si la liste contient la valeur value, 0 sinon.
 */
int fbl_contains(fblList *l, int value);

/**
 * Détruit correctement la liste.
 */
void fbl_destroyList(fblList *l);

/**
 * Donne la taille de la liste.
 */
int fbl_size(fblList *l);

#endif
