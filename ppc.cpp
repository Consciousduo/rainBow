#include "ppc.h"

#include "v3.h"
#include "m33.h"
#include "framebuffer.h"


PPC::PPC(float hfov,  int w, int h) : w(w), h(h) {

  C = V3(0.0f, 0.0f, 0.0f);
  a = V3(1.0f, 0.0f, 0.0f);
  b = V3(0.0f, -1.0f, 0.0f);
  float f = (float)w/2.0f/tan(hfov/180.0f*3.1415926f/2.0f);
  c = V3(-(float)w/2.0f, (float)h/2.0f, -f);

}

bool PPC::Project(V3 p, V3 &pp) {

  pp[0] = pp[1] = pp[2] = FLT_MAX;
  M33 m;
  m.SetColumn(0, a);
  m.SetColumn(1, b);
  m.SetColumn(2, c);
  V3 q = m.Inverted()*(p-C);

  if (q[2] < 0.0f)
    return false;

  pp[0] = q[0] / q[2];
  pp[1] = q[1] / q[2];
  pp[2] = 1.0f / q[2];

  return true;

}

V3 PPC::GetVD() {

  return (a^b).UnitVector();

}

void PPC::TranslateFB(float step) {

  V3 vd = GetVD();
  C = C + vd*step;

}


float PPC::Getf() {

  return GetVD()*c;

}

void PPC::RotateAboutAxis(V3 aO, V3 aD, float theta) {

  a = a.RotateThisVectorAboutAxis(aD, theta);
  b = b.RotateThisVectorAboutAxis(aD, theta);
  c = c.RotateThisVectorAboutAxis(aD, theta);
  C = C.RotateThisPointAboutAxis(aO, aD, theta);

}

V3 PPC::GetPoint(int u, int v, float z) {


  return C + (a*u + b*v + c)*z/Getf();

}

V3 PPC::Unproject(V3 projP) {

  return C + (a*projP[0]+b*projP[1]+c)*(1.0f / projP[2]);

}

void PPC::SetViewGL(float zNear, float zFar) {


  // intrinsics
  glViewport(0, 0, w, h);

  float scf = zNear / Getf();
  float wf = a.Length()*(float)w;
  float hf = b.Length()*(float)h;
  float left = -wf/2.0f*scf;
  float top = hf/2.0f*scf;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(left, -left, -top, top, zNear, zFar);
  glMatrixMode(GL_MODELVIEW);

  // extrinsics

  V3 lap = C + GetVD();
  glLoadIdentity();
  gluLookAt(C[0], C[1], C[2], 
    lap[0], lap[1], lap[2], 
    -b[0], -b[1], -b[2]);

}
