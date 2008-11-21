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
    
   if ( tNC <= MAX_SCRIPT_COMMANDS ) {
      if ( command == NULL ) {     
         command = new ScriptCommand[tNC];

         if ( command != NULL ) {
            // allocation successful //
            result = true;
            numCommands = tNC;
         }
      }
   }
   
   return (result);
}

//------------------------------------------------------------------------------
bool Script::setCommand( int cmdNum,  const ScriptCommand &sc )
{
   bool result = false;
   
    if ( command != NULL && cmdNum >= 0 && cmdNum < numCommands ) {
        command[cmdNum].routine = sc.routine;
        command[cmdNum].time    = sc.time;
        command[cmdNum].p1      = sc.p1;
        command[cmdNum].p2      = sc.p2;
        command[cmdNum].p3      = sc.p3;
        command[cmdNum].p4      = sc.p4;
        
        result = true;
    } 
    
    return (result);   
}

//------------------------------------------------------------------------------
bool Script::getCommand( int index, ScriptCommand &t ) const
{
   if ( command != NULL && index >= 0 && index < numCommands ) {
      t = command[index];
      return (true);
   }
   else {
      return (false);
   }
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
   
   switch (op)
   {
      case OP_EQ:
         result = ( v1 == v2 );
         break;
      case OP_NE:
         result = ( v1 != v2 );
         break;
      case OP_LT:
         result = ( v1 < v2 );
         break;
      case OP_GT:
         result = ( v1 > v2 );
         break;
      case OP_LE:
         result = ( v1 <= v2 );
         break;
      case OP_GE:
         result = ( v1 >= v2 );
         break;
   }
   
   return (result);
}

//------------------------------------------------------------------------------
float performArithmeticOperation(float v1, int op, float v2)
{
   float result = 0.0f;
   
   switch (op)
   {
      case OP_EQ:
         result = ( v1 == v2 );
         break;
      case OP_NE:
         result = ( v1 != v2 );
         break;
      case OP_LT:
         result = ( v1 < v2 );
         break;
      case OP_GT:
         result = ( v1 > v2 );
         break;
      case OP_LE:
         result = ( v1 <= v2 );
         break;
      case OP_GE:
         result = ( v1 >= v2 );
         break;
      case OP_ADD:
         result = ( v1 + v2 );
         break;
      case OP_SUB:
         result = ( v1 - v2 );
         break;
      case OP_MUL:
         result = ( v1 * v2 );
         break;
      case OP_DIV:
         if ( v2 != 0 ) 
            result = ( v1 / v2 );
         break;
      case OP_MOD:
         if ( v2 != 0 )
            result = fmod( v1, v2 );
         break;
   }
   
   return (result);
}
