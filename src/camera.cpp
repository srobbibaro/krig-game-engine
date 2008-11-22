//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum     8  amyst14                    //
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
   if ( scriptNum >= 0 )
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
int Camera::processExtendedCommand( const ScriptCommand &t )
{  
    int next = 1;
    /*
    //cout << "Camera extended routine: " << t.routine << endl << endl;
    int index1, index2, index3, index4, op;
   
    switch ( t.routine )
    {
         case SCRIPT_SET_PLR_POSITION_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                player->setPosition(memVars[index1], 
                            memVars[index2],
                            memVars[index3]);
            }
            break;
        case SCRIPT_SET_PLR_ROTATION_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                player->setRotationEuler(   memVars[index1], 
                                            memVars[index2],
                                            memVars[index3]);
            }
            break;
        case SCRIPT_SET_PLR_VELOCITY_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                player->setVelocity(memVars[index1], 
                                    memVars[index2],
                                    memVars[index3]);
            }
            break;
        case SCRIPT_SET_PLR_SPEED_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN) 
            {
                player->speedDir = (int)memVars[index1];
                player->speed = memVars[index2];
            }
            break;
        case SCRIPT_SET_PLR_ROT_VEL_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                player->setRotationVelocity(memVars[index1], 
                                            memVars[index2],
                                            memVars[index3]);
            }
            break;
        
        case SCRIPT_SET_PLR_POSITION_VAL:
            // t.p1 - val1
            // t.p2 - val2
            // t.p3 - val3
            if ( player != NULL ) {
                player->setPosition(t.p1, t.p2, t.p3);
            }
                    
            break;
        case SCRIPT_SET_PLR_ROTATION_VAL:
            // t.p1 - val1
            // t.p2 - val2
            // t.p3 - val3
   
            if ( player != NULL ) {
                player->setRotationEuler(t.p1, t.p2, t.p3);
            }

            break;
        case SCRIPT_SET_PLR_VELOCITY_VAL:
            // t.p1 - val1
            // t.p2 - val2
            // t.p3 - val3
            if ( player != NULL ) {
                player->setVelocity(t.p1, t.p2, t.p3);
            }
            
            break;
        case SCRIPT_SET_PLR_SPEED_VAL:
            // t.p1 - val1
            
            if ( player != NULL ) {
                player->speedDir = (int)t.p1;
                player->speed = t.p2;
            }
            
            break;
        case SCRIPT_SET_PLR_ROT_VEL_VAL:
            // t.p1 - val1
            // t.p3 - val2
            // t.p4 - val3
       
            if ( player != NULL ) {
                player->setRotationVelocity(t.p1, t.p2, t.p3);
            }
            
            break;
   
        case SCRIPT_ADD_PLR_POSITION_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                player->position += Vector( memVars[index1], 
                                    memVars[index2],
                                    memVars[index3]);
            }
            break;
        case SCRIPT_ADD_PLR_ROTATION_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                Quaternion t;
                t.buildFromEuler(memVars[index1], memVars[index2], memVars[index3]);
                
                player->rotation = rotation * t;
            }
            break;
        case SCRIPT_ADD_PLR_VELOCITY_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                player->velocity += Vector( memVars[index1], 
                                    memVars[index2],
                                    memVars[index3]);
            }
            break;
        case SCRIPT_ADD_PLR_SPEED_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL && index1 >= 0 && index1 < MEM_LEN) {
                player->speed += memVars[index1];
            }
            break;
        case SCRIPT_ADD_PLR_ROT_VEL_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                player->rotationVelocity += Vector( memVars[index1], 
                                            memVars[index2],
                                            memVars[index3]);
            }
            break;
      
        case SCRIPT_ADD_PLR_POSITION_VAL:
            if ( player != NULL ) {
                player->position += Vector(t.p1, t.p2, t.p3);
            }
            break;
        case SCRIPT_ADD_PLR_ROTATION_VAL:
        {
            if ( player != NULL ) {
                Quaternion qt;
                qt.buildFromEuler(t.p1, t.p2, t.p3);
                player->rotation = rotation * qt;
            }
            break;
        }
        case SCRIPT_ADD_PLR_VELOCITY_VAL:
            if ( player != NULL ) {
                player->velocity += Vector(t.p1, t.p2, t.p3);
            }
            break;
        case SCRIPT_ADD_PLR_SPEED_VAL:
            if ( player != NULL ) {
                player->speed += t.p1;
            }
            break;
        case SCRIPT_ADD_PLR_ROT_VEL_VAL:
            if ( player != NULL ) {
                player->rotationVelocity += Vector( t.p1, t.p2, t.p3 );
            }
            break;
        case SCRIPT_SET_PLR_ROT_VEL_AXIS_MEM: 
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            index4 = (int)t.p4;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN &&
                index4 >= 0 && index4 < MEM_LEN)
            {
                player->rotationVelocity.setVector(memVars[index1], memVars[index2], memVars[index3]);
                player->rotationVelocity.normalize();
                player->rotationVelocity.scale(memVars[index4]);
            }  
            break;          
        case SCRIPT_SET_PLR_ROT_VEL_AXIS_VAL:  
            if ( player != NULL ) {
                player->rotationVelocity.setVector(t.p1, t.p2, t.p3);
                player->rotationVelocity.normalize();
                player->rotationVelocity.scale(t.p4); 
            }
            break;     

        case SCRIPT_SET_PLR_ROTATION_AXIS_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            index4 = (int)t.p4;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN &&
                index4 >= 0 && index4 < MEM_LEN)
            {
                Vector tv;
                tv.setVector(memVars[index1], memVars[index2], memVars[index3]);
                player->rotation.buildFromAxis(tv, memVars[index4]);
            }  
            break;          
        case SCRIPT_SET_PLR_ROTATION_AXIS_VAL: 
        {
            if ( player != NULL ) {
                Vector tv;
                tv.setVector(t.p1, t.p2, t.p3);
                tv.normalize();
            
                player->rotation.buildFromAxis(tv, t.p4);
            }
            break;
        }               

        case SCRIPT_ADD_PLR_ROTATION_AXIS_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            index4 = (int)t.p4;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN &&
                index4 >= 0 && index4 < MEM_LEN)
            {
                Quaternion tq;
                Vector tv;
                tv.setVector(memVars[index1], memVars[index2], memVars[index3]);
                tv.normalize();
                tq.buildFromAxis(tv, memVars[index4]);
                
                player->rotation = player->rotation * tq;
            }
            break;        
        case SCRIPT_ADD_PLR_ROTATION_AXIS_VAL:
        {
            if ( player != NULL ) {
                Quaternion tq;
                Vector tv;
                tv.setVector(t.p1, t.p2, t.p3);
                tv.normalize();
                tq.buildFromAxis(tv, t.p4);
                
                player->rotation = player->rotation * tq;
            }
            break;               
        }  
    }
    
    */
    
    return( next );
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
