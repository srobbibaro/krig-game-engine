#ifndef _SCRIPTED_OBJECT_H_
#define _SCRIPTED_OBJECT_H_

#include "constants.h"
#include "model.h"

class ScriptedObject : public Model {
  public:
    ScriptedObject();
    virtual ~ScriptedObject();

    void printTypeName(void) { PRINT_DEBUG("ScriptedObject"); }
};

#endif

