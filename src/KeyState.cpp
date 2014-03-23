#include "KeyState.h"

void KeyState::initKeyState()
{
    for (int i = 0; i < 256; i++)
        keys[i] = -1;
}

bool KeyState::testKeyReleased(int key) {
    return key >= 0 && key < 256 && keys[key] == 2;
}

bool KeyState::testKeyPressed(int key) {
    return key >= 0 && key < 256 && keys[key] == 1;
}
