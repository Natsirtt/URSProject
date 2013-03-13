#include "fblList.h"

#include <stdio.h>
#include <stdlib.h>

void _fbl_debugPrint(fblList *l) {
  printf("fblList debug : ");
  fblCell *tmp = l->data;
  while (tmp != NULL) {
    printf("%d -> ", tmp->val);
    tmp = tmp->next;
  }
  printf("X ~ size = %d\n", fbl_size(l));
}

fblCell *_fbl_newfblCell() {
  return (fblCell *) malloc(sizeof(fblCell));
}

fblCell *_fbl_getfblCell(fblList *l, int value) {
  fblCell *tmp = l->data;
  while (tmp != NULL) {
    if (tmp->val == value) {
      return tmp;
    }
  }
  return NULL;
}

/**
 * Initialise la liste afin qu'elle soit utilisable.
 * A utiliser sur toute nouvelle variable du type fblList avant utilisation.
 * assertion d'entrée : l != NULL
 */
void fbl_initList(fblList *l) {
  l->data = NULL;
  l->size = 0;
}

/**
 * Insert une fblCellule avec la valeur value en tête de liste.
 * assertion d'entrée : l != NULL
 */
void fbl_beginingInsert(fblList *l, int value, void (*keyHandler)(void)) {
  fblCell *newfblCell = _fbl_newfblCell();
  newfblCell->val = value;
  newfblCell->keyHandler = keyHandler;
  newfblCell->next = l->data;
  l->data = newfblCell;
  l->size++;
}

/**
 * Change le pointeur de fonction pour cette valeur.
 * Assertion d'entrée : l != NULL && contains(l, value)
 */
void fbl_changeHandler(fblList *l, int value, void (*keyHandler)(void)) {
  fblCell *cell = _fbl_getfblCell(l, value);
  cell->keyHandler = keyHandler;
}

fblCell *_fbl_recursiveDelete(fblCell *fblCellList, int value, int *bool) {
  fblCell *res;
  *bool = 0;
  if (fblCellList == NULL) {
    res = NULL;
  } else if (fblCellList->val == value) {
    res = fblCellList->next;
    free(fblCellList);
    *bool = 1;
  } else {
    res = fblCellList;
    res->next = _fbl_recursiveDelete(fblCellList->next, value, bool);
  }
  return res;
}

/**
 * Supprime la première occurence de fblCellule à valeur value.
 * Ne fait rien si aucune fblCellule ne contient cette valeur.
 * Assertion d'entrée : l != NULL
 */
void fbl_deleteValue(fblList *l, int value) {
  int hasSuppr;
  l->data = _fbl_recursiveDelete(l->data, value, &hasSuppr);
  l->size -= hasSuppr;
}

/**
 * Retourne 1 si la liste contient la valeur value, 0 sinon.
 */
int fbl_contains(fblList *l, int value) {
  fblCell *tmp = l->data;
  while (tmp != NULL) {
    if (tmp->val == value) {
      return 1;
    }
    tmp = tmp->next;
  }
  return 0;
}

/**
 * Active la fonction de handler de cet entier.
 */
void fbl_handle(fblList *l, int value) {
  fblCell *cell = _fbl_getfblCell(l, value);
  if (cell != NULL) {
    cell->keyHandler();
  }
}

/**
 * Détruit correctement la liste.
 */
void fbl_destroyList(fblList *l) {
  fblCell *tmp = l->data;
  while (l->data != NULL) {
    tmp = l->data;
    l->data = tmp->next;
    free(tmp);
  }
}

/**
 * Donne la taille de la liste.
 */
int fbl_size(fblList *l) {
  return l->size;
}
