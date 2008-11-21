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

#endif
