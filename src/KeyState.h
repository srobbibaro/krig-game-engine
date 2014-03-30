#ifndef _KEY_STATE_H_
#define _KEY_STATE_H_

#define MAX_KEYS    256

class KeyState
{
    public:
    int keys[MAX_KEYS];

    void initKeyState();
    bool testKeyReleased(int key);
    bool testKeyPressed(int key);
};

#endif
