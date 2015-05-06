

#include "v3.h"
#include "m33.h"


V3::V3(float x, float y, float z) {

  xyz[0] = x;
  xyz[1] = y;
  xyz[2] = z;

}

float& V3::operator[](int i) {

  return xyz[i];

}


ostream& operator<<(ostream& ostr, V3 v) {

  ostr << v[0] << " " << v[1] << " "  << v[2]; 
  return ostr;

}

V3 V3::operator+(V3 v1) {

  V3 &v0 = (*this);
  V3 ret(v0[0]+v1[0], v0[1]+v1[1], v0[2]+v1[2]);
  return ret;

}

V3 V3::operator-(V3 v1) {

  V3 &v0 = (*this);
  V3 ret(v0[0]-v1[0], v0[1]-v1[1], v0[2]-v1[2]);
  return ret;

}

float V3::operator*(V3 v1) {

  V3 &v0 = (*this);
  float ret = v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2];
  return ret;

}

void V3::SetColor(unsigned int color) {

  xyz[0] = ((float) (((unsigned char*)&color)[0])) / 255.0f;
  xyz[1] = ((float) (((unsigned char*)&color)[1])) / 255.0f;
  xyz[2] = ((float) (((unsigned char*)&color)[2])) / 255.0f;


}

unsigned int V3::GetColor() {

  unsigned int ret = 0xFF000000;
  int red = (int) (xyz[0] * 256.0f);
  if (red == 256)
    red = 255;
  ret += red;

  int green = (int) (xyz[1] * 256.0f);
  if (green == 256)
    green = 255;
  ret += green << 8;

  int blue = (int) (xyz[2] * 256.0f);
  if (blue == 256)
    blue = 255;
  ret += blue << 16;

  return ret;

}

V3 V3::operator*(float scf) {

  V3 ret = *this;
  ret[0] *= scf;
  ret[1] *= scf;
  ret[2] *= scf;
  return ret;

}


V3 V3::operator/(float scf) {

  return (*this)*(1.0f/scf);

}

// cross product
V3 V3::operator^(V3 op1) {

  V3 ret;
  ret.xyz[0] = xyz[1]*op1.xyz[2]-xyz[2]*op1.xyz[1];
  ret.xyz[1] = xyz[2]*op1.xyz[0]-xyz[0]*op1.xyz[2];
  ret.xyz[2] = xyz[0]*op1.xyz[1]-xyz[1]*op1.xyz[0];

  return ret;

}

V3 V3::UnitVector() {

  V3 ret = (*this) / Length();

  return ret;

}

float V3::Length() {

  return sqrtf(*this * (*this));

}

AABB::AABB(V3 pt0) {

  corners[0] = corners[1] = pt0;

}

void AABB::AddPoint(V3 pt) {

  if (pt[0] < corners[0][0])
    corners[0][0] = pt[0];
  if (pt[1] < corners[0][1])
    corners[0][1] = pt[1];
  if (pt[2] < corners[0][2])
    corners[0][2] = pt[2];

  if (pt[0] > corners[1][0])
    corners[1][0] = pt[0];
  if (pt[1] > corners[1][1])
    corners[1][1] = pt[1];
  if (pt[2] > corners[1][2])
    corners[1][2] = pt[2];


}


V3 V3::RotateThisPointAboutAxis(V3 O, V3 a, float theta) {

  V3 ret;

  // construct local coordinate system with a as second axis
  V3 aux;
  if (fabsf(a[0]) < fabsf(a[1])) {
    aux = V3(1.0f, 0.0f, 0.0f);
  }
  else {
    aux = V3(0.0f, 1.0f, 0.0f);
  }

  V3 ax = (aux ^ a).UnitVector();
  V3 az = (ax ^ a).UnitVector();

  M33 lcs;
  lcs[0] = ax;
  lcs[1] = a;
  lcs[2] = az;

  // transform to local coordinate system
  V3 &P = (*this);
  V3 lP = lcs*(P-O);

  // rotate in local coordinate system
  M33 rm;
  rm.SetRotationByY(theta);
  V3 rlP = rm*lP;

  // transform back to world
  ret = lcs.Inverted()*rlP+O;

  return ret;

}


V3 V3::RotateThisVectorAboutAxis(V3 a, float theta) {

  V3 ret;

  // construct local coordinate system with a as second axis
  V3 aux;
  if (fabsf(a[0]) < fabsf(a[1])) {
    aux = V3(1.0f, 0.0f, 0.0f);
  }
  else {
    aux = V3(0.0f, 1.0f, 0.0f);
  }

  V3 ax = (aux ^ a).UnitVector();
  V3 az = (ax ^ a).UnitVector();

  M33 lcs;
  lcs[0] = ax;
  lcs[1] = a;
  lcs[2] = az;

  // transform to local coordinate system
  V3 &P = (*this);
  V3 lP = lcs*P;

  // rotate in local coordinate system
  M33 rm;
  rm.SetRotationByY(theta);
  V3 rlP = rm*lP;

  // transform back to world
  ret = lcs.Inverted()*rlP;

  return ret;

}

