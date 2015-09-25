//////////////////////////////////////////////////////////////////
// Description : Calls for managing camera position.            //
//////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Object.h"
#include "Frustum.h"

class Camera : public Object {
  public:
    Camera();
    Camera(int tid);
    virtual ~Camera();

    void update(float);
    void setCamera(const Vector&, const Vector&, const Quaternion&, const Vector&);
    void prepareGLView( void );

    // accessor functions
    void getRotationMatrix(Matrix &m) const;

    void printTypeName(void) { PRINT_DEBUG("Camera\n"); }
    void buildLuaObjectTable(lua_State *L);

    void draw(Object*);

    Frustum *getFrustum();

    void drawOutline(Object*) {}; // Camera*
    void drawShadow (Vector*) {}

    void handleCollision(Object*) {};
    void update(Vector*) {};
    void prepare(void) {};
    void animate(float, Object*) {}; // Camera*

    void setProjectionMatrix(void);

    Matrix rotationMatrix; // send to rendering library
    Frustum frustum;
    Matrix projectionMatrix;
    Matrix modelViewMatrix;

    Quaternion worldRotation;

    Matrix final;
    int id_;

  private:
    void initialize(void);
};

#endif
