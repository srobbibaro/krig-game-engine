//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : menuCredits.cpp                                //
//                                                              //
// Description : Functions for displaying the ending Credits,   //
//               the menu/pause screen, and game over screen    //
//                                                              //
// Date        : 05-8-04                                        //
//////////////////////////////////////////////////////////////////
/*#include <string>
using namespace std;

#include <gl/glut.h>
#include <stdio.h>
#include "text.h"

extern float creditsPos;
extern long highScore;

void displayCredits(float delay)
{
    float CredTrans;
    
    if (creditsPos >= 7.0f)
        CredTrans = 0.0f;
    else
        CredTrans = 0.1f;
    
    creditsPos += (CredTrans * delay);
    
    string creds[40] = { "Head Programmer", "Steven Robbibaro",
                       "Assistant Programmer", "Andrew Yochum",
                       "Terrain Generation/Design", "Steven Robbibaro",
                       "Model Design", "Andrew Yochum",
                       "Object Behavior", "Steven Robbibaro",
                       "Sound", "Andrew Yochum",
                       "Camera Behavior", "Steven Robbibaro",
                       "Menu Design", "Andrew Yochum",
                       "Data Storage", "Steven Robbibaro", "Andrew Yochum",
                       "Game Logic", "Steven Robbibaro", "Andrew Yochum",
                       "Controls", "Steven Robbibaro", "Andrew Yochum",
                       "Sound Editing", "Kelly McMeans",
                       "Music by", "The NESkimos", "http://www.neskimos.com", "Wily's Theme - Stage 1", "MegaMan2 - Intro", "MegaMan2 - Ending",
                       "Special Thanks", "NeHe Productions:", "http://nehe.gamedev.net", "http://www.openal.org", "Capcom",
                       "THE END",
                       "Press Escape to Continue" };

    float credCoord[40][2] = { {-0.2f   , 0.0f  }, { -0.22f, -0.1f },
                               {-0.25f  , -0.4f }, { -0.2f , -0.5f }, 
                               {-0.3f   , -0.8f }, { -0.22f, -0.9f },
                               {-0.17f  , -1.2f }, { -0.2f , -1.3f },
                               {-0.2f   , -1.6f }, { -0.22f, -1.7f },
                               {-0.08f  , -2.0f }, { -0.2f , -2.1f },
                               {-0.2f   , -2.4f }, { -0.22f, -2.5f },
                               {-0.15f  , -2.8f }, { -0.2f , -2.9f },
                               {-0.165f , -3.2f }, { -0.22f, -3.3f }, { -0.2f , -3.4f },
                               {-0.142f , -3.6f }, { -0.22f, -3.7f }, { -0.2f , -3.8f },
                               {-0.12f  , -4.0f }, { -0.22f, -4.1f }, { -0.2f , -4.2f },
                               {-0.18f  , -4.4f }, { -0.18f, -4.5f },
                               {-0.12f  , -4.8f }, { -0.17f, -4.9f }, { -0.29f , -5.0f }, { -0.26f , -5.1f }, { -0.26f , -5.15f }, { -0.26f , -5.2f }, 
                               {-0.2f   , -5.4f }, { -0.23f, -5.5f }, { -0.29f , -5.55f}, { -0.27f , -5.7f }, { -0.12f , -5.85f },
                               {-0.16f  , -7.0f },
                               {-0.37f  , -7.8f } };
                               
    char *str;
    glPushMatrix();
        glTranslatef (0.0f, creditsPos, -2.0f);
        
        for (int i = 0 ; i < 40 ; i++)
        {
            str = &creds[i][0];
            
            if (i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 ||
                i == 16 || i == 19 || i == 22 || i == 25 || i == 27 || i == 33)
            {
                glColor3f (1.0f, 1.0f, 1.0f);
                glRasterPos2f (credCoord[i][0], credCoord[i][1]);
                render_string(GLUT_BITMAP_9_BY_15, str);
            }
            else if (i == 38 || i == 39)
            {
                glColor3f (1.0f, 1.0f, 1.0f);
                glRasterPos2f (credCoord[i][0], credCoord[i][1]);
                render_string(GLUT_BITMAP_TIMES_ROMAN_24, str);
            }
            else
            {
                glColor3f (1.0f, 0.0f, 0.0f);
                glRasterPos2f (credCoord[i][0], credCoord[i][1]);
                render_string(GLUT_BITMAP_HELVETICA_18, str);
            }
        }
    glPopMatrix();
            
}

//------------------------------------------------------------------------------
void displayGameOver()
{
    char str[128];
    glPushMatrix();
        glTranslatef (0.0f, 0.0f, -2.0f);
        
        glColor3f (0.0f, 0.0f, 1.0f);
        sprintf (str, "GAME OVER");
        displayText(str, -0.35f, 0.0f, 0.0f, 0.001f, 0.001f);
        
        sprintf (str, "Press Escape to Continue");
        displayText(str, -0.64, -0.7f, 0.0f, 0.0008f, 0.0008f);

        glColor3f (0.0f, 0.0f, 0.0f);
        glLineWidth (7.0f);
        sprintf (str, "GAME OVER");
        displayText(str, -0.35f, 0.0f, 0.0f, 0.001f, 0.001f);
        
        sprintf (str, "Press Escape to Continue");
        displayText(str, -0.64, -0.7f, 0.0f, 0.0008f, 0.0008f);
        glLineWidth (1.0f);
    glPopMatrix();
}*/
