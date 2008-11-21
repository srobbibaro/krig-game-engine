#include "windows.h"
#include "QuadTree.h"

//------------------------------------------------------------------------------
QuadTree::QuadTree(void)
{
    root = NULL;
}

//------------------------------------------------------------------------------
QuadTree::~QuadTree(void)
{
}

//------------------------------------------------------------------------------
int QuadTree::buildTree(Terrain* t)
{   
   buildTree(root, 0.0f, ((t->xSize-1.0f) * t->scaleFactor), 0.0f, ((t->zSize-1.0f) * t->scaleFactor), t->scaleFactor);
}
//------------------------------------------------------------------------------
int QuadTree::buildTree(QuadTreeNode* &p, float xMin, float xMax, float zMin, float zMax, float scaleFactor)
{ 
        //cout << "leaf:" << (((xMax-xMin)/2.0f)+xMin) << " "  << (-(((zMax-zMin)/2.0f)+zMin)) << " "  << sqrt(((zMax - zMin) * (zMax - zMin)) + ((xMax - xMin) * (xMax - xMin))) << endl;
       
        p = new QuadTreeNode();
        
        p->min[0] = xMin;
        p->min[1] = zMin;
        p->max[0] = xMax;
        p->max[1] = zMax;
       
        p->boundingSphere.setSphere(
            (((xMax-xMin)/2.0f)+xMin), 
            0.0f, 
            (-(((zMax-zMin)/2.0f)+zMin)), 
            sqrt(((zMax - zMin) * (zMax - zMin)) + ((xMax - xMin) * (xMax - xMin)))
        );
        
      
    if (((xMax - xMin) > scaleFactor) && ((zMax - zMin) <= scaleFactor)) { 
        buildTree(p->child[0], ((xMax+xMin)/2.0f), xMax, zMin, zMax, scaleFactor);
        buildTree(p->child[1], xMin, ((xMax+xMin)/2.0f), zMin, zMax , scaleFactor);        
    }
    else if (((xMax - xMin) <= scaleFactor) && ((zMax - zMin) > scaleFactor)) {
        buildTree(p->child[2], xMin, xMax, zMin, ((zMax+zMin)/2.0f), scaleFactor);
        buildTree(p->child[3], xMin, xMax, (zMax+zMin)/2.0f, zMax, scaleFactor);
       
    }
    else if (((xMax - xMin) > scaleFactor) && ((zMax - zMin) > scaleFactor)) {
        buildTree(p->child[0], xMin, ((xMax+xMin)/2.0f), zMin, ((zMax+zMin)/2.0f), scaleFactor);
        buildTree(p->child[1], xMin, ((xMax+xMin)/2.0f), (zMax+zMin)/2.0f, zMax, scaleFactor);
        buildTree(p->child[2], ((xMax+xMin)/2.0f), xMax,  zMin, ((zMax+zMin)/2.0f), scaleFactor);
        buildTree(p->child[3], ((xMax+xMin)/2.0f), xMax, ((zMax+zMin)/2.0f), zMax , scaleFactor); 
    }
    else {
        //cout << "leaf: " << xMin << " " << xMax << " " << zMin << " " << zMax << endl;
        //cout << "leaf:" << (((xMax-xMin)/2.0f)+xMin) << " "  << (-(((zMax-zMin)/2.0f)+zMin)) << " "  << sqrt(((zMax - zMin) * (zMax - zMin)) + ((xMax - xMin) * (xMax - xMin))) << endl;
    }
    
    return (1);
}

//------------------------------------------------------------------------------
void QuadTree::traverseTree(void)
{
    traverseTree(root);
}

//------------------------------------------------------------------------------
void QuadTree::traverseTree(QuadTreeNode* n)
{
    if (n != NULL) {
        cout << "---------------------------" << endl;
        cout << "min x: " << n->min[0] << " max x: " << n->max[0] << endl;
        cout << "min z: " << n->min[1] << " max z: " << n->max[1] << endl;
        cout << "---------------------------" << endl;
    
        for (int i = 0; i < 4; i++) {
            traverseTree(n->child[i]);
        }
    }
    else {
        cout << "------------------" << endl;
        cout << "leaf node" << endl;
        cout << "------------------" << endl;
    }
}

//------------------------------------------------------------------------------
int QuadTree::buildDisplayList(DisplayList* l, Camera* c)
{
    buildDisplayList(root, l, c);
   
    return (0);
}

//------------------------------------------------------------------------------
int QuadTree::buildLeafList(DisplayList* l)
{
    buildLeafList(root, l);
   
    return (0);
}

//------------------------------------------------------------------------------
int QuadTree::buildDisplayList(QuadTreeNode* n, DisplayList* l, Camera* c)
{
    if (n != NULL) {  
        int r =  c->frustum.testSphere(n->boundingSphere);
        //cout << "res: " << r << endl;
		switch(r) 
        {
			case -1:
				return (0);
			
            case 1:
				//bTestChildren = false;
				break;
			case 0:
			// check if the box is in view
				//switch(pPovCamera->Frustrum().ContainsAaBox(pNode->m_bbox)) {
				//case IN:
				//bTestChildren = false;
				//	break;
			     //case OUT:
					//return;
				//}
				break;
        }
    
        if (n->child[0] == NULL && n->child[1] == NULL && n->child[2] == NULL && n->child[3] == NULL) { 
            l->insertLast(n);
        }
        
        for (int i = 0; i < 4; i++) {
            buildDisplayList(n->child[i], l, c);
        }
    }
    
    return (0); 
}


//------------------------------------------------------------------------------
int QuadTree::buildLeafList(QuadTreeNode* n, DisplayList* l)
{ 
     if (n != NULL) {  
       if (n->child[0] == NULL && n->child[1] == NULL && n->child[2] == NULL && n->child[3] == NULL) { 
            l->insertLast(n);
        }
        
        for (int i = 0; i < 4; i++) {
            buildLeafList(n->child[i], l);
        }
    }
    
    return (0);
}


