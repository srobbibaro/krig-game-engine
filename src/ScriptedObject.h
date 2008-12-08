#ifndef _SCRIPTED_OBJECT_H_
#define _SCRIPTED_OBJECT_H_

#include "constants.h"
#include "model.h"

class ScriptedObject : public Model
{
      public:
        void prepare() {}
        
        ScriptedObject();
        ScriptedObject( string );
        virtual ~ScriptedObject();
        
        void processAI( Object*, float elapsedTime ); //# Camera*
        
        void printTypeName(void) 
         {
            cout << "ScriptedObject";
         }
};

       
#endif

