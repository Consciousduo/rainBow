#include "tmesh.h"
#include "framebuffer.h"
#include "ppc.h"
#include "m33.h"

#include <fstream>
#include <iostream>

void TMesh::LoadBin(char *fname) {

  ifstream ifs(fname, ios::binary);
  if (ifs.fail()) {
    cerr << "INFO: cannot open file: " << fname << endl;
    return;
  }

  ifs.read((char*)&vertsN, sizeof(int));
  char yn;
  ifs.read(&yn, 1); // always xyz
  if (yn != 'y') {
    cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
    return;
  }
  if (verts)
    delete verts;
  verts = new V3[vertsN];

  ifs.read(&yn, 1); // cols 3 floats
  if (cols)
    delete cols;
  cols = 0;
  if (yn == 'y') {
    cols = new V3[vertsN];
  }

  ifs.read(&yn, 1); // normals 3 floats
  if (normals)
    delete normals;
  normals = 0;
  if (yn == 'y') {
    normals = new V3[vertsN];
  }

  ifs.read(&yn, 1); // texture coordinates 2 floats
  float *tcs = 0; // don't have texture coordinates for now
  if (tcs)
    delete tcs;
  tcs = 0;
  if (yn == 'y') {
    tcs = new float[vertsN*2];
  }

  ifs.read((char*)verts, vertsN*3*sizeof(float)); // load verts

  if (cols) {
    ifs.read((char*)cols, vertsN*3*sizeof(float)); // load cols
  }

  if (normals)
    ifs.read((char*)normals, vertsN*3*sizeof(float)); // load normals

  if (tcs)
    ifs.read((char*)tcs, vertsN*2*sizeof(float)); // load texture coordinates

  ifs.read((char*)&trisN, sizeof(int));
  if (tris)
    delete tris;
  tris = new unsigned int[trisN*3];
  ifs.read((char*)tris, trisN*3*sizeof(unsigned int)); // read tiangles

  ifs.close();

  cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl << "      " << fname << endl;
  cerr << "      xyz " << ((cols) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

}


void TMesh::RotateAboutArbitraryAxis(V3 O, V3 a, float theta) {

  for (int vi = 0; vi < vertsN; vi++) {
    verts[vi] = verts[vi].RotateThisPointAboutAxis(O, a, theta);
    if (normals)
      normals[vi] = normals[vi].RotateThisVectorAboutAxis(a, theta);
  }

}


V3 TMesh::GetCenter() {

  AABB aabb(verts[0]);

  for (int vi = 1; vi < vertsN; vi++)
    aabb.AddPoint(verts[vi]);

  V3 ret = (aabb.corners[0] + aabb.corners[1])/2.0f;

  return ret;


}

V3 TMesh::GetCenterMass() {

  V3 ret(0.0f, 0.0f, 0.0f);

  for (int vi = 0; vi < vertsN; vi++) {
    ret = ret + verts[vi];
  }

  ret = ret / (float) vertsN;

  return ret;

}

void TMesh::SetRectangle(V3 center, float a, float b, 
  V3 *colors, FrameBuffer *cb) {

  vertsN = 4;
  verts = new V3[vertsN];
  cols = new V3[vertsN];

  int i = 0;
  verts[i++] = center + V3(-a/2.0f, +b/2.0f, 0.0f);
  verts[i++] = center + V3(-a/2.0f, -b/2.0f, 0.0f);
  verts[i++] = center + V3(+a/2.0f, -b/2.0f, 0.0f);
  verts[i++] = center + V3(+a/2.0f, +b/2.0f, 0.0f);

  for (int i = 0; i < 4; i++) {
    cols[i] = colors[i];
  }

  trisN = 2;
  tris = new unsigned int[3*trisN];

  i = 0;
  tris[i++] = 0;
  tris[i++] = 1;
  tris[i++] = 2;

  tris[i++] = 0;
  tris[i++] = 2;
  tris[i++] = 3;

  if (cb) {
    texture = cb;
    tcs = new V3[vertsN];
    tcs[0] = V3(0.0f, 0.0f, 0.0f);
    tcs[1] = V3(0.0f, 1.0f, 0.0f);
    tcs[2] = V3(1.0f, 1.0f, 0.0f);
    tcs[3] = V3(1.0f, 0.0f, 0.0f);
  }


}

void TMesh::RenderHW() {

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, (float*)verts);

  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(3, GL_FLOAT, 0, (float*)cols);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, (float*)normals);

  glDrawElements(GL_TRIANGLES, 3*trisN, 
    GL_UNSIGNED_INT, tris);

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

}