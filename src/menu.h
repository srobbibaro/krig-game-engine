#ifndef _MENU_H_
#define _MENU_H_

class Menu
{
    private:
        float cursor;  //0.0 for Play Game -0.2 for Quit
        float alph;
        float alphaChange;
        
    public:
        Menu();
        void draw(char *, bool);
};
#endif
