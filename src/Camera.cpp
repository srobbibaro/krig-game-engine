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
Camera::Camera( void ) : Object()
{
    // default camera settings
    baseDirection.setVector(0.0f, 0.0f, -1.0f);
    direction.setVector(0.0f, 0.0f, -1.0f);
      
    glGetFloatv( GL_PROJECTION_MATRIX, projectionMatrix.data );
}

Camera::Camera( int tid ) : Object()
{
    // default camera settings
    baseDirection.setVector(0.0f, 0.0f, -1.0f);
    direction.setVector(0.0f, 0.0f, -1.0f);
      
    glGetFloatv( GL_PROJECTION_MATRIX, projectionMatrix.data );
    
    id = tid;
}

//------------------------------------------------------------------------------
Camera::~Camera( void )
{}

//------------------------------------------------------------------------------
void Camera::update( float timeElapsed)
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
    
    if (particleSystem != NULL)
        particleSystem->update(timeElapsed);
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
void Camera::draw(Object* hey)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPushMatrix();
    glBegin(GL_LINES);
    glVertex3f(position.x, position.y, position.z);
    glVertex3f(position.x, position.y-(up.y*12.0f), position.z);
    
    glEnd();
            
    glPopMatrix();
    
    if (particleSystem != NULL)
        particleSystem->draw();
}
