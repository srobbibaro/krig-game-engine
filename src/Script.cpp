#include <windows.h>
#include <math.h>

#include "constants.h"
#include "Script.h"

int numScripts;            // number of scripts
Script *script;            // storage of those scripts

//------------------------------------------------------------------------------
Script::Script()
{
    numCommands = 0;
    command = NULL;
}

//------------------------------------------------------------------------------
Script::Script( int tNC )
{
   initialize(tNC);
}

//------------------------------------------------------------------------------
Script::~Script( void )
{
   if ( command != NULL ) 
      delete [] command;
}

//------------------------------------------------------------------------------
bool Script::initialize( int tNC )
{
   bool result = false;
  
   return (result);
}

//------------------------------------------------------------------------------
bool Script::setCommand( int cmdNum,  const ScriptCommand &sc )
{
   bool result = false;
      
    return (result);   
}

//------------------------------------------------------------------------------
bool Script::getCommand( int index, ScriptCommand &t ) const
{
      return (false);
}

//------------------------------------------------------------------------------
int Script::getNumCommands( void ) const
{
   return (numCommands);
}

//------------------------------------------------------------------------------
bool evaluateBranchCondition(float v1, int op, float v2)
{
   bool result = false;
   
   return (result);
}

//------------------------------------------------------------------------------
float performArithmeticOperation(float v1, int op, float v2)
{
   float result = 0.0f;
   
   return (result);
}
