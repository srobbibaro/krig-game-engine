//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : terrain.h                                      //
//                                                              //
// Description : Class for handling terrain display and         //
//               generation.                                    //
//                                                              //
// Date        : 05-8-04                                        //
//////////////////////////////////////////////////////////////////

using namespace std;

#ifndef _TERRAIN_H_
#define _TERRAIN_H_


#include "constants.h"
#include "Objects.h"
#include "DisplayList.h"
  
    class Terrain : public Object
	{
	   public:
	       DisplayList* l;
        GLint xSize, zSize;
		GLfloat scaleFactor;
		
        //GLfloat terrain[X_SIZE*Z_SIZE][3];
        //int terrainIndex[17];       // 14 x positions to draw
		//GLfloat lightIntensity[X_SIZE*Z_SIZE];
        //GLfloat color[X_SIZE*Z_SIZE][3];
        
        GLfloat vertex[X_SIZE][Z_SIZE][3];
        GLfloat lightIntensity[X_SIZE][Z_SIZE];
        GLfloat color[X_SIZE][Z_SIZE][3];
        GLint type[X_SIZE][Z_SIZE];
        
        
       
        float totalTime;
        
        Terrain();
        ~Terrain();
		void draw( Camera* );
		void drawOutline( Camera* );

        void handleCollision( Object* temp ) {}
        void prepare() {}
        void animate( float, Camera* );
        void generateTerrain( void );
        
	    void calcTerrainNorm( Vector* );
        void loadTerrain( const char*, Vector* );
        void saveTerrain( char*, Vector* );
        void update( Vector* );
        void handleDeath( void ) {}
        
        float getHeight( float, float );
        
        void printTypeName(void) 
         {
            cout << "Terrain";
         }
         
         // remove from final version //
 		void drawGrid(void);
     };


#endif
