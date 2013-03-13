#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <SDL/SDL.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "sdlKeyUtils.h"

void _skuError(const char *msg) {
  fprintf(stderr, "sku_error: %s\n", msg);
}

void _skuWarning(const char *msg) {
  fprintf(stderr, "sku_warning: %s\n", msg);
}

void skuInitKeyBinder(keyBinder *kb) {
  if (kb == NULL) {
    _skuError("null keyBinder pointer");
    return;
  }
  il_initList(&(kb->keyBind));
  fbl_initList(&(kb->functionBind));
}

void skuBindKeyHandler(keyBinder *kb, int key, void (*keyHandler)(void)) {
  if (kb == NULL) {
    _skuError("null keyBinder pointer");
    return;
  }
  if (!fbl_contains(&(kb->functionBind), key)) {
    fbl_beginingInsert(&(kb->functionBind), key, keyHandler);
  } else {
    fbl_changeHandler(&(kb->functionBind), key, keyHandler);
  }
}

void skuKeyPressed(keyBinder *kb, int key) {
  if (kb == NULL) {
    _skuError("null keyBinder");
    return;
  }
  if (skuIsPressed(kb, key)) {
    _skuWarning("Key was already pressed");
    return;
  }
  il_beginingInsert(&(kb->keyBind), key);
}

void skuKeyReleased(keyBinder *kb, int key) {
  if (kb == NULL) {
    _skuError("null keyBinder pointer");
    return;
  }
  if (!skuIsPressed(kb, key)) {
    _skuWarning("Key was'nt pressed");
    return;
  }
  il_deleteValue(&(kb->keyBind), key);
}

int skuIsPressed(keyBinder *kb, int key) {
  if (kb == NULL) {
    _skuError("null keyBinder pointer");
    return -1;
  }
  return il_contains(&(kb->keyBind), key);
}

void skuHandle(keyBinder *kb) {
  fblCell *handlersCell = kb->functionBind.data;
  while (handlersCell != NULL) {
    if (il_contains(&(kb->keyBind), handlersCell->val)) {
      handlersCell->keyHandler();
    }
    handlersCell = handlersCell->next;
  }
}

void skuDestroyKeyBinder(keyBinder *kb) {
  il_destroyList(&(kb->keyBind));
  fbl_destroyList(&(kb->functionBind));
}
