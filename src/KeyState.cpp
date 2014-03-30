#include "KeyState.h"

void KeyState::initKeyState()
{
    for (int i = 0; i < MAX_KEYS; i++)
        keys[i] = -1;
}

bool KeyState::testKeyReleased(int key) {
    return key >= 0 && key < MAX_KEYS && keys[key] == 2;
}

bool KeyState::testKeyPressed(int key) {
    return key >= 0 && key < MAX_KEYS && keys[key] == 1;
}
