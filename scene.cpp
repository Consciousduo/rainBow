#include "scene.h"
#include <float.h>
#include <iostream>
#include <fstream>

using namespace std;

Scene *scene;

Scene::Scene() {

  soi = 0;
  cgi = 0;

  // create user interface
  gui = new GUI();
  gui->show();

  // create framebuffer
  int u0 = 30;
  int v0 = 30;
  int sci = 3;
  int w = sci*320;
  int h = sci*240;
  hwfb = new FrameBuffer(u0, v0, w, h);
  hwfb->label("HW");
  hwfb->show();
  // position UI window
  gui->uiw->position(hwfb->w+u0*2, v0);
  
  tmeshesN = 1;
  tmeshes = new TMesh[tmeshesN];
  tmeshes[0].LoadBin("geometry/teapot57K.bin");
  tmeshes[0].enabled=true;
  
  // create camera
  float hfov = 45.0f;
  ppc = new PPC(hfov, hwfb->w, hwfb->h);
  ppc->C = V3(0.0f, 0.0f, 180.0f);

  Render();
  return;

}

void Scene::Render() {

  if (hwfb)
    hwfb->redraw();

}

void Scene::RenderHW() {

  glEnable(GL_DEPTH_TEST);

  if (!cgi) {
    cgi = new CGInterface();
    cgi->PerSessionInit();
    soi = new ShaderOneInterface();
    soi->PerSessionInit(cgi);
  }

  // frame setup
  glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // view setup
  float zNear = 1.0f;
  float zFar = 1000.0f;
  ppc->SetViewGL(zNear, zFar);

  if (cgi) {
    cgi->EnableProfiles();
    soi->PerFrameInit();
    soi->BindPrograms();
  }

  for (int tmi = 0; tmi < tmeshesN; tmi++) {
    if (!tmeshes[tmi].enabled)
      continue;
    tmeshes[tmi].RenderHW();
  }

  if (cgi) {
    soi->PerFrameDisable();
    cgi->DisableProfiles();
  }

}

// function linked to the DBG GUI button for testing new features
void Scene::DBG() {

  cerr << "INFO: pressed DBG button" << endl;

}

