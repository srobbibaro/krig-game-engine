//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : eventqueue.cpp                                 //
//                                                              //
// Description : Functions for controlling events during game   //
//               play.                                          //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////
#include "eventqueue.h"

//Constructor
EventQueue::EventQueue()
{
    Top = 0, Bottom = 0;
    isEmpty = true;

    for (int i=0; i<500; i++)
	Queue[i] = 0;
}

//Add command to queue
void EventQueue::enQueue(int command)
{
    if (isEmpty)
    {
	Queue[Bottom] = command;
	isEmpty = false;
    }

    else
    {
        Bottom++;
	if (Bottom <= 499)
	    Queue[Bottom] = command;
    }
}

//Remove command from queue
int EventQueue::deQueue()
{
    if (isEmpty)
        return 0;
        
    int command;
    if (Top <= 499)
    {
    	command = Queue[Top];
	    Queue[Top] = 0;    

        Top++;

        if (Top > Bottom)
        {
            Top = 0;
            Bottom = 0;
    	    isEmpty = true;
        }
    }

    return command;    
}

//------------------------------------------------------------------------------
void EventQueue::emptyQueue()
{
    isEmpty = true;
    Top = 0;
    Bottom = 0;
}
