#ifndef _SOMETHING_
#define _SOMETHING_

struct ScriptCommand
{
    int routine;                    // operation code of command
    float time;                     // time to execute command (script clock)
    float p1, p2, p3, p4;           // 4 parameters in command
};

class Script
{
    private:
        ScriptCommand *command;     // list of commands in script
        int numCommands;            // number of commands in script

     public:
        Script( void );
        Script( int tNC  );
        ~Script( void );
        
        bool initialize( int tNC  );
        bool setCommand( int, const ScriptCommand& ); 
        bool getCommand( int, ScriptCommand & ) const;
        int  getNumCommands( void ) const;
};
       

extern int numScripts;              // number of scripts
extern Script *script;              // storage of those scripts

extern bool evaluateBranchCondition(float v1, int op, float v2);
extern float performArithmeticOperation(float v1, int op, float v2);
extern void setInterpolationVariable(int index);

#endif
