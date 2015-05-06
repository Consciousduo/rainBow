#pragma once

#include "v3.h"

class M33 {

  V3 rows[3];
public:
  M33() {};
  V3& operator[](int i);
  V3 operator*(V3 v);
  M33 operator*(M33 m);
  void SetColumn(int i, V3 col);
  V3 GetColumn(int i);
  M33 Inverted();
  void SetRotationByY(float theta);
};