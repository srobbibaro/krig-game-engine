//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : eventqueue.h                                   //
//                                                              //
// Description : Class to handle events during gameplay         //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////
#ifndef _EVENTQUEUE_H_
#define _EVENTQUEUE_H_

class EventQueue
{
    private:
	int Queue[500];
	int Top, Bottom;
	bool isEmpty;

    public:
	EventQueue();

	void enQueue(int);
	int deQueue();
    void emptyQueue();
};

class KeyState
{
    public:
    
    int keys[256];
    
     void initKeyState()
        {
            for (int i = 0; i < 256; i++) 
                keys[i] = -1;
        }
        
        bool testKeyReleased(int key) {
            return key >= 0 && key < 256 && keys[key] == 2;
        }
        
        bool testKeyPressed(int key) {
            return key >= 0 && key < 256 && keys[key] == 1;
        }
};

#endif
