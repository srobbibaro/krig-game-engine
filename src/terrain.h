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
#include "Object.h"
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
        
        GLfloat*** vertex;
        GLfloat** lightIntensity;
        GLfloat*** color;
        GLint** type;
        
        Vector* lastLight;
        
        float totalTime;
        
        Terrain();
        ~Terrain();
        
		void draw( Object* );
		void drawOutline( Object* );

        void handleCollision( Object* temp ) {}
        void prepare() {}
        void animate( float, Object* );
        void generate( void );
        
	    void calcTerrainNorm( Vector* );
        void update( Vector* );
        void handleDeath( void ) {}
        
        void init();
        void load( const char*, Vector* );
        void save( char*, Vector* );
        void unload();
        
        float getHeight( float, float );
        
        void printTypeName(void) 
         {
            cout << "Terrain";
         }
         
         // remove from final version //
 		void drawGrid(void);
     };


#endif
