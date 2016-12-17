/**
 * @file KeyState.h
 * @brief Lookup state of all keyboard keys.
 *
 * Determine whether a given keyboard key is being pressed, has been released,
 * or has not been pressed at all.
 */
#ifndef _KEY_STATE_H_
#define _KEY_STATE_H_

#define MAX_KEYS           256
#define KEY_STATE_INIT     -1
#define KEY_STATE_PRESSED  1
#define KEY_STATE_RELEASED 2

class KeyState {
  public:
    void initKeyState();
    bool testKeyReleased(const int&);
    bool testKeyPressed(const int&);

    int keys[MAX_KEYS];
};

#endif
