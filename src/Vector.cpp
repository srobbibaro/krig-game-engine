#include "Vector.h"

//------------------------------------------------------------------------------
Vector::Vector() {
  x = y = z = 0.0f;
}

//------------------------------------------------------------------------------
Vector::Vector(const float &tx, const float &ty, const float &tz) {
  x = tx; y = ty; z = tz;
}

//------------------------------------------------------------------------------
void Vector::setVector(const float &tx, const float &ty, const float &tz) {
  x = tx; y = ty; z = tz;
}

//------------------------------------------------------------------------------
void Vector::normalize() {
  float len = sqrtf((x * x) + (y * y) + (z * z));

  if (len != 0.0f) {
    x /= len;
    y /= len;
    z /= len;
  }
}

//------------------------------------------------------------------------------
float Vector::dotProduct(const Vector &rhs) {
  return (((x * rhs.x) + (y * rhs.y) + (z * rhs.z)));
}

//------------------------------------------------------------------------------
void Vector::operator =(const Vector &rhs) {
  x = rhs.x; y = rhs.y; z = rhs.z;
}

//------------------------------------------------------------------------------
void Vector::rotateVector(const Matrix &M, const Vector &V) {
  x = (M.data[0] * V.x) + (M.data[4] * V.y) + (M.data[8] * V.z);
  y = (M.data[1] * V.x) + (M.data[5] * V.y) + (M.data[9] * V.z);
  z = (M.data[2] * V.x) + (M.data[6] * V.y) + (M.data[10] * V.z);
}

//------------------------------------------------------------------------------
void Vector::crossProduct(const Vector &d1, const Vector &d2) {
  x = d1.y * d2.z - d1.z * d2.y;
  y = d1.z * d2.x - d1.x * d2.z;
  z = d1.x * d2.y - d1.y * d2.x;
}

//------------------------------------------------------------------------------
void Vector::calcNorm(const Vector &p1, const Vector &p2, const Vector &p3) {
  // calculate the surface normal of the triangle formed by p1, p2 and p3
  Vector t1;
  Vector t2;

  t1.setVector(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
  t2.setVector(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

  crossProduct(t1, t2);
  normalize();
}

//------------------------------------------------------------------------------
void Vector::transformVector(const Matrix &m, const Vector &v) {
  float h;

  x = (v.x * m.data[0]) + (v.y * m.data[4]) + (v.z * m.data[8]) + m.data[12];
  y = (v.x * m.data[1]) + (v.y * m.data[5]) + (v.z * m.data[9]) + m.data[13];
  z = (v.x * m.data[2]) + (v.y * m.data[6]) + (v.z * m.data[10]) + m.data[14];
  h = (v.x * m.data[3]) + (v.y * m.data[7]) + (v.z * m.data[11]) + m.data[15];

  // x /= h;
  // y /= h;
  // z /= h;
}

//------------------------------------------------------------------------------
void Vector::average(const Vector &a, const Vector &b) {
  x = a.x + b.x;
  y = a.y + b.y;
  z = a.z + b.z;

  normalize();
}

//------------------------------------------------------------------------------
void Vector::scale(const float &m) {
  x *= m; y *= m; z *= m;
}

//------------------------------------------------------------------------------
float Vector::getDistance(const Vector &t) {
  float dx = t.x - x;
  float dy = t.y - y;
  float dz = t.z - z;

  return sqrtf(dx * dx + dy * dy + dz * dz);
}

//------------------------------------------------------------------------------
float Vector::getSum(void) {
  return (x + y + z);
}

//------------------------------------------------------------------------------
float Vector::getScaler(const Vector &vector) {
  Vector t;
  t.x = x * vector.x;
  t.y = y * vector.y;
  t.z = z * vector.z;

  return (t.x + t.y + t.z);
}

//------------------------------------------------------------------------------
bool Vector::intersectBox(
  const Vector &rayPosition, Vector collisionBox[],
  float extend, Vector &tv
) {
  bool hit = false;
  float s  = 0;

  // test planes with fixed x value //
  if (x > 0) {
    if (rayPosition.x <= collisionBox[1].x + extend)
      s = (collisionBox[1].x + extend - rayPosition.x) / x;
  }
  else if (x < 0) {
    if (rayPosition.x >= collisionBox[0].x - extend)
      s = (collisionBox[0].x - extend - rayPosition.x) / x;
  }

  if (s != 0) {
    tv.x = rayPosition.x + x * s;
    tv.y = rayPosition.y + y * s;
    tv.z = rayPosition.z + z * s;

    if (tv.y <= collisionBox[1].y + extend && tv.y >= collisionBox[0].y - extend &&
        tv.z <= collisionBox[1].z + extend && tv.z >= collisionBox[0].z - extend) {
      hit = true;
    }
  }
  //////////////////////////////////////

  // if no hit on x planes test planes with fixed y value //
  if (!hit) {
    s = 0;

    if (y > 0) {
      if (rayPosition.y <= collisionBox[1].y + extend)
        s = (collisionBox[1].y + extend - rayPosition.y) / y;

    }
    else if (y < 0) {
      if (rayPosition.y >= collisionBox[0].y - extend)
        s = (collisionBox[0].y - extend - rayPosition.y) / y;
    }

    if (s != 0) {
      if (tv.x <= collisionBox[1].x + extend && tv.x >= collisionBox[0].x - extend &&
          tv.z <= collisionBox[1].z + extend && tv.z >= collisionBox[0].z - extend) {
        hit = true;
      }
    }
  }
  ////////////////////////////////////////////////////////////

  // if no hit on x or y planes test planes with fixed z value //
  if (!hit) {
    s = 0;

    if (z > 0) {
      if (rayPosition.z <= collisionBox[1].z + extend)
        s = (collisionBox[1].z + extend - rayPosition.z) / z;
    }
    else if (z < 0) {
      if (rayPosition.z >= collisionBox[0].z-extend)
        s = (collisionBox[0].z - extend - rayPosition.z) / z;
    }

    if (s != 0) {
      tv.x = rayPosition.x + x * s;
      tv.y = rayPosition.y + y * s;
      tv.z = rayPosition.z + z * s;

      if (tv.x <= collisionBox[1].x + extend && tv.x >= collisionBox[0].x - extend &&
          tv.y <= collisionBox[1].y + extend && tv.y >= collisionBox[0].y - extend) {
        hit = true;
      }
    }
  }
  ///////////////////////////////////////////////////////////////////

  return hit;
}

//------------------------------------------------------------------------------
bool Vector::intersectBox(const Vector &rayPosition, Vector collisionBox[], float extend) {
  Vector tv;
  bool hit = false;
  float s  = 0;

  // test planes with fixed x value //
  if (x > 0) {
    if (rayPosition.x <= collisionBox[1].x + extend)
      s = (collisionBox[1].x + extend - rayPosition.x) / x;
  }
  else if (x < 0) {
    if (rayPosition.x >= collisionBox[0].x - extend)
      s = (collisionBox[0].x - extend - rayPosition.x) / x;
  }

  if (s != 0) {
    tv.x = rayPosition.x + x * s;
    tv.y = rayPosition.y + y * s;
    tv.z = rayPosition.z + z * s;

    if (tv.y <= collisionBox[1].y + extend && tv.y >= collisionBox[0].y - extend &&
        tv.z <= collisionBox[1].z + extend && tv.z >= collisionBox[0].z - extend) {
      hit = true;
    }
  }
  //////////////////////////////////////

  // if no hit on x planes test planes with fixed y value //
  if (!hit) {
    s = 0;

    if (y > 0) {
      if (rayPosition.y <= collisionBox[1].y + extend)
        s = (collisionBox[1].y + extend - rayPosition.y) / y;
    }
    else if (y < 0) {
      if (rayPosition.y >= collisionBox[0].y - extend)
        s = (collisionBox[0].y - extend - rayPosition.y) / y;
    }

    if (s != 0) {
      if (tv.x <= collisionBox[1].x + extend && tv.x >= collisionBox[0].x - extend &&
          tv.z <= collisionBox[1].z + extend && tv.z >= collisionBox[0].z - extend) {
        hit = true;
      }
    }
  }
  ////////////////////////////////////////////////////////////

  // if no hit on x or y planes test planes with fixed z value //
  if (!hit) {
    s = 0;

    if (z > 0) {
      if (rayPosition.z <= collisionBox[1].z + extend)
        s = (collisionBox[1].z + extend - rayPosition.z) / z;
    }
    else if (z < 0) {
      if (rayPosition.z >= collisionBox[0].z - extend)
        s = (collisionBox[0].z - extend - rayPosition.z) / z;
    }

    if (s != 0) {
      tv.x = rayPosition.x + x * s;
      tv.y = rayPosition.y + y * s;
      tv.z = rayPosition.z + z * s;

      if (tv.x <= collisionBox[1].x + extend && tv.x >= collisionBox[0].x - extend &&
          tv.y <= collisionBox[1].y + extend && tv.y >= collisionBox[0].y - extend) {
        hit = true;
      }
    }
  }
  ///////////////////////////////////////////////////////////////////

  return hit;
}

//------------------------------------------------------------------------------
Vector Vector::operator +(const Vector &a) {
  Vector t;
  t.setVector(x + a.x, y + a.y, z + a.z);
  return t;
}

//------------------------------------------------------------------------------
Vector Vector::operator *(float a) {
  return Vector(a * x, a * y, a * z);
}

//------------------------------------------------------------------------------
void Vector::operator +=(const Vector &a) {
  x += (a.x); y += (a.y); z += (a.z);
}

//------------------------------------------------------------------------------
void Vector::operator -=(const Vector &a) {
  x -= (a.x); y -= (a.y); z -= (a.z);
}

//------------------------------------------------------------------------------
float Vector::getLength(void) {
  return (sqrtf((x * x) + (y * y) + (z * z)));
}
