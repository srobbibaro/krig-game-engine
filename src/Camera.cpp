//////////////////////////////////////////////////////////////////
// Description : Functions for managing the camera position.    //
//////////////////////////////////////////////////////////////////

#include "Camera.h"
#include "api.h"
#include "api_camera.h"

//------------------------------------------------------------------------------
Camera::Camera() : Object() { initialize(); }

//------------------------------------------------------------------------------
Camera::Camera(const int &id) : Object() { initialize(); id_ = id; }

//------------------------------------------------------------------------------
void Camera::initialize() {
  // default camera settings
  baseDirection_.setVector(0.0f, 0.0f, 1.0f);
  direction_.setVector(0.0f, 0.0f, 1.0f);
  worldRotation.buildFromEuler(0.0f, 0.0f, 0.0f);
}

//------------------------------------------------------------------------------
void Camera::setProjectionMatrix() {
  glGetFloatv( GL_PROJECTION_MATRIX, projectionMatrix.data );
}

//------------------------------------------------------------------------------
Camera::~Camera() {}

//------------------------------------------------------------------------------
void Camera::update(const float &timeElapsed) {
  // exectue the current object's update function
  animateScript(timeElapsed);

  // calculate new position using speed_
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

  if (!isInterpolationEnabled_) {
    if (rotationVelocity_.x != 0.0f ||
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

  rotation_.buildRotationMatrix( rotationMatrix );

  direction_.rotateVector( rotationMatrix, baseDirection_ );
  direction_.setVector(direction_.x, direction_.y, -direction_.z);
  direction_.normalize();

  Vector upV;
  upV.setVector(0.0f, 1.0f, 0.0f);

  up_.rotateVector( rotationMatrix, upV );
  up_.normalize();

  orth_.crossProduct(up_, direction_);
  orth_.normalize();

  Matrix translationMatrix;
  translationMatrix.setTranslation(-position_.x, -position_.y, -position_.z);

  modelViewMatrix = rotationMatrix * translationMatrix;

  final = projectionMatrix * modelViewMatrix;

  if (particleSystem_)
    particleSystem_->update(timeElapsed);
}

//------------------------------------------------------------------------------
void Camera::setCamera(
    const Vector &tPos, const Vector &tVel,
    const Quaternion &tRot, const Vector &tRotVel
) {
  position_ = tPos;
  rotation_ = tRot;
  velocity_ = tVel;
  rotationVelocity_ = tRotVel;
}

//------------------------------------------------------------------------------
void Camera::prepareGLView() {
  Quaternion tempQ = rotation_ * worldRotation;
  Matrix m;
  tempQ.buildRotationMatrix(m);

  glMultMatrixf(m.data);
  //glTranslatef(-position.x, -position.y, -position.z);
}

//------------------------------------------------------------------------------
void Camera::getRotationMatrix(Matrix &m) const { m = rotationMatrix; }

//------------------------------------------------------------------------------
Frustum *Camera::getFrustum() { return &frustum; }

//------------------------------------------------------------------------------
void Camera::draw(Object* object) {
  if (particleSystem_)
      particleSystem_->draw();
}

void Camera::buildLuaObjectTable(lua_State *L) {
  Object::buildLuaObjectTable(L);

  lua_pushstring(L, "world_rotation");
  returnQuaternion(L, worldRotation);
  lua_rawset(L, -3);

  luaL_setfuncs(L, krigCameraLib, 0);
}
