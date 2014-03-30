//////////////////////////////////////////////////////////////////
// Description : Functions for displaying stroke fonts and      //
//               bitmap fonts.                                  //
//////////////////////////////////////////////////////////////////
#include <GL/glut.h>
#include "text.h"
#include "constants.h"

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
    if ( isInView_ ) {
        glEnable(GL_BLEND);
        glPushMatrix();
            glColor4fv( color );
            glLineWidth(width);
            //glTranslatef(position.x, position.y, position.z );
            glRasterPos3f(position_.x, position_.y, position_.z);
            glScalef (scale_.x, scale_.y, scale_.z);
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
    if (speed_.x != 0.0f) {
        direction_.scale(speed_.x * timeElapsed);
        position_.x += direction_.x;
        position_.y += direction_.y;
        position_.z += direction_.z;
        direction_.normalize();
    }

    if (speed_.y != 0.0f) {
        up_.scale(speed_.y * timeElapsed);
        position_.x += up_.x;
        position_.y += up_.y;
        position_.z += up_.z;
        direction_.normalize();
    }

    if (speed_.z != 0.0f) {
        Vector rotationAxis;

        rotationAxis.crossProduct(up_, direction_);
        rotationAxis.normalize();

        rotationAxis.scale(speed_.z * timeElapsed);
        position_.x += rotationAxis.x;
        position_.y += rotationAxis.y;
        position_.z += rotationAxis.z;
    }

    // update position using velocity
    if (velocity_.x != 0.0f)
        position_.x += velocity_.x * timeElapsed;

    if (velocity_.y != 0.0f)
        position_.y += velocity_.y * timeElapsed;

    if (velocity_.z != 0.0f)
        position_.z += velocity_.z * timeElapsed;

    // update scale
    if (scaleRate_.x != 0.0f) {
        scale_.x += scaleRate_.x * timeElapsed;
        scaleChanged_ = true;
    }

    if (scaleRate_.y != 0.0f) {
        scale_.y += scaleRate_.y * timeElapsed;
        scaleChanged_ = true;
    }

    if (scaleRate_.z != 0.0f) {
        scale_.z += scaleRate_.z * timeElapsed;
        scaleChanged_ = true;
    }

    if (!isInterpolationEnabled_) {
        if ( rotationVelocity_.x != 0.0f ||
             rotationVelocity_.y != 0.0f ||
             rotationVelocity_.z != 0.0f ) {
                rotationChanged_ = true;

                Vector tempV;
                Quaternion tempQ;

                tempV.x = rotationVelocity_.x * timeElapsed;
                tempV.y = rotationVelocity_.y * timeElapsed;
                tempV.z = rotationVelocity_.z * timeElapsed;

                tempQ.buildFromEuler(tempV);
                rotation_ = rotation_ * tempQ;
        }
    }
    else {
        rotationChanged_ = true;

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

        rotation_.slerp(rInterpStart_, t, rInterpEnd_ );
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
    PRINT_DEBUG("Routine=%d\n", t.routine);

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

            PRINT_DEBUG("Size=%d\n", (int)t.p1);

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
