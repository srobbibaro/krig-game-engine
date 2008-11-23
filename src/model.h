#include <map>
#include <string>

#include "objects.h"

using namespace std;

#ifndef _MODEL_H_
#define _MODEL_H_

    struct Triangle
    {
	    GLuint vertices[3];         // index into model's vertex array
        GLfloat colors[3][3];       // [vertex][rgb]
    };

    struct Edge
    {
        int vertices[2];
        int triangleIndex[2];
    };
    
    struct TransformedEdge
    {
        Vector p1, p2;
    };
    
    struct ModelStorage
    {
        GLuint numVertices;          // number of vertices in the model
        GLuint numTriangles;         // number of triangles in the model
        GLfloat initalScale;		 // the scaleFactor that was used to scale the vertices
        GLfloat** baseVertex;        // list of base vertices ( not transformed )
        Vector* normal;              // list of normals that comprise the model
        Triangle* triangle;     
    
        void load( char[] );
    };

    extern map <string, ModelStorage*> modelHash;
    
    class Model : public Object
	{
		protected:
            string modelKey;
            
            // transformed data //
            GLfloat* lightIntensity;     // light intensity due to each normal, updated per cycle
            GLfloat** updatedVertex;     // list of vertices that comprise the model
            
            // this is in here for shadows...???
            int numEdges;
            Edge* edges;
    
            TransformedEdge* sEdge;
            int numSEdge;
        
        public:
            Model();
            Model( string );
            virtual ~Model();
            void load( string );
            void unload( void );

            void draw( Camera* );
            void drawOutline( Camera* );
            void drawShadow ( Vector* );
            void handleCollision( Object* );
            void update( Vector* );
            void prepare( void ); 
            void animate( float, Camera* );
            void handleDeath( void ) {}
            
            void buildEdges();
            
            virtual void processAI( Camera* c ) = 0; 
	};

#endif