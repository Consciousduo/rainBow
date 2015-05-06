

#include "M33.h"

V3& M33::operator[](int i) {

  return rows[i];

}

V3 M33::operator*(V3 v) {

  return V3(rows[0]*v, rows[1]*v, rows[2]*v);

}

V3 M33::GetColumn(int i) {

  V3 ret(rows[0][i], rows[1][i], rows[2][i]);
  return ret;

}

void M33::SetColumn(int i, V3 col) {

  rows[0][i] = col[0];
  rows[1][i] = col[1];
  rows[2][i] = col[2];

}


M33 M33::operator*(M33 m1) {

  M33 &m0 = *this;
  M33 ret;
  ret.SetColumn(0, m0*m1.GetColumn(0));
  ret.SetColumn(1, m0*m1.GetColumn(1));
  ret.SetColumn(2, m0*m1.GetColumn(2));
  return ret;

}

M33 M33::Inverted() {

  M33 ret;
  V3 a = GetColumn(0), b = GetColumn(1), c = GetColumn(2);
  V3 _a = b ^ c; _a = _a / (a * _a);
  V3 _b = c ^ a; _b = _b / (b * _b);
  V3 _c = a ^ b; _c = _c / (c * _c);
  ret[0] = _a;
  ret[1] = _b;
  ret[2] = _c;

  return ret;

}

void M33::SetRotationByY(float theta) {

  float tr = theta / 180.0f * 3.1415926f;
  float ct = cosf(tr);
  float st = sinf(tr);
  rows[0] = V3(ct, 0.0f, st);
  rows[1] = V3(0.0f, 1.0f, 0.0f);
  rows[2] = V3(-st, 0.0, ct);

}
