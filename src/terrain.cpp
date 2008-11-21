//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : terrain.cpp                                    //
//                                                              //
// Description : Functions for terrain generation and display.  //
//                                                              //
// Date        : 05-8-04                                        //
//////////////////////////////////////////////////////////////////
#include "Terrain.h"


//------------------------------------------------------------------------------
Terrain::Terrain() : Object()
{
    xSize = X_SIZE;
	zSize = Z_SIZE;
	scaleFactor = SCALE_FACTOR;
	active = true;
}

//------------------------------------------------------------------------------
Terrain::~Terrain()
{
    cout << "deleting terrain";
    ObjectNode* obj = (Object*)next; ObjectNode* temp;
    
    while ( obj != NULL )
    {
        temp = obj->next;
        obj->remove();
        delete obj;
        obj = temp;
    }
    cout <<"..done deleting terrain";
    
    // code will have to be added to delete dynamic arrays //
}

//------------------------------------------------------------------------------
void Terrain::draw( Camera* c )
{   
        int x1, x2;
        int z1, z2;
        
        float xStart, zStart;
        float xPer1, xPer2, zPer1, zPer2 ;
        float tPer;
        GLfloat v[3];
        
        xStart = zStart = 0;   
            
        glPushMatrix();
    
    
        QuadTreeNode* n = l->head;
        
        while (n != NULL) {
            glBegin(GL_TRIANGLE_STRIP);
                xStart = n->min[0] / SCALE_FACTOR;
                zStart = n->min[1] /  SCALE_FACTOR;
                                
                x1 = (int)xStart;
                x2 = ((int)xStart) + 1;
                
                z1 = (int)zStart;
                z2 = ((int)zStart) +1;
           
                v[0] = vertex[x1][z1][0];
                v[2] = vertex[x1][z1][2];
                
                xPer1 = (vertex[x1][z1][0] - c->position.x) * (vertex[x1][z1][0] - c->position.x);
                xPer2 = (vertex[x2][z1][0] - c->position.x) * (vertex[x2][z1][0] - c->position.x);
                zPer1 = (vertex[x1][z1][2] - c->position.z)* (vertex[x1][z1][2] - c->position.z);
                zPer2 = (vertex[x1][z2][2] - c->position.z)* (vertex[x1][z2][2] - c->position.z);    
                
                tPer = sqrt(xPer1 + zPer1);
                float rate = .05f;
                float offset = 200.0f;
                    
                //offset = 1000000;
    
                if ( tPer > offset ) {
                    tPer = tPer - offset;
                    v[1] = -(rate * (tPer)) * ( rate * (tPer) )  + vertex[x1][z1][1]; 
                }
                else
                    v[1] = vertex[x1][z1][1];
                    
               
                
                glColor3fv( color[x1][z1] ); 
                glTexCoord1f( lightIntensity[x1][z1] ); 
                glVertex3fv( v );

                v[0] = vertex[x2][z1][0];
                v[2] = vertex[x2][z1][2];
                       
                tPer = sqrt(xPer2 + zPer1);
                
                if ( tPer > offset ) {
                    tPer = tPer - offset;
                    v[1] = -(rate * (tPer)) * ( rate * (tPer) )  + vertex[x2][z1][1]; 
                }
                else
                    v[1] = vertex[x2][z1][1];
                    
              
                
                glColor3fv( color[x2][z1] ); 
                glTexCoord1f( lightIntensity[x2][z1] ); 
                glVertex3fv( v );
                
                
                v[0] = vertex[x1][z2][0];
                v[2] = vertex[x1][z2][2];
                
                tPer = sqrt(xPer1 + zPer2);
                
                 if ( tPer > offset ) {
                    tPer = tPer - offset;
                 
                    v[1] = -(rate * (tPer)) * ( rate * (tPer) )  + vertex[x1][z2][1]; 
                }
                else
                    v[1] = vertex[x1][z2][1];
                    
              
               
                glColor3fv( color[x1][z2] ); 
                glTexCoord1f( lightIntensity[x1][z2] ); 
                glVertex3fv( v );
               
                
                tPer = sqrt(xPer2 + zPer2);
                    
                v[0] = vertex[x2][z2][0];
                v[2] = vertex[x2][z2][2];
                
                if ( tPer > offset ) {
                    tPer = tPer - offset;
                    v[1] = -(rate * (tPer)) * (rate * (tPer) )  + vertex[x2][z2][1]; 
                }
                else
                    v[1] = vertex[x2][z2][1];
               
                
               
                glColor3fv( color[x2][z2] );
                glTexCoord1f( lightIntensity[x2][z2] ); 
                glVertex3fv( v );
            glEnd();
            
            n = n->next;
        }
      
        glPopMatrix();   
}

//------------------------------------------------------------------------------
void Terrain::drawOutline( Camera* c )
{	 
    int x1, x2;
    int z1, z2;
        
    float xStart, zStart;
        
        
        float xPer1, xPer2, zPer1, zPer2 ;
        float tPer;
        GLfloat v[3];
        
        xStart = zStart = 0;   
       
        glPushMatrix();
       
        QuadTreeNode* n = l->head;
        
        
        glColor3f(0.0f, 0.0f, 0.0f);
        
        while (n != NULL) {
            glBegin(GL_TRIANGLES);
                xStart = n->min[0] / SCALE_FACTOR;
                zStart = n->min[1] /  SCALE_FACTOR;
                                
                x1 = (int)xStart;
                x2 = ((int)xStart) + 1;
                
                z1 = (int)zStart;
                z2 = ((int)zStart) +1;
              
                v[0] = vertex[x1][z1][0];
                v[2] = vertex[x1][z1][2];
                
                xPer1 = (vertex[x1][z1][0] - c->position.x) * (vertex[x1][z1][0] - c->position.x);
                xPer2 = (vertex[x2][z1][0] - c->position.x) * (vertex[x2][z1][0] - c->position.x);
                zPer1 = (vertex[x1][z1][2] - c->position.z)* (vertex[x1][z1][2] - c->position.z);
                zPer2 = (vertex[x1][z2][2] - c->position.z)* (vertex[x1][z2][2] - c->position.z);    
                
                tPer = sqrt(xPer1 + zPer1);
                //float rate = .045f;
                float rate = .05f;
                float offset = 200.0f;
        
                if ( tPer > offset ) {
                    tPer = tPer - offset;
                    v[1] = -(rate * (tPer)) * ( rate * (tPer) )  + vertex[x1][z1][1]; 
                }
                else
                    v[1] = vertex[x1][z1][1];
           
               glVertex3fv( v );

                v[0] = vertex[x2][z1][0];
                v[2] = vertex[x2][z1][2];
                       
                tPer = sqrt(xPer2 + zPer1);
                
                if ( tPer > offset ) {
                    tPer = tPer - offset;
                    v[1] = -(rate * (tPer)) * ( rate * (tPer) )  + vertex[x2][z1][1]; 
                }
                else
                    v[1] = vertex[x2][z1][1];
                    
                     glVertex3fv( v );
                
                
                v[0] = vertex[x1][z2][0];
                v[2] = vertex[x1][z2][2];
                
                tPer = sqrt(xPer1 + zPer2);
                
                 if ( tPer > offset ) {
                    tPer = tPer - offset;
                 
                    v[1] = -(rate * (tPer)) * ( rate * (tPer) )  + vertex[x1][z2][1]; 
                }
                else
                    v[1] = vertex[x1][z2][1];
                    
                 glVertex3fv( v );
               
                
                tPer = sqrt(xPer2 + zPer2);
                    
                v[0] = vertex[x2][z2][0];
                v[2] = vertex[x2][z2][2];
                
                if ( tPer > offset ) {
                    tPer = tPer - offset;
                    v[1] = -(rate * (tPer)) * (rate * (tPer) )  + vertex[x2][z2][1]; 
                }
                else
                    v[1] = vertex[x2][z2][1];
               
                 glVertex3fv( v );
            glEnd();
            
            n = n->next;
        }
        
        //cout << "draw count: " << num << "\n";
        
    
        glPopMatrix();
        

       
}

//------------------------------------------------------------------------------
void Terrain::generateTerrain( void )
{
   
}

//------------------------------------------------------------------------------
void Terrain::calcTerrainNorm( Vector* light )
{
    /*
   Vector surfaceNormal;
   Vector vertexNormal[ 17 * Z_SIZE ];
   Vector temp1;
   Vector temp2;
   GLfloat tempLightIntensity;
    
   Vector temp[5];
   int count = 0;
    
   Matrix tempMatrix;
   tempMatrix.loadIdentity();
    
    
   int x1, x2;    
    
   // calculate surface normal
   for ( int z = 0; z < zSize-1; z++ ) {
      for ( int x = 0; x < 16; x++ )  {	
         x1 = terrainIndex[x];
         x2 = terrainIndex[x+1];
           
           temp[0].setVector( terrain[z*X_SIZE+x1][0], terrain[z*X_SIZE+x1][1], terrain[z*X_SIZE+x1][2]  );
           temp[1].setVector( terrain[(z+1)*X_SIZE+x1][0], terrain[(z+1)*X_SIZE+x1][1], terrain[(z+1)*X_SIZE+x1][2] );
           temp[2].setVector( terrain[z*X_SIZE+x2][0], terrain[z*X_SIZE+x2][1], terrain[z*X_SIZE+x2][2] );
           temp[3].setVector( terrain[(z+1)*X_SIZE+x2][0], terrain[(z+1)*X_SIZE+x2][1], terrain[(z+1)*X_SIZE+x2][2] );
        
           surfaceNormal.calcNorm( temp[0], temp[1], temp[2] );

           vertexNormal[z*17+x].x += surfaceNormal.x;
           vertexNormal[z*17+x].y += surfaceNormal.y;
           vertexNormal[z*17+x].z += surfaceNormal.z;
           
           vertexNormal[(z+1)*17+x].x += surfaceNormal.x;
           vertexNormal[(z+1)*17+x].y += surfaceNormal.y;
           vertexNormal[(z+1)*17+x].z += surfaceNormal.z;

           vertexNormal[z*17+x+1].x += surfaceNormal.x;
           vertexNormal[z*17+x+1].y += surfaceNormal.y;
           vertexNormal[z*17+x+1].z += surfaceNormal.z;

           surfaceNormal.calcNorm( temp[2], temp[1], temp[3] );

           vertexNormal[z*17+x+1].x += surfaceNormal.x;
           vertexNormal[z*17+x+1].y += surfaceNormal.y;
           vertexNormal[z*17+x+1].z += surfaceNormal.z;

           vertexNormal[(z+1)*17+x].x += surfaceNormal.x;
           vertexNormal[(z+1)*17+x].y += surfaceNormal.y;
           vertexNormal[(z+1)*17+x].z += surfaceNormal.z;


           vertexNormal[(z+1)*17+x+1].x += surfaceNormal.x;
           vertexNormal[(z+1)*17+x+1].y += surfaceNormal.y;
           vertexNormal[(z+1)*17+x+1].z += surfaceNormal.z;

        }
    }
    
    // calculate light intensity at every vertex
    for ( int z = 0; z < zSize; z++ )
    {    
        for ( int x = 0; x < 17; x++ )
        {
            vertexNormal[x+17*z].normalize();
            
            temp2.rotateVector( tempMatrix, vertexNormal[x+17*z] );
            temp2.normalize();
    
            tempLightIntensity = temp2.dotProduct( *light );
    
            if ( tempLightIntensity == 1.0f )
                tempLightIntensity = 0.5f;
            else if ( tempLightIntensity < 0.0f )
                tempLightIntensity = 0.0f;       
            
            x1 = terrainIndex[x];
            lightIntensity[z*X_SIZE+x1] = tempLightIntensity;
            
            count +=2;
        }
    }
    */
    
    Vector surfaceNormal;
   Vector vertexNormal[X_SIZE][Z_SIZE];
   Vector temp1;
   Vector temp2;
   GLfloat tempLightIntensity;
    
   Vector temp[5];
   int count = 0;
    
   Matrix tempMatrix;
   tempMatrix.loadIdentity();
    
    
   int x1, x2;
   int z1, z2;    
    
   // calculate surface normal
   for ( int z = 0; z < (zSize-1); z++ ) {
      for ( int x = 0; x < (xSize-1); x++ )  {
            x1 = x;
            x2 = x+1;
            z1 = z;
            z2 = z+1;
           	    
           temp[0].setVector( vertex[x1][z1][0], vertex[x1][z1][1], vertex[x1][z1][2] );
           temp[1].setVector( vertex[x1][z2][0], vertex[x1][z2][1], vertex[x1][z2][2] );
           temp[2].setVector( vertex[x2][z1][0], vertex[x2][z1][1], vertex[x2][z1][2] );
           temp[3].setVector( vertex[x2][z2][0], vertex[x2][z2][1], vertex[x2][z2][2] );
        
           surfaceNormal.calcNorm( temp[0], temp[2], temp[1] );

           vertexNormal[x1][z1].x += surfaceNormal.x;
           vertexNormal[x1][z1].y += surfaceNormal.y;
           vertexNormal[x1][z1].z += surfaceNormal.z;
           
           vertexNormal[x1][z2].x += surfaceNormal.x;
           vertexNormal[x1][z2].y += surfaceNormal.y;
           vertexNormal[x1][z2].z += surfaceNormal.z;

           vertexNormal[x2][z1].x += surfaceNormal.x;
           vertexNormal[x2][z1].y += surfaceNormal.y;
           vertexNormal[x2][z1].z += surfaceNormal.z;

           surfaceNormal.calcNorm( temp[2], temp[3], temp[1] );

           vertexNormal[x2][z1].x += surfaceNormal.x;
           vertexNormal[x2][z1].y += surfaceNormal.y;
           vertexNormal[x2][z1].z += surfaceNormal.z;

           vertexNormal[x1][z2].x += surfaceNormal.x;
           vertexNormal[x1][z2].y += surfaceNormal.y;
           vertexNormal[x1][z2].z += surfaceNormal.z;


           vertexNormal[x2][z2].x += surfaceNormal.x;
           vertexNormal[x2][z2].y += surfaceNormal.y;
           vertexNormal[x2][z2].z += surfaceNormal.z;
        }
    }
    
    // calculate light intensity at every vertex
    for ( int z = 0; z < zSize; z++ ) {    
        for ( int x = 0; x < xSize; x++ ) {
            vertexNormal[x][z].normalize();
            
            temp2.rotateVector( tempMatrix, vertexNormal[x][z] );
            temp2.normalize();
    
            tempLightIntensity = temp2.dotProduct( *light );
    
            // light correction //
            if ( tempLightIntensity == 1.0f )
                tempLightIntensity = 0.5f;
            else if ( tempLightIntensity < 0.0f )
                tempLightIntensity = 0.0f;       
            
            lightIntensity[x][z] = tempLightIntensity;
        }
    }  
}

//------------------------------------------------------------------------------
void Terrain::update( Vector* light )
{
    //calcTerrainNorm( light );
}

//------------------------------------------------------------------------------
float Terrain::getHeight( float x, float z )
{
    float terX, terZ;

    // x and z value in terms of the terrain
    terX = x/scaleFactor;
    terZ = z/scaleFactor;
    
    // calculate cell row and column into array
    int row, col1, col2;
    col1 = (int)(terX);
    row = (int)(terZ);
    col2 = col1 + 1;
    
    // test to make sure still in terrain
    if ( col2 >= xSize )
        col2 -= xSize;
    
    // calculate percentage into cell
    float perX, perZ;
    perX = fmod(terX, xSize) - col1;
    perZ = float(terZ) - row;
    
    // get height values at all 4 corners
    float height1, height2, height3, height4;
    height1 = vertex[row][col1][1];
    height2 = vertex[row][col2][1];
    height3 = vertex[row+1][col1][1];
    height4 = vertex[row+1][col2][1];
    
    // calculate the final height using bilinear interpolation
    float th1 = ( 1 - perZ )*height1 + (perZ*height3 );
    float th2 = ( 1 - perZ )*height2 + (perZ*height4 );
    
    float finalHeight = ( 1 - perX )*th1 + ( perX * th2 );
    
    return ( finalHeight );
}

//------------------------------------------------------------------------------
void Terrain::animate( float elapsedTime, Camera* c )
{
    GLfloat min[] = { 9999.0f, 9999.0f, 9999.0f };
    GLfloat max[] = { -9999.0f, -9999.0f, -9999.0f };
    
    totalTime += elapsedTime;
    /*
    int terX = int(c->position.x/scaleFactor) - 7;
    if ( terX < 0 )
        terX = 0;
        
    int startX = terX % X_SIZE, x1;
    int scx = terX * int(scaleFactor), cx;
    
        for ( int z = 0; z < zSize; z++ )
        {
            cx = scx;
            for ( int x = 0; x < 17; x++ )  
	        {	
                x1 = startX + x;
                
                if ( x1 >= X_SIZE )    
                    x1 -= X_SIZE;

                terrainIndex[x] = x1;
                terrain[z*X_SIZE+x1][0] = cx;
                
                if ( terrainType == 3 )
                {
                    if ( z < zSize -1 && z < 3 )
                        terrain[z*X_SIZE+x1][1] = 2*sin( (totalTime+x1+((z%3)) )+1 );
                    else if ( z < zSize -1 )
                        terrain[z*X_SIZE+x1][1] = 2*sin((totalTime+x1+((-z%3)))+1);
                }
            */ 
                /* 
                for ( int j = 0; j < 3; j++ )
                {
                    if ( terrain[z*X_SIZE+x1][j] > max[j] )
                        max[j] = terrain[z*X_SIZE+x1][j];
                
                    if ( terrain[z*X_SIZE+x1][j] < min[j] )
                        min[j] = terrain[z*X_SIZE+x1][j];
                }
                cx+=int(scaleFactor);
            }
        }  
        */
        
        int x1, x2;
        int z1, z2;
        
        float xStart, zStart;
        float xPer1, xPer2, zPer1, zPer2 ;
        float tPer;
        GLfloat v[3];
        
        xStart = zStart = 0;   
     
        QuadTreeNode* n = l->head;
        
        while (n != NULL) {
                xStart = n->min[0] / SCALE_FACTOR;
                zStart = n->min[1] /  SCALE_FACTOR;
                                
                x1 = (int)xStart;
                x2 = ((int)xStart) + 1;
                
                z1 = (int)zStart;
                z2 = ((int)zStart) +1;
           
                 if ( type[x1][z1] == 1 )
                {
                    if ( z1 < zSize -1 && z1 < 3 )
                        vertex[x1][z1][1] = 2*sin( (totalTime+x1+((z1%3)) )+1 );
                    else if ( z1 < zSize -1 )
                        vertex[x1][z1][1] = 2*sin((totalTime+x1+((-z1%3)))+1);
                }
            
            n = n->next;
        }                      
                                                  
    collisionBox[0].setVector( min[0], min[1], min[2] );
    collisionBox[1].setVector( max[0], max[1], max[2] );
    
}

//------------------------------------------------------------------------------
void Terrain::loadTerrain( char* filePath, Vector* light )
{
    ifstream fin;
    fin.open(filePath);
        fin >> xSize;
        fin >> zSize;
        fin >> scaleFactor;
        
        fin >> light->x;
        fin >> light->y;
        fin >> light->z;
        
        for (int z = 0; z < zSize; z++) {
            for (int x = 0; x < xSize; x++) {
                fin >> vertex[x][z][1];
                       vertex[x][z][0] = x * scaleFactor;
                       vertex[x][z][2] = -z * scaleFactor;
                fin >> color[x][z][0] >> color[x][z][1] >> color[x][z][2];
                fin >> type[x][z];
            }
        }
        
    fin.close();
        
    // calculate normals in code for now, later add static calculation to be
    // loaded in... only water will be dynamic    
    calcTerrainNorm(light);
}

//------------------------------------------------------------------------------
void Terrain::drawGrid(void)
{
    glPushMatrix();
    //glDepthFunc( GL_ALWAYS );
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    
    for (int x = 0; x < xSize; x++) {
        
        
        glVertex3f(x*SCALE_FACTOR, 10.0f, 0.0f);
        glVertex3f(x*SCALE_FACTOR, 10.0f, -(zSize-1)*SCALE_FACTOR);
        
    } 
   
    for (int z = 0; z < zSize; z++ ) {
        
        glVertex3f(0.0f, 10.0f, -z*SCALE_FACTOR);
        glVertex3f((xSize-1)*SCALE_FACTOR, 10.0f, -z*SCALE_FACTOR);    
    } 
    
    
    glEnd();   
    
    glPopMatrix(); 
}

//------------------------------------------------------------------------------
void Terrain::saveTerrain( char* filePath, Vector* light)
{
    ofstream fin;
    fin.open(filePath);
        fin << xSize << endl;
        fin << zSize << endl;
        fin << scaleFactor << endl;
        
        fin << light->x << endl;
        fin << light->y << endl;
        fin << light->z << endl;
        
        for (int z = 0; z < zSize; z++) {
            for (int x = 0; x < xSize; x++) {
                fin << vertex[x][z][1] << endl;
                      
                fin << color[x][z][0] <<  "\t" << color[x][z][1]<< "\t" << color[x][z][2] << endl;
                fin << type[x][z] << endl;
            }
        }
        
    fin.close();
}
