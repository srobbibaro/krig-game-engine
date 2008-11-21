#ifndef _CREDITS_H_
#define _CREDITS_H_

#include <string>
using namespace std;

class Credits
{
    private:
        float creditsPos;
        long highScore;
        static string creds[40];
        static float credCoord[40][2];
        
    public:
        void draw(long, float);
};

#endif
