#include "Constants.h"
#include "Model.h"
using namespace std;

map <string, ModelStorage*> modelHash;

Model::Model()
: Object()
{
    updatedVertex = NULL;
    lightIntensity = NULL;
}

//------------------------------------------------------------------------------
Model::~Model()
{
    unload();
}

//------------------------------------------------------------------------------
void Model::load( string tModelKey )
{
    // model must have been unloaded first
    if (lightIntensity != NULL || updatedVertex != NULL)
        return;

    GLfloat temp;
    modelKey = tModelKey;
    
    int numVertices = modelHash[modelKey]->numVertices;
    
    updatedVertex = new GLfloat*[ numVertices ];
    lightIntensity = new GLfloat[ numVertices ];

    int	r = 0;
     
    updatedVertex[r] = new GLfloat[3];
            
    while ( updatedVertex[ r ] != NULL && r < numVertices-1 ) {
        updatedVertex[++r] = new GLfloat[3];
    }
            
    // load in vertices used for model //////
    for ( int i = 0; i < numVertices; i++ ) {    
        for ( int j = 0; j < 3; j++ ) {    
            updatedVertex[i][j] = modelHash[modelKey]->baseVertex[i][j];
        }
    }
    /////////////////////////////////////////

    //buildEdges();
}

//------------------------------------------------------------------------------
void Model::unload()
{
    // delete old transforemed data /////////////
    if ( updatedVertex != NULL ) {
        for( int r = 0; r < modelHash[modelKey]->numVertices; r++) {
           delete[] updatedVertex[r];
        }
    
        delete[] updatedVertex;
    }
    
    if ( lightIntensity != NULL )
        delete[] lightIntensity;

        
    lightIntensity = NULL;
    updatedVertex = NULL;
}

//------------------------------------------------------------------------------
void Model::draw(Object* c)
{
    // model must be loaded
    if (lightIntensity == NULL || updatedVertex == NULL)
        return;
    
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    ModelStorage *m = modelHash[modelKey];
    glBegin( GL_TRIANGLES );
        for ( int i = 0; i < m->numTriangles; i++ ) {
            for ( int j = 0; j < 3; j++ ) {
                glColor3fv( m->triangle[i].colors[j] );
                glTexCoord1f( lightIntensity[m->triangle[i].vertices[j]] ); 
                glVertex3fv( updatedVertex[m->triangle[i].vertices[j]] ); 
            }
        }   
    glEnd();
    
    glPopMatrix();
    
    if (particleSystem != NULL)
        particleSystem->draw();
}

//------------------------------------------------------------------------------
void Model::drawOutline(Object* c)
{
    // model must first be loaded
    if (lightIntensity == NULL || updatedVertex == NULL)
        return;

    ModelStorage *m = modelHash[modelKey];
    
    glPushMatrix();  
    glTranslatef(position.x, position.y, position.z);

    glColor3f( 0.0f, 0.0f, 0.0f );
       
    glBegin( GL_TRIANGLES );
        for ( int i = 0; i < m->numTriangles; i++ ) {	
            for ( int j = 0; j < 3; j++ ) {
                glVertex3fv( updatedVertex[m->triangle[i].vertices[j]] );       
            }
        }
    glEnd();
    glPopMatrix();
}

//------------------------------------------------------------------------------
void Model::drawShadow (Vector* light)
{
        int p1, p2;
        Vector v1, v2;
        
        for ( int i = 0; i < numSEdge; i++ ) {            
					//calculate the length of the vector
					v1.x = (sEdge[i].p1.x - light->x)*100;
					v1.y = (sEdge[i].p1.y - light->y)*100;
					v1.z = (sEdge[i].p1.z - light->z)*100;

					v2.x = (sEdge[i].p2.x - light->x)*100;
					v2.y = (sEdge[i].p2.y - light->y)*100;
					v2.z = (sEdge[i].p2.z - light->z)*100;    
                    
                    //draw the polygon
                    /*
                    glBegin( GL_LINES );
                        glVertex3f( sEdge[i].p1.x, sEdge[i].p1.y, sEdge[i].p1.z );
                        glVertex3f( sEdge[i].p2.x, sEdge[i].p2.y, sEdge[i].p2.z );
                    glEnd();
                    */
	
                    glBegin(GL_TRIANGLE_STRIP);
						glVertex3f( sEdge[i].p1.x, sEdge[i].p1.y, sEdge[i].p1.z );
						glVertex3f( sEdge[i].p1.x + v1.x,
									sEdge[i].p1.y + v1.y,
									sEdge[i].p1.z + v1.z);

						glVertex3f(sEdge[i].p2.x,
									sEdge[i].p2.y,
									sEdge[i].p2.z );
						glVertex3f(sEdge[i].p2.x + v2.x,
									sEdge[i].p2.y + v2.y,
									sEdge[i].p2.z + v2.z);
					glEnd();
            }     
}

//------------------------------------------------------------------------------
void Model::handleCollision( Object* temp )
{
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (L == NULL)
        return;
        
    // Find the update function and call it
    lua_getglobal(L, "on_collision");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)this);
    lua_pushlightuserdata(L, (void*)temp);
	     
    // Call the function with 2 argument and no return values
    lua_call(L, 2, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
}

//------------------------------------------------------------------------------
void Model::update( Vector* light )
{
    // model must first be loaded
    if (lightIntensity == NULL || updatedVertex == NULL)
        return;
        
    if (scaleChanged == false && rotationChanged == false && 
        lastLight.x == light->x &&
        lastLight.y == light->y && 
        lastLight.z == light->z) {
        boundingSphere.setOriginVector(position);
        return;
    }
    
    lastLight.x = light->x;
    lastLight.y = light->y;
    lastLight.z = light->z;
    scaleChanged = false;
    rotationChanged = false;

    Vector tempV;
    GLfloat temp;
    Matrix rotationMatrix;
    Matrix scaleMatrix;    
    Matrix transform;
    
    GLfloat min[] = { 9999.0f, 9999.0f, 9999.0f };
    GLfloat max[] = { -9999.0f, -9999.0f, -9999.0f };
  
    // setup transformation matrices ////////////
    rotation.buildRotationMatrix( rotationMatrix );   
    scaleMatrix.setScale( scale.x, scale.y, scale.z );
    
    // setup the transformation matrix //////////
    transform = rotationMatrix * scaleMatrix;   
    /////////////////////////////////////////////
    
    ModelStorage *m = modelHash[modelKey];
    
    float radius = 0.0f;
    
    for ( int i = 0; i < m->numVertices; i++ ) {   
        // transform vertex /////////////////////   
        transform.transformVertex( m->baseVertex[i], updatedVertex[i] );   
        
        // calculate light intensity ////////////
        tempV.rotateVector( transform, m->normal[i] );
        tempV.normalize();
            
        temp = tempV.dotProduct( *light );
                   
        if ( temp == 1.0f )
            temp = 0.5f;
        else if ( temp < 0.0f )
            temp = 0.0f;       

        lightIntensity[i] = temp;
        /////////////////////////////////////////
        
        // re-calculate collision box ///////////
        for ( int j = 0; j < 3; j++ ) {
            if ( updatedVertex[i][j] > max[j] )
                max[j] = updatedVertex[i][j];
                
            if ( updatedVertex[i][j] < min[j] )
                  min[j] = updatedVertex[i][j];
        }
        /////////////////////////////////////////
       
        float x = (updatedVertex[i][0]);
        float y = (updatedVertex[i][1]);
        float z = (updatedVertex[i][2]);
        
        float distance = (x * x) + (y * y) +(z * z);
                         
        if (distance > radius)
            radius = distance;
    }
    
    collisionBox[0].setVector( min[0], min[1], min[2] );
    collisionBox[1].setVector( max[0], max[1], max[2] );
    
    radius = sqrt(radius);    
    boundingSphere.setSphere(position.x, position.y, position.z, radius );
    
    controlPoints[0].setVector( max[0], min[1], min[2] );
    controlPoints[1].setVector( max[0], min[1], max[2] );
    controlPoints[2].setVector( min[0], min[1], (min[2]+max[2])/2 );
    
    direction.rotateVector( rotationMatrix, baseDirection );
    direction.normalize();
    
    Vector upV;
    upV.setVector(0.0f, 1.0f, 0.0f);
    
    up.rotateVector( rotationMatrix, upV );
    up.normalize();
    
 /*
        Vector p1, p2, p3;
        Vector tempN;
        Vector lp;
        float tempD[2];
        
        numSEdge = 0;
        
        for ( int i = 0; i < numEdges; i++ ) {   
            for ( int u = 0; u < 2; u++ ) {
                
               p1.setVector(   updatedVertex[triangle[edges[i].triangleIndex[u]].vertices[0]][0],
                                updatedVertex[triangle[edges[i].triangleIndex[u]].vertices[0]][1],
                                updatedVertex[triangle[edges[i].triangleIndex[u]].vertices[0]][2]
                            );
                
                p2.setVector(   updatedVertex[triangle[edges[i].triangleIndex[u]].vertices[1]][0],
                                updatedVertex[triangle[edges[i].triangleIndex[u]].vertices[1]][1],
                                updatedVertex[triangle[edges[i].triangleIndex[u]].vertices[1]][2]
                            );
                
                p3.setVector(   updatedVertex[triangle[edges[i].triangleIndex[u]].vertices[2]][0],
                                updatedVertex[triangle[edges[i].triangleIndex[u]].vertices[2]][1],
                                updatedVertex[triangle[edges[i].triangleIndex[u]].vertices[2]][2]
                            );
                
                tempV.rotateVector( transform, tempN );
                tempV.normalize();
            
                tempD[u] = tempV.dotProduct( *light ); 
            }            
    
            if ( tempD[0] >= 0 && tempD[1] <= 0 || tempD[0] <= 0 && tempD[1] >= 0 ) {
                 p1.setVector( updatedVertex[edges[i].vertices[0]][0],
                              updatedVertex[edges[i].vertices[0]][1],
                              updatedVertex[edges[i].vertices[0]][2]
                );
                p2.setVector( updatedVertex[edges[i].vertices[1]][0],
                              updatedVertex[edges[i].vertices[1]][1],
                              updatedVertex[edges[i].vertices[1]][2]
                );

                tempV.transformVector( transform, p1 );     
                sEdge[numSEdge].p1 = tempV;
                
                tempV.transformVector( transform, p2 );
                sEdge[numSEdge].p2 = tempV;
                
                numSEdge++;
            }          
        } 

*/
}

//------------------------------------------------------------------------------
void Model::animate( float timeElapsed, Object* c ) 
{   
    int r =  dynamic_cast<Camera*>(c)->frustum.testSphere(boundingSphere);
    isInView = (r != -1);
    
    animateScript(timeElapsed);

    // calculate new position and orientation //
    if ( speed == 0 ) {
        position.x += velocity.x * timeElapsed;   
        position.y += velocity.y * timeElapsed;   
        position.z += velocity.z * timeElapsed;   
    }
    else {
        switch (speedDir)
        {
            case 0:
                direction.scale(speed*timeElapsed);
                position.x += direction.x;
                position.y += direction.y;
                position.z += direction.z;
                direction.normalize();
                break;
            case 1:
            {
                Vector rotationAxis;
            
                rotationAxis.crossProduct(up, direction);
                rotationAxis.normalize();
                        
                rotationAxis.scale(speed*timeElapsed);
                position.x += rotationAxis.x;
                position.y += rotationAxis.y;
                position.z += rotationAxis.z;
                break;
            }
            case 2:
            {
                up.scale(speed*timeElapsed);
                position.x += up.x;
                position.y += up.y;
                position.z += up.z;
                direction.normalize();
                break;
            }
        }
    }
    
    if (scaleRate.x != 0.0f ||
        scaleRate.y != 0.0f ||
        scaleRate.z != 0.0f) {        
        scale.x += scaleRate.x * timeElapsed;
        scale.y += scaleRate.y * timeElapsed;
        scale.z += scaleRate.z * timeElapsed;
        scaleChanged = true;
    }
            
    if (!interp) {
        if ( rotationVelocity.x != 0.0f ||
             rotationVelocity.y != 0.0f ||
             rotationVelocity.z != 0.0f ) {
                rotationChanged = true;
             
                Vector tempV;
                Quaternion tempQ;
	
                tempV.x = rotationVelocity.x * timeElapsed;
                tempV.y = rotationVelocity.y * timeElapsed;
                tempV.z = rotationVelocity.z * timeElapsed;
	             
                tempQ.buildFromEuler(tempV);	
                rotation = rotation * tempQ;
        }
    }
    else {
        rotationChanged = true;
        
        float endVal = valInterpEnd - valInterpStart;
        float curVal = *valInterpPtr - valInterpStart;
              
        float t = 0.0f;
                                              
        if ( endVal > 0 ) {
            if ( curVal > endVal )
                t = 1.0f;
            else if ( curVal < 0.0f )
                t = 0.0f;
            else
                t = curVal / endVal; 
        }
        else if ( endVal < 0 ) {
            if ( curVal < endVal )
                t = 1.0f;
            else if ( curVal > 0.0f )
                t = 0.0f;
            else
                t = curVal / endVal; 
        }
                                
        rotation.slerp(rInterpStart, t, rInterpEnd );
    }
    /////////////////////////////////////////////
    
    // The particle systems might not always live here...
    if (particleSystem != NULL)
        particleSystem->update(timeElapsed);
}

//------------------------------------------------------------------------------
void ModelStorage::load( char fileName[] )
{
    GLfloat temp;
   
    ifstream fin( fileName );    
        fin >> initalScale;
        fin >> numVertices;
            baseVertex = new GLfloat* [ numVertices ];
            normal = new Vector[ numVertices ];

            int	r = 0;
            baseVertex[r] = new GLfloat[3];
            
            while ( baseVertex[ r ] != NULL && r < numVertices-1 ) {
	           baseVertex[++r] = new GLfloat[3];
            }
            
            // load in vertices used for model //////
            for ( int i = 0; i < numVertices; i++ ) {    
                for ( int j = 0; j < 3; j++ ) {    
                    fin >> temp;
                    baseVertex[i][j] = temp;   
                }

                fin >> normal[i].x;
                fin >> normal[i].y;
                fin >> normal[i].z;
            }      
            /////////////////////////////////////////
   
            fin >> numTriangles;
            triangle = new Triangle[ numTriangles ];
        
            // load in triangles used for model /////
            for ( int i = 0; i < numTriangles; i++ ) {
                for ( int j = 0; j < 3; j++ )
                    fin >> triangle[i].vertices[j];
            
                for ( int j = 0; j < 3; j++ )
                    for ( int k = 0; k < 3; k++ )
                        fin >> triangle[i].colors[j][k];
            }
            /////////////////////////////////////////
    fin.close();
}

//------------------------------------------------------------------------------
void Model::buildEdges()
    { 
        /*  
        // Allocate enough space to hold all edges
        edges = new Edge[numTriangles * 3];
        sEdge = new TransformedEdge[numTriangles * 3 ];
                
        int edgeCount = 0,
            i1, i2, i3;

        // First pass: find edges
        for ( int a = 0; a < numTriangles; a++) {
            i1 = triangle[a].vertices[0];
            i2 = triangle[a].vertices[1];
            i3 = triangle[a].vertices[2];

            if (i1 < i2) {
                edges[edgeCount].vertices[0] = i1;
                edges[edgeCount].vertices[1] = i2;
                edges[edgeCount].triangleIndex[0] = a;
                edges[edgeCount].triangleIndex[1] = -1;
                edgeCount++;
            }

            if (i2 < i3) {
                edges[edgeCount].vertices[0] = i2;
                edges[edgeCount].vertices[1] = i3;
                edges[edgeCount].triangleIndex[0] = a;
                edges[edgeCount].triangleIndex[1] = -1;
                edgeCount++;
            }

            if (i3 < i1) {
                edges[edgeCount].vertices[0] = i3;
                edges[edgeCount].vertices[1] = i1;
                edges[edgeCount].triangleIndex[0] = a;
                edges[edgeCount].triangleIndex[1] = -1;
                edgeCount++;
            }
        }

        
        // Second pass: match triangles to edges
        for ( int a = 0; a < numTriangles; a++) {
            i1 = triangle[a].vertices[0];
            i2 = triangle[a].vertices[1];
            i3 = triangle[a].vertices[2];

            if (i1 > i2) {
                for (int b = 0; b < edgeCount; b++) {
                    if ((edges[b].vertices[0] == i2) &&
                        (edges[b].vertices[1] == i1) &&
                        (edges[b].triangleIndex[1] == -1)) {
                            edges[b].triangleIndex[1] = a;
                            break;
                    }
                }
            }

            if (i2 > i3) {
                for (int b = 0; b < edgeCount; b++) {
                    if ((edges[b].vertices[0] == i3) &&
                        (edges[b].vertices[1] == i2) &&
                        (edges[b].triangleIndex[1] == -1)) {
                            edges[b].triangleIndex[1] = a;
                            break;
                    }
                }
            }

            if (i3 > i1) {
                for (int b = 0; b < edgeCount; b++) {
                    if ((edges[b].vertices[0] == i1) &&
                        (edges[b].vertices[1] == i3) &&
                        (edges[b].triangleIndex[1] == -1)) {
                            edges[b].triangleIndex[1] = a;
                            break;
                    }
                }
            }

        }

        numEdges = edgeCount;
        */
    } 
