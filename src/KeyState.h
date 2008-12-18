#ifndef _KEY_STATE_H_
#define _KEY_STATE_H_

class KeyState
{
    public:  
    int keys[256];
    
    void initKeyState();
    bool testKeyReleased(int key);
    bool testKeyPressed(int key);    
};

#endif
