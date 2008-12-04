//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : camera.cpp                                     //
//                                                              //
// Description : Functions for managing the camera position.    //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////

#include "Camera.h"


//------------------------------------------------------------------------------
Camera::Camera( void ) : ScriptedObject()
{
      // default camera settings
      baseDirection.setVector(0.0f, 0.0f, -1.0f);
      direction.setVector(0.0f, 0.0f, -1.0f);
      
      glGetFloatv( GL_PROJECTION_MATRIX, projectionMatrix.data );
}

//------------------------------------------------------------------------------
Camera::~Camera( void )
{
}

//------------------------------------------------------------------------------
void Camera::update( float timeElapsed)
{   
      animateScript(timeElapsed);

         // calculate new position and orientation //
            if ( speed == 0 ) {
                position.x += velocity.x * timeElapsed;   
                position.y += velocity.y * timeElapsed;   
                position.z += velocity.z * timeElapsed;   
            }
            else {
                switch (speedDir)
                {
                    case 0:
                        direction.scale(speed*timeElapsed);
                        position.x += direction.x;
                        position.y += direction.y;
                        position.z += direction.z;
                        direction.normalize();
                        break;
                    case 1:
                    {
                        Vector rotationAxis;
            
                        rotationAxis.crossProduct(up, direction);
                        rotationAxis.normalize();
                        
                        rotationAxis.scale(speed*timeElapsed);
                        position.x += rotationAxis.x;
                        position.y += rotationAxis.y;
                        position.z += rotationAxis.z;
                        
                        break;
                    }
                    case 2:
                        up.scale(speed*timeElapsed);
                        position.x += up.x;
                        position.y += up.y;
                        position.z += up.z;
                        direction.normalize();
                        break;
                }
            }
            
        if (!interp) {
                if ( 
                    rotationVelocity.x != 0.0f ||
                    rotationVelocity.y != 0.0f ||
                    rotationVelocity.z != 0.0f 
                ) {
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
                float endVal = valInterpEnd - valInterpStart;
                float curVal = *valInterpPtr - valInterpStart;
              
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
        
        
    rotation.buildRotationMatrix( rotationMatrix );
        
    direction.rotateVector( rotationMatrix, baseDirection );
    direction.normalize();
    
    Vector upV;
    upV.setVector(0.0f, 1.0f, 0.0f);
    
    up.rotateVector( rotationMatrix, upV );
    up.normalize();
    
    Matrix translationMatrix;
    translationMatrix.setTranslation(-position.x, -position.y, -position.z);
    
    modelViewMatrix = rotationMatrix * translationMatrix;
    
    
    final = projectionMatrix * modelViewMatrix;
}

//------------------------------------------------------------------------------
void Camera::setCamera( 
    const Vector &tPos, const Vector &tVel, 
    const Quaternion &tRot, const Vector &tRotVel 
)
{
    position = tPos;
    rotation = tRot;
    velocity = tVel;
    rotationVelocity = tRotVel;
}

//------------------------------------------------------------------------------
void Camera::prepareGLView( void )
{
    glMultMatrixf(rotationMatrix.data);
    //glTranslatef(-position.x, -position.y, -position.z);
}

//------------------------------------------------------------------------------
void Camera::getRotationMatrix( Matrix &m ) const
{
    m = rotationMatrix;
}

//------------------------------------------------------------------------------
void Camera::getFrustum(Frustum &f)
{
    //f.setFrustum(
}

//------------------------------------------------------------------------------
void Camera::draw(void)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPushMatrix();
    glBegin(GL_LINES);
    glVertex3f(position.x, position.y, position.z);
    glVertex3f(position.x, position.y-(up.y*12.0f), position.z);
    
    glEnd();
            
    glPopMatrix();
}
