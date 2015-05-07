#pragma once

#include <ostream>

using namespace std;


class V3 {
  
public:
  float xyz[3];
  V3(float x, float y, float z);
  V3() {};
  float& operator[](int i);
  V3 operator+(V3 v1);
  V3 operator-(V3 v1);
  float operator*(V3 v1);
  V3 operator*(float scf);
  V3 operator/(float scf);
  void SetColor(unsigned int color);
  unsigned int GetColor();
  V3 operator^(V3 op1);
  V3 UnitVector();
  float Length();
  friend ostream& operator<<(ostream& ostr, V3 v);
  V3 RotateThisPointAboutAxis(V3 O, V3 a, float theta);
  V3 RotateThisVectorAboutAxis(V3 a, float theta);
};

class AABB {
public:
  V3 corners[2];
  AABB(V3 pt0);
  void AddPoint(V3 pt);

};