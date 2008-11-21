#include <windows.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdio.h>
#include "menu.h"
#include "text.h"

//------------------------------------------------------------------------------
Menu::Menu()
{
    cursor = -0.2f;
    alph = 0.1f;
    alphaChange = 0.01f;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Menu::draw(char * menStr, bool updown)
{
    if (updown)
        cursor = 0.0f;
    else
        cursor = -0.2f;

    char str[128];
    glTranslatef (0.0f, 0.0f, -2.0f);

    glPushMatrix();
        glTranslatef (0.0f, 0.0f, -4.0f);
        glBegin(GL_TRIANGLES);
            glColor3f (1.0f, 1.0f, 1.0f);
            glVertex3f (-4.0f, 3.0f, 0.0f);
            glColor3f (0.0f, 0.0f, 0.0f);
            glVertex3f (0.0f, 0.0f, 0.0f);
            glColor3f (1.0f, 1.0f, 1.0f);
            glVertex3f (4.0f, 3.0f, 0.0f);
            
            glVertex3f (-4.0f, 3.0f, 0.0f);
            glVertex3f (-4.0f, -3.0f, 0.0f);            
            glColor3f (0.0f, 0.0f, 0.0f);            
            glVertex3f (0.0f, 0.0f, 0.0f);
            
            glColor3f (1.0f, 1.0f, 1.0f);
            glVertex3f (-4.0f, -3.0f, 0.0f);
            glVertex3f (4.0f, -3.0f, 0.0f);
            glColor3f (0.0f, 0.0f, 0.0f);            
            glVertex3f (0.0f, 0.0f, 0.0f);
            
            glColor3f (1.0f, 1.0f, 1.0f);            
            glVertex3f (4.0f, -3.0f, 0.0f);
            glVertex3f (4.0f, 3.0f, 0.0f);
            glColor3f (0.0f, 0.0f, 0.0f);
            glVertex3f (0.0f, 0.0f, 0.0f);            
        glEnd(); 
    glPopMatrix();
    
    glEnable(GL_BLEND);
    glLineWidth(3.0f);
    glColor3f (0.0f, 0.0f, 1.0f);
    sprintf (str, "KRIG");
    displayText(str, -0.69f, 0.5f, -0.01f, 0.005f, 0.003f);
    
    glLineWidth(15.0f);
    glColor3f (0.0f, 0.0f, 0.0f);
    sprintf (str, "KRIG");
    displayText(str, -0.69f, 0.5f, -0.01f, 0.005f, 0.003f);
    
    glPushMatrix();
        glRotatef (50.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef (-0.9f, -0.2f, 0.0f);
        
        glLineWidth(2.0f);
        glColor3f (1.0f, 0.0f, 0.0f);
        displayText(menStr, 0.0f, 0.0f, 0.0f, 0.001f, 0.001f);
        sprintf (str, "Quit");
        displayText(str, 0.0f, -0.2f, 0.0f, 0.001f, 0.001f);
        glLineWidth(1.0f);

        glPushMatrix(); //Draws Blue translucent rectangle. Cursor for Menu
            glTranslatef (0.0f, cursor, 0.0f);
            for (int i=1; i<=2; i++) //1 for lines, 2 for solid. Makes borders
            {
                if (i == 2)
                {
                    glColor3f (0.0f, 0.0f, 0.0f);
                    glDepthFunc (GL_LEQUAL);
                    glCullFace (GL_FRONT);
                    glPolygonMode (GL_BACK, GL_LINE);
                    glLineWidth(10.0f);
                }
                else
                { 
                    glColor4f (0.0f, 0.0f, 1.0f, alph);
                }
                glBegin(GL_QUADS);
                    glVertex3f (-0.05f, -0.05f, 0.05f);
                    glVertex3f (0.75f, -0.05f, 0.05f);
                    glVertex3f (0.75f, 0.15f, 0.05f);
                    glVertex3f (-0.05f, 0.15f, 0.05f);

                    glVertex3f (-0.05f, 0.15f, 0.05f);
                    glVertex3f (0.75f, 0.15f, 0.05f);
                    glVertex3f (0.75f, 0.15f, -0.05f);
                    glVertex3f (-0.05f, 0.15f, -0.05f);

                    glVertex3f (-0.05f, -0.05f, -0.05f);
                    glVertex3f (-0.05f, -0.05f, 0.05f);
                    glVertex3f (-0.05f, 0.15f, 0.05f);
                    glVertex3f (-0.05f, 0.15f, -0.05f);

                    glVertex3f (0.75f, -0.05f, 0.05f);
                    glVertex3f (0.75f, -0.05f, -0.05f);
                    glVertex3f (0.75f, 0.15f, -0.05f);
                    glVertex3f (0.75f, 0.15f, 0.05f);

                    glVertex3f (-0.05f, -0.05f, -0.05f);
                    glVertex3f (0.75f, -0.05f, -0.05f);
                    glVertex3f (0.75f, -0.05f, 0.05f);
                    glVertex3f (-0.05f, -0.05f, 0.05f);

                    glVertex3f (0.75f, -0.05f, -0.05f);
                    glVertex3f (-0.05f, -0.05f, -0.05f);
                    glVertex3f (-0.05f, 0.15f, -0.05f);
                    glVertex3f (0.75f, 0.15f, -0.05f);
                glEnd();
        
                if (i == 2)
                {
                    glPolygonMode (GL_BACK, GL_FILL);
                    glDepthFunc (GL_LESS);
                    glCullFace (GL_BACK);
                    glLineWidth(1.0f);
                }
                
            }
        glPopMatrix();
    glPopMatrix();
    glDisable(GL_BLEND);
    
    glPushMatrix();     //Displays raster text for the game instructions
        glTranslatef (0.0f, -0.4f, 0.0f);
        glColor3f (1.0f, 0.0f, 0.0f);
        sprintf (str, "Controls:");
        glRasterPos2f(0.0f, 0.2f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        glColor3f (0.0f, 0.0f, 0.0f);
        sprintf (str, "Arrow Keys      : Move Ship/Menu");
        glRasterPos2f(0.1f, 0.15f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        sprintf (str, "Enter           : Choose Option");
        glRasterPos2f(0.1f, 0.1f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        sprintf (str, "Spacebar        : Fire Weapon");
        glRasterPos2f(0.1f, 0.05f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        sprintf (str, "Escape          : Pause Menu");
        glRasterPos2f(0.1f, 0.0f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        sprintf (str, "F1, F2, F3      : Lower Detail Level");
        glRasterPos2f(0.1f, -0.05f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        glColor3f (1.0f, 0.0f, 0.0f);
        sprintf (str, "Game Description:");
        glRasterPos2f(0.0f, -0.1f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        glColor3f (0.0f, 0.0f, 0.0f);
        sprintf (str, "An alien army, the Krig, has come");
        glRasterPos2f(0.1f, -0.15);
        render_string(GLUT_BITMAP_9_BY_15, str);
        sprintf (str, "to destroy the planet! Shoot down");
        glRasterPos2f(0.1f, -0.2);
        render_string(GLUT_BITMAP_9_BY_15, str);
        sprintf (str, "their fighters and avoid asteroids");
        glRasterPos2f(0.1f, -0.25);
        render_string(GLUT_BITMAP_9_BY_15, str);
        sprintf (str, "to stop the full scale invasion");
        glRasterPos2f(0.1f, -0.3);
        render_string(GLUT_BITMAP_9_BY_15, str);
        sprintf (str, "of the Krig!!!");
        glRasterPos2f(0.1f, -0.35);
        render_string(GLUT_BITMAP_9_BY_15, str);
        
        sprintf (str, "v 3.0");
        glRasterPos2f(0.6f, .87);
        render_string(GLUT_BITMAP_HELVETICA_18, str);
        
        //glColor3f(1.0f, 0.0f, 0.0f);
        //sprintf (str, "HighScore: %08ld", highScore);
        //glRasterPos2f(-0.25f, .8 );
        //render_string(GLUT_BITMAP_9_BY_15, str);
        
    glPopMatrix();
    
    if (alph >= 0.5f)
        alphaChange = -0.01f;
    else if (alph <= 0.1f)
        alphaChange = 0.01f;
    alph += alphaChange;

    glLineWidth(3.0f);
}

//------------------------------------------------------------------------------
