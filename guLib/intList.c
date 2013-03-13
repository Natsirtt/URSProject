#include "intList.h"

#include <stdio.h>
#include <stdlib.h>

void _il_debugPrint(intList *l) {
  printf("intList debug : ");
  intCell *tmp = l->data;
  while (tmp != NULL) {
    printf("%d -> ", tmp->val);
    tmp = tmp->next;
  }
  printf("X ~ size = %d\n", il_size(l));
}

intCell *_il_newintCell() {
  return (intCell *) malloc(sizeof(intCell));
}

intCell *_il_getintCell(intList *l, int value) {
  intCell *tmp = l->data;
  while (tmp != NULL) {
    if (tmp->val == value) {
      return tmp;
    }
  }
  return NULL;
}

/**
 * Initialise la liste afin qu'elle soit utilisable.
 * A utiliser sur toute nouvelle variable du type intList avant utilisation.
 * assertion d'entrée : l != NULL
 */
void il_initList(intList *l) {
  l->data = NULL;
  l-> size = 0;
}

/**
 * Insert une intCellule avec la valeur value en tête de liste.
 * assertion d'entrée : l != NULL
 */
void il_beginingInsert(intList *l, int value) {
  intCell *newintCell = _il_newintCell();
  newintCell->val = value;
  newintCell->next = l->data;
  l->data = newintCell;
  l->size++;
}

intCell *_il_recursiveDelete(intCell *intCellList, int value, int *bool) {
  intCell *res;
  *bool = 0;
  if (intCellList == NULL) {
    res = NULL;
  } else if (intCellList->val == value) {
    res = intCellList->next;
    free(intCellList);
    *bool = 1;
  } else {
    res = intCellList;
    res->next = _il_recursiveDelete(intCellList->next, value, bool);
  }
  return res;
}

/**
 * Supprime la première occurence de intCellule à valeur value.
 * Ne fait rien si aucune intCellule ne contient cette valeur.
 * Assertion d'entrée : l != NULL
 */
void il_deleteValue(intList *l, int value) {
  int hasSuppr;
  l->data = _il_recursiveDelete(l->data, value, &hasSuppr);
  l->size -= hasSuppr;
}

/**
 * Retourne 1 si la liste contient la valeur value, 0 sinon.
 */
int il_contains(intList *l, int value) {
  intCell *tmp = l->data;
  while (tmp != NULL) {
    if (tmp->val == value) {
      return 1;
    }
    tmp = tmp->next;
  }
  return 0;
}

/**
 * Détruit correctement la liste.
 */
void il_destroyList(intList *l) {
  intCell *tmp = l->data;
  while (l->data != NULL) {
    tmp = l->data;
    l->data = tmp->next;
    free(tmp);
  }
}

/**
 * Donne la taille de la liste.
 */
int il_size(intList *l) {
  return l->size;
}
