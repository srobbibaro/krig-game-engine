#include "constants.h"
#include "model.h"
#include "terrain.h"
using namespace std;

map <string, ModelStorage*> modelHash;

Model::Model()
: Object()
{
    updatedVertex = NULL;
    lightIntensity = NULL;
    modelKey_ = "";
}

//------------------------------------------------------------------------------
Model::~Model()
{
    unload();
}

//------------------------------------------------------------------------------
void Model::load( string modelKey )
{
    // don't load the model if it's already loaded
    if (modelKey_ == modelKey) {
        int numVertices = modelHash[modelKey_]->numVertices;

        // load in vertices used for model //////
        for ( int i = 0; i < numVertices; i++ ) {
            for ( int j = 0; j < 3; j++ ) {
                updatedVertex[i][j] = modelHash[modelKey_]->baseVertex[i][j];
            }
        }
        return;
    }

    // model must have been unloaded first
    if (lightIntensity || updatedVertex)
        unload();

    modelKey_ = modelKey;

    int numVertices = modelHash[modelKey]->numVertices;

    updatedVertex = new GLfloat*[ numVertices ];
    lightIntensity = new GLfloat[ numVertices ];

    int	r = 0;

    updatedVertex[r] = new GLfloat[3];

    while ( updatedVertex[r] && r < numVertices-1 ) {
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
    if ( updatedVertex ) {
        for( int r = 0; r < modelHash[modelKey_]->numVertices; r++) {
           delete[] updatedVertex[r];
        }

        delete[] updatedVertex;
    }

    if ( lightIntensity )
        delete[] lightIntensity;


    lightIntensity = NULL;
    updatedVertex = NULL;

    modelKey_ = "";
}

//------------------------------------------------------------------------------
void Model::draw(Object* c)
{
    // model must be loaded
    if (!lightIntensity || !updatedVertex)
        return;

    glPushMatrix();
    glTranslatef(position_.x, position_.y, position_.z);

    ModelStorage *m = modelHash[modelKey_];
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

    if (particleSystem_ != NULL)
        particleSystem_->draw();
}

//------------------------------------------------------------------------------
void Model::drawOutline(Object* c)
{
    // model must first be loaded
    if (!lightIntensity || !updatedVertex)
        return;

    ModelStorage *m = modelHash[modelKey_];

    glPushMatrix();
    glTranslatef(position_.x, position_.y, position_.z);

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
    if (!L_)
        return;

    // Find the update function and call it
    lua_getglobal(L_, "on_collision");

    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L_, (void*)this);
    lua_pushlightuserdata(L_, (void*)temp);

    // Call the function with 2 argument and no return values
    lua_call(L_, 2, 0);
}

//------------------------------------------------------------------------------
void Model::update( Vector* light )
{
    // model must first be loaded
    if (!lightIntensity || !updatedVertex)
        return;

    if (!scaleChanged_ && !rotationChanged_ &&
        lastLight_.x == light->x &&
        lastLight_.y == light->y &&
        lastLight_.z == light->z) {
        boundingSphere_.setOriginVector(position_);
        return;
    }

    lastLight_.x = light->x;
    lastLight_.y = light->y;
    lastLight_.z = light->z;
    scaleChanged_ = false;
    rotationChanged_ = false;

    Vector tempV;
    GLfloat temp;
    Matrix rotationMatrix;
    Matrix scaleMatrix;
    Matrix transform;

    GLfloat min[] = { 9999.0f, 9999.0f, 9999.0f };
    GLfloat max[] = { -9999.0f, -9999.0f, -9999.0f };

    // setup transformation matrices ////////////
    rotation_.buildRotationMatrix( rotationMatrix );
    scaleMatrix.setScale( scale_.x, scale_.y, scale_.z );

    // setup the transformation matrix //////////
    transform = rotationMatrix * scaleMatrix;
    /////////////////////////////////////////////

    ModelStorage *m = modelHash[modelKey_];

    float radius = 0.0f;

    for ( int i = 0; i < m->numVertices; i++ ) {
        // transform vertex /////////////////////
        transform.transformVertex( m->baseVertex[i], updatedVertex[i] );

        // calculate light intensity ////////////
        if (isAlwaysLit_) {
            temp = 0.5f;
        }
        else {
            tempV.rotateVector( transform, m->normal[i] );
            tempV.normalize();

            temp = tempV.dotProduct( *light );

            if ( temp == 1.0f )
                temp = 0.5f;
            else if ( temp < 0.0f )
                temp = 0.0f;
        }

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

    collisionBox_[0].setVector( min[0], min[1], min[2] );
    collisionBox_[1].setVector( max[0], max[1], max[2] );

    radius = sqrt(radius);
    boundingSphere_.setSphere(position_.x, position_.y, position_.z, radius );

    controlPoints_[0].setVector( max[0], min[1], min[2] );
    controlPoints_[1].setVector( max[0], min[1], max[2] );
    controlPoints_[2].setVector( min[0], min[1], (min[2]+max[2])/2 );

    direction_.rotateVector( rotationMatrix, baseDirection_ );
    direction_.normalize();

    Vector upV;
    upV.setVector(0.0f, 1.0f, 0.0f);

    up_.rotateVector( rotationMatrix, upV );
    up_.normalize();

    orth_.crossProduct(up_, direction_);
    orth_.normalize();

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
    // determine whether or not the current object is in the camera's view
    int r =  dynamic_cast<Camera*>(c)->frustum.testSphere(boundingSphere_);
    isInView_ = (r != -1);

    // exectue the current object's update function
    animateScript(timeElapsed);

    // calculate new position using speed
    if (speed_.x != 0.0f) {
        direction_.scale(speed_.x * timeElapsed);
        position_.x += direction_.x;
        position_.y += direction_.y;
        position_.z += direction_.z;
        direction_.normalize();
    }

    if (speed_.y != 0.0f) {
        up_.scale(speed_.y * timeElapsed);
        position_.x += up_.x;
        position_.y += up_.y;
        position_.z += up_.z;
        direction_.normalize();
    }

    if (speed_.z != 0.0f) {
        Vector rotationAxis;

        rotationAxis.crossProduct(up_, direction_);
        rotationAxis.normalize();

        rotationAxis.scale(speed_.z * timeElapsed);
        position_.x += rotationAxis.x;
        position_.y += rotationAxis.y;
        position_.z += rotationAxis.z;
    }

    // update position using velocity
    if (velocity_.x != 0.0f)
        position_.x += velocity_.x * timeElapsed;

    if (velocity_.y != 0.0f)
        position_.y += velocity_.y * timeElapsed;

    if (velocity_.z != 0.0f)
        position_.z += velocity_.z * timeElapsed;

    // update scale
    if (scaleRate_.x != 0.0f) {
        scale_.x += scaleRate_.x * timeElapsed;
        scaleChanged_ = true;
    }

    if (scaleRate_.y != 0.0f) {
        scale_.y += scaleRate_.y * timeElapsed;
        scaleChanged_ = true;
    }

    if (scaleRate_.z != 0.0f) {
        scale_.z += scaleRate_.z * timeElapsed;
        scaleChanged_ = true;
    }

    if (!isInterpolationEnabled_) {
        if ( rotationVelocity_.x != 0.0f ||
             rotationVelocity_.y != 0.0f ||
             rotationVelocity_.z != 0.0f ) {
                rotationChanged_ = true;

                Vector tempV;
                Quaternion tempQ;

                tempV.x = rotationVelocity_.x * timeElapsed;
                tempV.y = rotationVelocity_.y * timeElapsed;
                tempV.z = rotationVelocity_.z * timeElapsed;

                tempQ.buildFromEuler(tempV);
                rotation_ = rotation_ * tempQ;
        }
    }
    else {
        rotationChanged_ = true;

        float endVal = valInterpEnd_ - valInterpBegin_;
        float curVal = valInterpCurrent_ - valInterpBegin_;

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

        rotation_.slerp(rInterpStart_, t, rInterpEnd_ );
    }
    /////////////////////////////////////////////

    // The particle systems might not always live here...
    if (particleSystem_ != NULL)
        particleSystem_->update(timeElapsed);
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

    void Model::orientOnTerrain(Terrain *temp, Quaternion baseRotation)
    {
        if (temp == NULL)
            return;

                Vector p1, p2, p3;

                p1.setVector(controlPoints_[0].x + position_.x, 0.0f, controlPoints_[0].z + position_.z);
                p2.setVector(controlPoints_[1].x + position_.x, 0.0f, controlPoints_[1].z + position_.z);
                p3.setVector(controlPoints_[2].x + position_.x, 0.0f, controlPoints_[2].z + position_.z);

                p1.y = temp->getHeight(p1.x, p1.z);
                p2.y = temp->getHeight(p2.x, p2.z);
                p3.y = temp->getHeight(p3.x, p3.z);

                PRINT_DEBUG_LVL(5, "p1.x=%f, p1.y=%f, p1.z=%f, p2.y=%f, p3.y=%f\n", p1.x, p1.y, p1.z, p2.y, p3.y);

                float height = calcTriangleCenter(p1.y, p2.y, p3.y);

                PRINT_DEBUG_LVL(5, "height =%f\n", height);

                //float position_height = ((Terrain*)temp)->getHeight( position_.x, position_.z );

                //if ( position_.y < ((Terrain*)temp)->getHeight( position_.x, position_.z ) )  {
                    Vector normal, up;
                    normal.calcNorm( p1, p2, p3);

                    PRINT_DEBUG_LVL(5, "normal x=%f, normal y=%f, normal z=%f\n", normal.x, normal.y, normal.z);

                    up.setVector( 0.0f, 1.0f, 0.0f );
                    Vector rotationAxis; rotationAxis.crossProduct( up, normal );
                    float rotationAngle = normal.dotProduct( up );

                    Quaternion orientRotation; orientRotation.buildFromAxis(rotationAxis, rotationAngle);
                    rotation_ = baseRotation * orientRotation;

                    //setRotationAxis(rotationAxis,rotationAngle);
                    position_.y = height;
                    rotationChanged_ = true;
                //}
    }


    void Model::setHeightFromTerrain(Terrain *temp, float offset)
    {
        if (temp == NULL)
            return;

                Vector p1, p2, p3;

                p1.setVector(controlPoints_[0].x + position_.x, 0.0f, controlPoints_[0].z + position_.z);
                p2.setVector(controlPoints_[1].x + position_.x, 0.0f, controlPoints_[1].z + position_.z);
                p3.setVector(controlPoints_[2].x + position_.x, 0.0f, controlPoints_[2].z + position_.z);

                p1.y = temp->getHeight(p1.x, p1.z);
                p2.y = temp->getHeight(p2.x, p2.z);
                p3.y = temp->getHeight(p3.x, p3.z);

                PRINT_DEBUG_LVL(5, "p1.x=%f, p1.y=%f, p1.z=%f, p2.y=%f, p3.y=%f\n", p1.x, p1.y, p1.z, p2.y, p3.y);

                float height = calcTriangleCenter(p1.y, p2.y, p3.y);

                PRINT_DEBUG_LVL(5, "height =%f\n", height);

                //float position_height = ((Terrain*)temp)->getHeight( position_.x, position_.z );

                position_.y = height + offset;

                //}
    }
