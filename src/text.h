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
#include "Object.h"
#include <string>

using namespace std;

#ifndef _TEXT_H_
#define _TEXT_H_
    class ScriptTextType : public Object
    {
        public:
        string text;
        float color[4];
        float width;
        float fadeRate;

        ScriptTextType();

        void printTypeName(void)
        {
#if DEBUG
            printf("text");
#endif
        }

        void draw(Object*);
        void drawOutline( Object* ) {}; // Camera*
        void drawShadow ( Vector* ) {}
        void handleCollision( Object* ) {};
        void update( Vector* ) {};
        void prepare( void ) {};
        void animate( float, Object* ); // Camera*
    };


    void render_string(void* font, const char* string);
    void displayText(char *text, float x, float y, float z, float, float );

#endif
