//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : text.h                                         //
//                                                              //
// Description : Functions for displaying stroke fonts and      //
//               bitmap fonts.                                  //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////
#include "constants.h"
#include "ScriptedObject.h"

#ifndef _TEXT_H_
#define _TEXT_H_
    class ScriptTextType : public ScriptedObject
    {
        public:
        char line[128];
        float color[4];
        bool visible;
        int size;
        float width;
        int style;
        float fadeRate;
       
        void displayText();
        void updateText( float );
    };


    void render_string(void* font, const char* string);
    void displayText(char *text, float x, float y, float z, float, float );
    
#endif
