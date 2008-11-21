#include "model.h"

#ifndef _BEAM_H_
#define _BEAM_H_

class Beam : public Model
{
    private:
        
    public:
        void handleCollision( Object* );
        void prepare() {}
        
        Beam();
        Beam( string, string, float* );
        void processAI( Camera* );
       
        
        void printTypeName(void) 
         {
            cout << "Beam";
         }
};


#endif
