#include "ScriptedObject.h"

//------------------------------------------------------------------------------
ScriptedObject::ScriptedObject( void )
{
    rotation.loadMultIdentity();
    baseDirection.setVector( 0.0f, 0.0f, 1.0f );
    direction.setVector( 0.0f, 0.0f, 1.0f );
    up.setVector(0.0f, 1.0f, 0.0f);
          
    speed = 0.0f;
    speedDir = 0;
            
    rInterpStart.loadMultIdentity();
    rInterpEnd.loadMultIdentity();
    
    tieMemVarIndex = 0;
    valInterpStart = 0.0f;
    valInterpEnd = 0.0f;
    interp = false;
        
    scriptLastTime = 0.0f;
    //scriptNum = NO_SCRIPT;
    scriptName = NO_SCRIPT;
    currentScriptCommand = 0;
    
    animCurrTime = NULL;  
    player = NULL;
    camera = NULL;
    
    L = NULL;
   
    //memVars[MEM_NEXT_SCRIPT_INDEX] = NO_SCRIPT;
    //memVars[MEM_NEXT_SCRIPT_COMMAND_INDEX] = 0;
}

//------------------------------------------------------------------------------
ScriptedObject::~ScriptedObject( void )
{
    if (L != NULL)
        lua_close(L);
}

void ScriptedObject::setLuaState(lua_State* tl)
{
}

//------------------------------------------------------------------------------
void ScriptedObject::setPosition( GLfloat x, GLfloat y, GLfloat z )
{ position.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void ScriptedObject::setPosition( Vector t )
{ position = t; }
 
//------------------------------------------------------------------------------ 
void ScriptedObject::setRotationAxis( GLfloat vx, GLfloat vy, GLfloat vz, GLfloat vw )
{
    rotation.buildFromAxis( Vector( vx, vy, vz ), vw );
}

//------------------------------------------------------------------------------
void ScriptedObject::setRotationAxis( const Vector &v, GLfloat a )
{
    rotation.buildFromAxis( v, a );
}

//------------------------------------------------------------------------------
void ScriptedObject::setRotationEuler( GLfloat x, GLfloat y, GLfloat z )
{
    rotation.buildFromEuler( x, y, z );
}

//------------------------------------------------------------------------------
void ScriptedObject::setRotationEuler( const Vector &v )
{
    rotation.buildFromEuler( v );
}

//------------------------------------------------------------------------------            
void ScriptedObject::setRotationQuaternion( const Quaternion &q )
{
    rotation = q;
}

//------------------------------------------------------------------------------
void ScriptedObject::setRotationVelocity( GLfloat xAngle, GLfloat yAngle, GLfloat zAngle )
{
    rotationVelocity.setVector( xAngle, yAngle, zAngle );
}

//------------------------------------------------------------------------------            
void ScriptedObject::setRotationVelocity( const Vector &v )
{
    rotationVelocity = v; 
}

//------------------------------------------------------------------------------
void ScriptedObject::setVelocity( GLfloat x, GLfloat y, GLfloat z )
{ velocity.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void ScriptedObject::setVelocity( const Vector &v )
{
    velocity = v;
}      

//------------------------------------------------------------------------------
void ScriptedObject::setTimer( float* temp )
{
    animCurrTime = temp;   
}

//------------------------------------------------------------------------------
void ScriptedObject::setScript( int script )
{
    // save off last values for jump back...
    //memVars[MEM_NEXT_SCRIPT_INDEX] = (float)scriptNum;
    //memVars[MEM_NEXT_SCRIPT_COMMAND_INDEX] = (float)currentScriptCommand;

    // update internally to the new script and line number
    // script must start on 0...
    scriptNum = script;
    currentScriptCommand = 0;    
}

//------------------------------------------------------------------------------
int ScriptedObject::processBasicCommand( const ScriptCommand &t )
{
   int next = 1;
   /*
   //cout << "routine: " << t.routine << endl << endl;
   int index1, index2, index3, index4, op;
   
    switch (t.routine)
    {
        // perform no operation //
        case SCRIPT_NO_OP:
            break;
            
        // arithmetic operations //
        case SCRIPT_ARITH_VAL_AOP_VAL:
            index1 = (int)t.p1;
            // t.p2 val1
            op     = (int)t.p3;  
            // t.p4 val2
            
            if (index1 >= 0 && index1 < MEM_LEN) {
                memVars[index1] = performArithmeticOperation(t.p2, op, t.p4);
            }
            break;
        case SCRIPT_ARITH_VAL_AOP_MEM:
            index1 = (int)t.p1;
            // t.p2 val1
            op     = (int)t.p3;
            index3 = (int)t.p4;
      
            if (index1 >= 0 && index1 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN) 
            {
                memVars[index1] = performArithmeticOperation(t.p2, op, memVars[index3]);
            }
            break;
        case SCRIPT_ARITH_MEM_AOP_VAL:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            op     = (int)t.p3;
            // t.p4 val2
      
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN ) 
            {
                 
                memVars[index1] = performArithmeticOperation(memVars[index2], op, t.p4);
            }
            break;
        case SCRIPT_ARITH_MEM_AOP_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            op     = (int)t.p3;
            index3 = (int)t.p4;
      
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index3 < MEM_LEN &&
                index3 >= 0 && index2 < MEM_LEN )
            {
                memVars[index1] = performArithmeticOperation(memVars[index2], op, memVars[index3]);
            }
            break;
   
        // branch to a relative script command based on outcome of boolean operation //
        case SCRIPT_BRANCH_VAL_REL_VAL:
            // t.p1 (int) relative new script command index
            // t.p2 val1
            op  = (int)t.p3;
            // t.p4 val2
      
            if (evaluateBranchCondition(t.p2, op, t.p4))
                next = (int)t.p1;
            
            break;  
        case SCRIPT_BRANCH_VAL_REL_MEM:
            // t.p1 (int) relative new script command index
            // t.p2 val1
            op     = (int)t.p3;
            index3 = (int)t.p4;
      
            if (index3 >= 0 && index3 < MEM_LEN) {
                if (evaluateBranchCondition(t.p2, op, memVars[index3]))
                    next = (int)t.p1;
            }
            break;
       case SCRIPT_BRANCH_MEM_REL_VAL:
            // t.p1 (int) relative new script command index
            index2 = (int)t.p2;
            op     = (int)t.p3;
            // t.p4 val2
      
            if (index2 >= 0 && index2 < MEM_LEN) {
                if (evaluateBranchCondition(memVars[index2], op, t.p4))
                    next = (int)t.p1;
            }
            break;
        case SCRIPT_BRANCH_MEM_REL_MEM:
            // t.p1 (int) relative new script command index
            index2 = (int)t.p2;
            op     = (int)t.p3;
            index3 = (int)t.p4;
      
            if (index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                if (evaluateBranchCondition(memVars[index2], op, memVars[index3]))
                    next = (int)t.p1;
            }
            break;
        
        // store data into memory //
        case SCRIPT_STORE_VAL_IN_MEM:
            // t.p1 - value to store into memory
            index1 = (int)t.p1;
            
            if (index1 >= 0 && index1 < MEM_LEN) {
                memVars[index1] = t.p2;
            }
            break;
        case SCRIPT_STORE_MEM_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
      
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN)
            {
                memVars[index1] = memVars[index2];
            }
            break;
        
        // jump to another script command index //   
        case SCRIPT_JUMP:
            next = (int)t.p1;
            break;
    
        // load another script to execute //
        case SCRIPT_SET_SCRIPT:
            setScript((int)t.p1);
            next = 0;
            break;

        // extended basic // 
        case SCRIPT_SET_POSITION_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                setPosition(memVars[index1], 
                            memVars[index2],
                            memVars[index3]);
            }
            break;
        case SCRIPT_SET_ROTATION_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                setRotationEuler(   memVars[index1], 
                                    memVars[index2],
                                    memVars[index3]);
            }
            break;
        case SCRIPT_SET_VELOCITY_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                setVelocity(memVars[index1], 
                            memVars[index2],
                            memVars[index3]);
            }
            break;
        case SCRIPT_SET_SPEED_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >=0 && index2 < MEM_LEN) 
            {
                speedDir = (int)memVars[index1];
                speed = memVars[index2];
            }
            break;
        case SCRIPT_SET_ROT_VEL_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                setRotationVelocity(memVars[index1], 
                                    memVars[index2],
                                    memVars[index3]);
            }
            break;
        
        case SCRIPT_SET_POSITION_VAL:
            // t.p1 - val1
            // t.p2 - val2
            // t.p3 - val3
            setPosition(t.p1, t.p2, t.p3);
            
            break;
        case SCRIPT_SET_ROTATION_VAL:
            // t.p1 - val1
            // t.p2 - val2
            // t.p3 - val3
   
            setRotationEuler(t.p1, t.p2, t.p3);

            break;
        case SCRIPT_SET_VELOCITY_VAL:
            // t.p1 - val1
            // t.p2 - val2
            // t.p3 - val3
       
            setVelocity(t.p1, t.p2, t.p3);
            
            break;
        case SCRIPT_SET_SPEED_VAL:
            // t.p1 - val1
            
            speedDir = (int)t.p1;
            speed = t.p2;
            
            break;
        case SCRIPT_SET_ROT_VEL_VAL:
            // t.p1 - val1
            // t.p3 - val2
            // t.p4 - val3
       
            setRotationVelocity(t.p1, t.p2, t.p3);
            
            break;
   
        case SCRIPT_ADD_POSITION_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                position += Vector( memVars[index1], 
                                    memVars[index2],
                                    memVars[index3]);
            }
            break;
        case SCRIPT_ADD_ROTATION_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                Quaternion t;
                t.buildFromEuler(memVars[index1], memVars[index2], memVars[index3]);
                
                rotation = rotation * t;
            }
            break;
        case SCRIPT_ADD_VELOCITY_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                velocity += Vector( memVars[index1], 
                                    memVars[index2],
                                    memVars[index3]);
            }
            break;
        case SCRIPT_ADD_SPEED_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN) {
                speed += memVars[index1];
            }
            break;
        case SCRIPT_ADD_ROT_VEL_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                rotationVelocity += Vector( memVars[index1], 
                                            memVars[index2],
                                            memVars[index3]);
            }
            break;
      
        case SCRIPT_ADD_POSITION_VAL:
            position += Vector(t.p1, t.p2, t.p3);
            break;
        case SCRIPT_ADD_ROTATION_VAL:
        {
            Quaternion qt;
            qt.buildFromEuler(t.p1, t.p2, t.p3);
            rotation = rotation * qt;
        
            break;
        }
        case SCRIPT_ADD_VELOCITY_VAL:
            velocity += Vector(t.p1, t.p2, t.p3);
            break;
        case SCRIPT_ADD_SPEED_VAL:
            speed += t.p1;
            break;
        case SCRIPT_ADD_ROT_VEL_VAL:
            rotationVelocity += Vector( t.p1, t.p2, t.p3 );
            break;
      
        case SCRIPT_STORE_POSITION_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = position.x;
                memVars[index2] = position.y;
                memVars[index3] = position.z;
            }
            break;
        case SCRIPT_STORE_ROTATION_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                Vector tv;
                rotation.getEulerAngles(tv);
                
                memVars[index1] = tv.x;
                memVars[index2] = tv.y;
                memVars[index3] = tv.z;
            }
            break;
        case SCRIPT_STORE_VELOCITY_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = velocity.x;
                memVars[index2] = velocity.y;
                memVars[index3] = velocity.z;
            }
            break;
        case SCRIPT_STORE_SPEED_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN) {
                memVars[index1] = (float)speedDir;
                memVars[index2] = speed;
            }
            break;
        case SCRIPT_STORE_ROT_VEL_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = rotationVelocity.x;
                memVars[index2] = rotationVelocity.y;
                memVars[index3] = rotationVelocity.z;
            }
            break;
        case SCRIPT_STORE_TIMER_IN_MEM:
            index1 = (int)t.p1;
       
            if (index1 >= 0 && index1 < MEM_LEN) {
                memVars[index1] = *animCurrTime;
            }
            break;
        
        case SCRIPT_SET_INTERP_ROTATION_START_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN )
            {
                rInterpStart.buildFromEuler(memVars[index1], memVars[index2], memVars[index3]);
            }
            break;
        case SCRIPT_SET_INTERP_ROTATION_START_VAL:
            rInterpStart.buildFromEuler( t.p1, t.p2, t.p3 );
            break;       
        case SCRIPT_SET_INTERP_ROTATION_END_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN )
            {
                rInterpEnd.buildFromEuler(memVars[index1], memVars[index2], memVars[index3]);
            }
            break;
        case SCRIPT_SET_INTERP_ROTATION_END_VAL:
            rInterpEnd.buildFromEuler( t.p1, t.p2, t.p3 );
            break;
        case SCRIPT_SET_INTERP_VAR_VAL_VAL:
            index1 = (int)t.p1;
        
            setInterpolationVariable(index1);
            
            valInterpStart = t.p2;
            valInterpEnd = t.p3;
        
            break; 
        case SCRIPT_SET_INTERP_VAR_VAL_MEM:
            index1 = (int)t.p1;
            // t.p2 val1
            index3 = (int)t.p3;
            
            setInterpolationVariable(index1);
            valInterpStart = t.p2;
            
            if ( index3 >= 0 && index3 < MEM_LEN ) {
                valInterpEnd = memVars[index3];
            }
        
            break; 
        case SCRIPT_SET_INTERP_VAR_MEM_VAL:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            //t.p3 val2
            
            setInterpolationVariable(index1);
            valInterpEnd = t.p3;
            
            if ( index2 >= 0 && index2 < MEM_LEN ) {
                valInterpStart = memVars[index2];
            }
            
            break;
        case SCRIPT_SET_INTERP_VAR_MEM_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            setInterpolationVariable(index1);
            
            if ( index2 >= 0 && index2 < MEM_LEN ) {
                valInterpStart = memVars[index2];
            }
            
            if ( index3 >=0 && index3 < MEM_LEN ) {
                valInterpEnd = memVars[index3];
            }
        
            break; 
            
        case SCRIPT_SET_INTERP_ENABLE:
            op = (int)t.p1;
            
            if (op == 0)
                interp = false;
            else
                interp = true;
            
            index1 = (int) t.p2;
            index2 = (int) t.p3;
            
            //cout << "interp set:" << interp << endl;
            
            //if (interp) {
            //    if ( index1 == 1 )
            //        rInterpStart = rotation;
            //    
            //    if ( index2 == 1 )
            //        valInterpStart = *valInterpPtr;
            //}
            
            break;
            
        case SCRIPT_STORE_PLR_POSITION_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = player->position.x;
                memVars[index2] = player->position.y;
                memVars[index3] = player->position.z;
            }
            break;
        case SCRIPT_STORE_PLR_ROTATION_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                Vector tv;
                player->rotation.getEulerAngles(tv);
                
                memVars[index1] = tv.x;
                memVars[index2] = tv.y;
                memVars[index3] = tv.z;            
            }
            break;
        case SCRIPT_STORE_PLR_VELOCITY_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = player->velocity.x;
                memVars[index2] = player->velocity.y;
                memVars[index3] = player->velocity.z;
            }
            break;
        case SCRIPT_STORE_PLR_SPEED_IN_MEM:
            index1 = (int)t.p1;
            
            if (player != NULL && index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = player->speed;
            }
            break;
        case SCRIPT_STORE_PLR_ROT_VEL_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = player->rotationVelocity.x;
                memVars[index2] = player->rotationVelocity.y;
                memVars[index3] = player->rotationVelocity.z;
            }
            break;

        case SCRIPT_STORE_CAM_POSITION_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (camera != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = camera->position.x;
                memVars[index2] = camera->position.y;
                memVars[index3] = camera->position.z;
            }
            break;
        case SCRIPT_STORE_CAM_ROTATION_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                Vector tv;
                camera->rotation.getEulerAngles(tv);
                
                memVars[index1] = tv.x;
                memVars[index2] = tv.y;
                memVars[index3] = tv.z;            
            }
            break;
        case SCRIPT_STORE_CAM_VELOCITY_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (camera != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = camera->velocity.x;
                memVars[index2] = camera->velocity.y;
                memVars[index3] = camera->velocity.z;
            }
            break;
        case SCRIPT_STORE_CAM_SPEED_IN_MEM:
            index1 = (int)t.p1;
            
            if (camera != NULL && index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = player->speed;
            }
            break;
        case SCRIPT_STORE_CAM_ROT_VEL_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (camera != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = camera->rotationVelocity.x;
                memVars[index2] = camera->rotationVelocity.y;
                memVars[index3] = camera->rotationVelocity.z;
            }
            break;
            
        case SCRIPT_STORE_RND_IN_MEM:
            index1 = (int)t.p1;
            
            if ( index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = (rand() % (int)t.p2);
            }
            break;          
        case SCRIPT_STORE_SIN_IN_MEM:
            index1 = (int)t.p1;
            
            if ( index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = sin(t.p2);    
            }
            break;  
        case SCRIPT_STORE_COS_IN_MEM:
            index1 = (int)t.p1;
            
            if ( index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = cos(t.p2);    
            }
            break;
        case SCRIPT_STORE_TAN_IN_MEM:
            index1 = (int)t.p1;
            
            if ( index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = tan(t.p2);    
            }
            break;
        case SCRIPT_STORE_ASIN_IN_MEM:
            index1 = (int)t.p1;
            
            if ( index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = asin(t.p2);    
            }
            break;
        case SCRIPT_STORE_ACOS_IN_MEM:
            index1 = (int)t.p1;
            
            if ( index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = acos(t.p2);    
            }
            break;
        case SCRIPT_STORE_ATAN_IN_MEM:
            index1 = (int)t.p1;
            
            if ( index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = atan(t.p2);    
            }
            break;
            
        case SCRIPT_STORE_DIR_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = direction.x;
                memVars[index2] = direction.y;
                memVars[index3] = direction.z;
            }
            break;              
        case SCRIPT_STORE_PLR_DIR_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = player->direction.x;
                memVars[index2] = player->direction.y;
                memVars[index3] = player->direction.z;
            }
            break;
        case SCRIPT_STORE_CAM_DIR_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (camera != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = camera->direction.x;
                memVars[index2] = camera->direction.y;
                memVars[index3] = camera->direction.z;
            }
            break;
        case SCRIPT_STORE_ORTH_DIR_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                 Vector rotationAxis;
            
                rotationAxis.crossProduct(up, direction);
                rotationAxis.normalize();
            
                memVars[index1] = rotationAxis.x;
                memVars[index2] = rotationAxis.y;
                memVars[index3] = rotationAxis.z;
            }
            break;              
        case SCRIPT_STORE_PLR_ORTH_DIR_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                Vector rotationAxis;
            
                rotationAxis.crossProduct(player->up, player->direction);
                rotationAxis.normalize();
            
                memVars[index1] = rotationAxis.x;
                memVars[index2] = rotationAxis.y;
                memVars[index3] = rotationAxis.z;
            }
            break;
        case SCRIPT_STORE_CAM_ORTH_DIR_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (camera != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                Vector rotationAxis;
            
                rotationAxis.crossProduct(camera->up, camera->direction);
                rotationAxis.normalize();
            
                memVars[index1] = rotationAxis.x;
                memVars[index2] = rotationAxis.y;
                memVars[index3] = rotationAxis.z;
            }
            break;  

        case SCRIPT_STORE_UP_DIR_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = up.x;
                memVars[index2] = up.y;
                memVars[index3] = up.z;
            }
            break;              
        case SCRIPT_STORE_PLR_UP_DIR_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (player != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = player->up.x;
                memVars[index2] = player->up.y;
                memVars[index3] = player->up.z;
            }
            break;
        case SCRIPT_STORE_CAM_UP_DIR_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
       
            if (camera != NULL &&
                index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN)
            {
                memVars[index1] = camera->up.x;
                memVars[index2] = camera->up.y;
                memVars[index3] = camera->up.z;
            }
            break; 

        case SCRIPT_SET_ROT_VEL_AXIS_MEM: 
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            index4 = (int)t.p4;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN &&
                index4 >= 0 && index4 < MEM_LEN)
            {
                rotationVelocity.setVector(memVars[index1], memVars[index2], memVars[index3]);
                rotationVelocity.normalize();
                rotationVelocity.scale(memVars[index4]);
            }  
            break;          
        case SCRIPT_SET_ROT_VEL_AXIS_VAL:  
            rotationVelocity.setVector(t.p1, t.p2, t.p3);
            rotationVelocity.normalize();
            rotationVelocity.scale(t.p4); 
            break;     

        case SCRIPT_SET_ROTATION_AXIS_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            index4 = (int)t.p4;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN &&
                index4 >= 0 && index4 < MEM_LEN)
            {
                Vector tv;
                tv.setVector(memVars[index1], memVars[index2], memVars[index3]);
                rotation.buildFromAxis(tv, memVars[index4]);
            }  
            break;          
        case SCRIPT_SET_ROTATION_AXIS_VAL: 
        {
            Vector tv;
            tv.setVector(t.p1, t.p2, t.p3);
            tv.normalize();
            
            rotation.buildFromAxis(tv, t.p4);
            break;
        }               

        case SCRIPT_ADD_ROTATION_AXIS_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            index4 = (int)t.p4;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN &&
                index4 >= 0 && index4 < MEM_LEN)
            {
                Quaternion tq;
                Vector tv;
                tv.setVector(memVars[index1], memVars[index2], memVars[index3]);
                tv.normalize();
                tq.buildFromAxis(tv, memVars[index4]);
                
                rotation = rotation * tq;
            }
            break;        
        case SCRIPT_ADD_ROTATION_AXIS_VAL:
        {
            Quaternion tq;
            Vector tv;
            tv.setVector(t.p1, t.p2, t.p3);
            tv.normalize();
            tq.buildFromAxis(tv, t.p4);
                
            rotation = rotation * tq;
            break;               
        }
        
        case SCRIPT_SET_INTERP_ROTATION_START_AXIS_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            index4 = (int)t.p4;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN &&
                index4 >= 0 && index4 < MEM_LEN)
            {
                Vector tv;
                tv.setVector(memVars[index1], memVars[index2], memVars[index3]);
                rInterpStart.buildFromAxis(tv, memVars[index4]);
            }  
            break;   
        case SCRIPT_SET_INTERP_ROTATION_START_AXIS_VAL:
        {
            Vector tv;
            tv.setVector(t.p1, t.p2, t.p3);
            tv.normalize();
            
            rInterpStart.buildFromAxis(tv, t.p4);
            break;
        }         
        case SCRIPT_SET_INTERP_ROTATION_END_AXIS_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            index4 = (int)t.p4;
       
            if (index1 >= 0 && index1 < MEM_LEN &&
                index2 >= 0 && index2 < MEM_LEN &&
                index3 >= 0 && index3 < MEM_LEN &&
                index4 >= 0 && index4 < MEM_LEN)
            {
                Vector tv;
                tv.setVector(memVars[index1], memVars[index2], memVars[index3]);
                rInterpEnd.buildFromAxis(tv, memVars[index4]);
            }  
            break;   
        case SCRIPT_SET_INTERP_ROTATION_END_AXIS_VAL:
        {
            Vector tv;
            tv.setVector(t.p1, t.p2, t.p3);
            tv.normalize();
            
            rInterpEnd.buildFromAxis(tv, t.p4);
            break;
        }         
    }  
   
   */
    return (next);
}

//------------------------------------------------------------------------------
void ScriptedObject::animateScript(void)
{
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (L == NULL)
        return;
    
    // Find the update function and call it
    lua_getglobal(L, "on_update");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)this);
	   
    // Call the function with 1 argument and no return values
    lua_call(L, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
}

//------------------------------------------------------------------------------
void ScriptedObject::setPlayerPtr( ScriptedObject* tPlayer )
{
    player = tPlayer;
}

//------------------------------------------------------------------------------
void ScriptedObject::setCameraPtr( ScriptedObject* tCamera)
{
    camera = tCamera;
}

//------------------------------------------------------------------------------
void ScriptedObject::setInterpolationVariable(int index)
{
    switch (index)
    {
        case 0:
            valInterpPtr = animCurrTime;
            break;
        case 1:
            valInterpPtr = &position.x;
            break;
        case 2:
            valInterpPtr = &position.y;
            break;
        case 3:
            valInterpPtr = &position.z;
            break;
        default:
            valInterpPtr = animCurrTime;
    }
}

void ScriptedObject::setScriptName( string name ) 
{
    scriptName = name;
}

void ScriptedObject::loadScript(string name)
{
    scriptName = name;
    
    // If the lua state has not been initialized for this object, attempt to
    // initialize it.     
    if (scriptName == "" || L != NULL)
        return;
        
    L = lua_open();
    
    // If the lua state still could not be initialized, then exit the game.
    // ... we can do something smarter with this in the finished product.
    if (L == NULL) {
        printf("Error creating Lua state.\n");
	    exit(-1);
    }

    // Load the base lua libraries
    luaL_openlibs(L);
        
    // Register our functions for use in lua (currently defined in 
    // ScriptedObject.h)
    lua_register(L, "setPosition", setPositionLua);
    lua_register(L, "getPosition", getPositionLua);
    lua_register(L, "setVelocity", setVelocityLua);
    lua_register(L, "getVelocity", getVelocityLua);
    lua_register(L, "setRotationVelocity", setRotationVelocityLua);
    lua_register(L, "getCamera", getCamera);
    lua_register(L, "getPlayer", getPlayer);
    
    // Load this object's animation script
    luaL_dofile(L, scriptName.c_str());
        
    // Set player and camera pointers for use within exposed c functions.
    lplayer = player;
    lcamera = camera;  
        
    // Find the update function and call it
    lua_getglobal(L, "on_load");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)this);
	   
    // Call the function with 1 argument and no return values
    lua_call(L, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
}

void ScriptedObject::unloadScript()
{
    // Attempt to execute the on_unload function only if the lua state has 
    // already been initialized with a script
    if (L == NULL)
        return;
    
    // Find the update function and call it
    lua_getglobal(L, "on_unload");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)this);
	   
    // Call the function with 1 argument and no return values
    lua_call(L, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
    
    lua_close(L);
    L = NULL;
}

