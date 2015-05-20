#ifndef _KEY_STATE_H_
#define _KEY_STATE_H_

#define MAX_KEYS           256
#define KEY_STATE_INIT     -1
#define KEY_STATE_PRESSED  1
#define KEY_STATE_RELEASED 2

class KeyState {
  public:
    void initKeyState();
    bool testKeyReleased(int key);
    bool testKeyPressed(int key);

    int keys[MAX_KEYS];
};

#endif
