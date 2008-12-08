#include "ObjectInclude.h"
#include "ScriptedObject.h"

//------------------------------------------------------------------------------
ScriptedObject::ScriptedObject()
: Model( NO_SCRIPT )
{
    load( MODEL_BOSS_A );
    
    //#animCurrTime = NULL;
    
    //#life = 40;
    
    //#isEnemy = true;  
    
    //#shotDelay = 0.5f;
    //#numShots = 1;   
}

//------------------------------------------------------------------------------
ScriptedObject::ScriptedObject( string script )
: Model( script )
{ 
    //load( tModelKey );
    
    //#animCurrTime = tTime;
    
    //#life = 40;
    
    //#isEnemy = true;  
    
    //#shotDelay = 0.5f;
    //#numShots = 1;   
}

//------------------------------------------------------------------------------
ScriptedObject::~ScriptedObject()
{
}

//------------------------------------------------------------------------------
void ScriptedObject::processAI( Object* c, float elapsedTime )
{     
   animateScript(elapsedTime);
    if (particleSystem != NULL)
        particleSystem->update(elapsedTime);
}

