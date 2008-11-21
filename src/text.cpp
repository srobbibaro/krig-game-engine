//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : text.cpp                                       //
//                                                              //
// Description : Functions for displaying stroke fonts and      //
//               bitmap fonts.                                  //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////
#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include "text.h"
#include "constants.h"

using namespace std;

//------------------------------------------------------------------------------
void render_string(void* font, const char* string)
{
    //Renders a bitmap font string
    
	char* p = (char*) string;
	while (*p != '\0') glutBitmapCharacter(font, *p++);
}

//------------------------------------------------------------------------------
void displayText(char *text, float x, float y, float z, float scaleX, float scaleY)
{
    //Renders a stroke font string
    
    char *p;
    glPushMatrix();
        glTranslatef (x, y, z);
        glScalef (scaleX, scaleY, 0.0f);
        for (p = text; *p; p++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);      
    glPopMatrix();
}

//------------------------------------------------------------------------------
void ScriptTextType::displayText()
{
    if ( visible == true ) {
    glEnable(GL_BLEND);
        float scale = .005;
        if ( size == 0 ) {
            scale = .005;
            width = 3.0f;
        }
        else if ( size == 1 ) {
            scale = .008;
            width = 5.0f;
        }
        else if ( size == 2 ) {
            scale = .02;
            width = 8.0f;
        }
    
        char *p;
        glPushMatrix();
            if ( style == 1 ) {
                glColor4f( 0.0f, 0.0f, 0.0f, color[3] );
                glTranslatef(position.x, position.y, -20.2f );

                glLineWidth(width+8);
                glScalef (scale, scale, 0.0f);
                for (p = line; *p; p++)
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);  
            }
        glPopMatrix();
        glPushMatrix();  
            glColor4f( color[0], color[1], color[2], color[3] );
            glTranslatef(position.x, position.y, -20.0f );
            glLineWidth(width);
            glScalef (scale, scale, 0.0f);
            for (p = line; *p; p++)
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);      
        glPopMatrix();
           
        
        glDisable( GL_BLEND );
    }
}

//------------------------------------------------------------------------------
void ScriptTextType::updateText( float timeElapsed )
{
    if ( visible == true ) {
        position.x += velocity.x * timeElapsed;
        position.y += velocity.y * timeElapsed;
        position.z += velocity.z * timeElapsed;
    
    
        color[3] += timeElapsed * fadeRate;
        
        if ( color[3] > 1.0f ) {
            color[3] = 1.0f;
            fadeRate = 0.0f;
        }
        else if ( color[3] < 0.0f ) {
            color[3] = 0.0f;
            fadeRate = 0.0f;
        }
    }
}

//------------------------------------------------------------------------------
int ScriptTextType::processExtendedCommand( const ScriptCommand &t )
{
    cout << t.routine;
    
    switch ( t.routine )
    {
        case TEXT_SCRIPT_CLEAR_ALL:
            break;
            case TEXT_SCRIPT_SET_COLOR:
            color[0] = t.p1;
            color[1] = t.p2;
            color[2] = t.p3;
            break;
        case TEXT_SCRIPT_SET_FADE:
            fadeRate = t.p1;
            break;
        case TEXT_SCRIPT_SET_ALPHA:
            color[3] = t.p1;
            break;
        case TEXT_SCRIPT_SET_VISIBLE:
            if ( t.p1 >= 1 )
                visible = true;
            else
                visible = false;
                
            break;
        case TEXT_SCRIPT_SET_SIZE:
            if ( t.p1 > 2 || t.p1 < 0 )
                size = 0;
            else
                size = (int)t.p1;
                
            cout << "size" <<(int)t.p1;
            
            break;    
            
        case TEXT_SCRIPT_SET_TYPE:
            if ( t.p1 >= 1 )
                style = 1;
            else
                style = 0;
                
            
            break;
            
       }
}
