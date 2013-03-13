#ifndef SDL_KEY_UTILS
#define SDL_KEY_UTILS

#define KEY_NB 231

#include "intList.h"
#include "fblList.h"

typedef struct {
  fblList functionBind;
  intList keyBind;
} keyBinder;

void skuInitKeyBinder(keyBinder *kb);

void skuBindKeyHandler(keyBinder *kb, int key, void (*keyHandler)(void));

void skuKeyPressed(keyBinder *kb, int key);

void skuKeyReleased(keyBinder *kb, int key);

int skuIsPressed(keyBinder *kb, int key);

void skuHandle(keyBinder *kb);

void skuDestroyKeyBinder(keyBinder *kb);

#endif
