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
ScriptTextType::ScriptTextType()
: Object()
{
    text = "";
    color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.0f;
    width = 10.0f;
    fadeRate = 0.0f;
}

//------------------------------------------------------------------------------
void ScriptTextType::draw(Object* camera)
{
    if ( isInView ) {
        glEnable(GL_BLEND);
        glPushMatrix();  
            glColor4fv( color );
            glLineWidth(width);
            //glTranslatef(position.x, position.y, position.z );
            glRasterPos3f(position.x, position.y, position.z); 
            glScalef (scale.x, scale.y, scale.z);
            for (int i = 0; i < text.length(); i++) {
                //glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);      
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
            }
        glPopMatrix();
        glDisable( GL_BLEND );
    }
}

//------------------------------------------------------------------------------
void ScriptTextType::animate( float timeElapsed, Object* camera) // Camera*
{
    // exectue the current object's update function
    animateScript(timeElapsed);

    // calculate new position using speed
    if (speed.x != 0.0f) {
        direction.scale(speed.x * timeElapsed);
        position.x += direction.x;
        position.y += direction.y;
        position.z += direction.z;
        direction.normalize();
    }
    
    if (speed.y != 0.0f) {
        up.scale(speed.y * timeElapsed);
        position.x += up.x;
        position.y += up.y;
        position.z += up.z;
        direction.normalize();
    }
    
    if (speed.z != 0.0f) {
        Vector rotationAxis;
            
        rotationAxis.crossProduct(up, direction);
        rotationAxis.normalize();
                        
        rotationAxis.scale(speed.z * timeElapsed);
        position.x += rotationAxis.x;
        position.y += rotationAxis.y;
        position.z += rotationAxis.z;
    }
    
    // update position using velocity
    if (velocity.x != 0.0f)
        position.x += velocity.x * timeElapsed; 
        
    if (velocity.y != 0.0f)  
        position.y += velocity.y * timeElapsed;   
        
    if (velocity.z != 0.0f)
        position.z += velocity.z * timeElapsed;   
    
    // update scale
    if (scaleRate.x != 0.0f) {     
        scale.x += scaleRate.x * timeElapsed;
        scaleChanged = true;
    }
    
    if (scaleRate.y != 0.0f) {     
        scale.y += scaleRate.y * timeElapsed;
        scaleChanged = true;
    }
    
    if (scaleRate.z != 0.0f) {     
        scale.z += scaleRate.z * timeElapsed;
        scaleChanged = true;
    }
                 
    if (!isInterpolationEnabled_) {
        if ( rotationVelocity.x != 0.0f ||
             rotationVelocity.y != 0.0f ||
             rotationVelocity.z != 0.0f ) {
                rotationChanged = true;
             
                Vector tempV;
                Quaternion tempQ;
	
                tempV.x = rotationVelocity.x * timeElapsed;
                tempV.y = rotationVelocity.y * timeElapsed;
                tempV.z = rotationVelocity.z * timeElapsed;
	             
                tempQ.buildFromEuler(tempV);	
                rotation = rotation * tempQ;
        }
    }
    else {
        rotationChanged = true;
        
        float endVal = valInterpEnd_ - valInterpBegin_;
        float curVal = valInterpCurrent_ - valInterpBegin_;
              
        float t = 0.0f;
                                              
        if ( endVal > 0 ) {
            if ( curVal > endVal )
                t = 1.0f;
            else if ( curVal < 0.0f )
                t = 0.0f;
            else
                t = curVal / endVal; 
        }
        else if ( endVal < 0 ) {
            if ( curVal < endVal )
                t = 1.0f;
            else if ( curVal > 0.0f )
                t = 0.0f;
            else
                t = curVal / endVal; 
        }
                                
        rotation.slerp(rInterpStart, t, rInterpEnd );
    }
    /////////////////////////////////////////////
    
    color[3] += timeElapsed * fadeRate;
        
    if ( color[3] > 1.0f )
        color[3] = 1.0f;
    else if ( color[3] < 0.0f )
        color[3] = 0.0f;
}

//------------------------------------------------------------------------------
/*
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
*/
