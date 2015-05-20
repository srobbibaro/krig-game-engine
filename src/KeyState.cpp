#include "KeyState.h"

void KeyState::initKeyState() {
  for (int i = 0; i < MAX_KEYS; i++)
    keys[i] = KEY_STATE_INIT;
}

bool KeyState::testKeyReleased(int key) {
  return key >= 0 && key < MAX_KEYS && keys[key] == KEY_STATE_RELEASED;
}

bool KeyState::testKeyPressed(int key) {
  return key >= 0 && key < MAX_KEYS && keys[key] == KEY_STATE_PRESSED;
}
