//////////////////////////////////////////////////////////////////
// Description : Class for handling terrain display and         //
//               generation.                                    //
//////////////////////////////////////////////////////////////////
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "constants.h"
#include "Object.h"
#include "DisplayList.h"

class Terrain : public Object {
  public:
    Terrain();
    ~Terrain();

    void draw(Object*);
    void drawOutline(Object*);

    void handleCollision(Object* temp) {}
    void animate(const float&, Object*);
    void generate();

    void calcTerrainNorm(Vector*);
    void calcViewableTerrainNorm();
    void update(Vector*);

    void init();
    void load(const char*, Vector*);
    void save(const char*, Vector*);
    void unload();

    float getHeight(const float&, const float&);

    void printTypeName() { PRINT_DEBUG("Terrain\n"); }
    virtual void buildLuaObjectTable(lua_State *L);
    virtual void transferLuaObjectTable(lua_State *L);
    void drawGrid();

    void setVertexHeight(const int&, const int&, const float&);
    void setVertexType(const int&, const int&, const int&);
    void setVertexColor(const int&, const int&, const Vector&);
    float getVertexHeight(const int&, const int&);
    int getVertexType(const int&, const int&);
    Vector getVertexColor(const int&, const int&);

    DisplayList* getDisplayList()                 { return displayList_; }
    void setDisplayList(DisplayList* displayList) { displayList_ = displayList; }

    int getXSize()       { return xSize_; }
    int getZSize()       { return zSize_; }
    int getScaleFactor() { return scaleFactor_; }

    void setCurveDistance(const GLfloat &curveDistance) { curveDistance_ = curveDistance; }
    void setCurveRate(const GLfloat &curveRate)         { curveRate_ = curveRate; }
    void setIsCurveEnabled(const bool &isCurveEnabled)  { isCurveEnabled_ = isCurveEnabled; }
    GLfloat getCurveDistance()                          { return curveDistance_; };
    GLfloat getCurveRate()                              { return curveRate_; };
    GLfloat getIsCurveEnabled()                         { return isCurveEnabled_; }

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
