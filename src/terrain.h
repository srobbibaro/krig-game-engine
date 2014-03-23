//////////////////////////////////////////////////////////////////
// Description : Class for handling terrain display and         //
//               generation.                                    //
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
        Terrain();
        ~Terrain();

		void draw( Object* );
		void drawOutline( Object* );

        void handleCollision( Object* temp ) {}
        void animate( float, Object* );
        void generate( void );

	    void calcTerrainNorm( Vector* );
	    void calcViewableTerrainNorm();
        void update( Vector* );

        void init();
        void load( const char*, Vector* );
        void save( char*, Vector* );
        void unload();

        float getHeight( float, float );

        // remove from final version //
        void printTypeName(void)
        {
            PRINT_DEBUG("Terrain");
        }
 		void drawGrid(void);


 		void setVertexHeight(int x, int z, float height);
        void setVertexType(int x, int z, int type);
        void setVertexColor(int x, int z, Vector color);
        float getVertexHeight(int x, int z);
        int getVertexType(int x, int z);
        Vector getVertexColor(int x, int z);

        DisplayList* getDisplayList() { return displayList_; }
        void setDisplayList( DisplayList* displayList ) { displayList_ = displayList; }

        int getXSize() { return xSize_; }
        int getZSize() { return zSize_; }
        int getScaleFactor() { return scaleFactor_; }

        void setCurveDistance( GLfloat curveDistance ) { curveDistance_ = curveDistance; }
        void setCurveRate( GLfloat curveRate ) { curveRate_ = curveRate; }
        void setIsCurveEnabled( bool isCurveEnabled ) { isCurveEnabled_ = isCurveEnabled; }
        GLfloat getCurveDistance() { return curveDistance_; };
        GLfloat getCurveRate() { return curveRate_; };
        GLfloat getIsCurveEnabled() { return isCurveEnabled_; }

    private:
        DisplayList* displayList_;

        GLint xSize_, zSize_;
		GLfloat scaleFactor_;

        GLfloat*** vertex_;
        GLfloat** lightIntensity_;
        GLfloat*** color_;
        Vector **vertexNormal_;
        int** type_;

        GLfloat curveDistance_;
        GLfloat curveRate_;
        bool isCurveEnabled_;

        float totalTime_;

        Vector *light_;
};


#endif
