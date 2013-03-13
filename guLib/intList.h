#ifndef GU_INTLIST
#define GU_INTLIST

typedef struct INT_CELL {
  int val;
  struct INT_CELL *next;
} intCell;

typedef struct {
  intCell *data;
  int size;
} intList;

/**
 * Initialise la liste afin qu'elle soit utilisable.
 * A utiliser sur toute nouvelle variable du type intList avant utilisation.
 */
void il_initList(intList *l);

/**
 * Insert une intCellule avec la valeur value en tête de liste.
 */
void il_beginingInsert(intList *l, int value);

/**
 * Supprime la première occurence de intCellule à valeur value.
 * Ne fait rien si aucune intCellule ne contient cette valeur.
 */
void il_deleteValue(intList *l, int value);

/**
 * Retourne 1 si la liste contient la valeur value, 0 sinon.
 */
int il_contains(intList *l, int value);

/**
 * Détruit correctement la liste.
 */
void il_destroyList(intList *l);

/**
 * Donne la taille de la liste.
 */
int il_size(intList *l);

#endif
