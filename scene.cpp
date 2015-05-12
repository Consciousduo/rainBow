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
  
  PrismN = 1;
  DispersivePrism = new TMesh[PrismN];
  for(int i=0; i<PrismN; i++){
  DispersivePrism[i].SetDispersivePrism(30.0f, 20.0f);
  DispersivePrism[i].RotateAboutArbitraryAxis(V3(0,0,0), V3(0,1,0), 30);
  DispersivePrism[i].TranslateBackward(22*i);
  }

  tmeshesN = 1;
  tmeshes = new TMesh[tmeshesN];
  tmeshes[0].LoadBin("geometry/teapot57K.bin");
  tmeshes[0].TranslateBackward(150);
  tmeshes[0].enabled = true;

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
  //light set up
  // global ambient light
  GLfloat aGa[] = { 0.0, 0.0, 0.0, 0.0 };
  
  // light 's ambient, diffuse, specular
  float ambient, diffuse, specular;
  ambient = 0.2;
  diffuse = 0.8;
  specular = 1.0;
  GLfloat lKa0[] = { ambient, ambient, ambient, 1.0 };
  GLfloat lKd0[] = { diffuse, diffuse, diffuse, 1.0 };
  GLfloat lKs0[] = { specular, specular, specular, 1.0 };

  GLfloat lKa1[] = { ambient, ambient, ambient, 1.0 };
  GLfloat lKd1[] = { diffuse, diffuse, diffuse, 1.0 };
  GLfloat lKs1[] = { specular, specular, specular, 1.0 };

  GLfloat lKa2[] = { ambient, ambient, ambient, 1.0 };
  GLfloat lKd2[] = { diffuse, diffuse, diffuse, 1.0 };
  GLfloat lKs2[] = { specular, specular, specular, 1.0 };

  GLfloat lKa3[] = { ambient, ambient, ambient, 1.0 };
  GLfloat lKd3[] = { diffuse, diffuse, diffuse, 1.0 };
  GLfloat lKs3[] = { specular, specular, specular, 1.0 };

  GLfloat lKa4[] = { ambient, ambient, ambient, 1.0 };
  GLfloat lKd4[] = { diffuse, diffuse, diffuse, 1.0 };
  GLfloat lKs4[] = { specular, specular, specular, 1.0 };

  GLfloat lKa5[] = { ambient, ambient, ambient, 1.0 };
  GLfloat lKd5[] = { diffuse, diffuse, diffuse, 1.0 };
  GLfloat lKs5[] = { specular, specular, specular, 1.0 };

  GLfloat lKa6[] = { ambient, ambient, ambient, 1.0 };
  GLfloat lKd6[] = { diffuse, diffuse, diffuse, 1.0 };
  GLfloat lKs6[] = { specular, specular, specular, 1.0 };

  GLfloat lKa7[] = { ambient, ambient, ambient, 1.0 };
  GLfloat lKd7[] = { diffuse, diffuse, diffuse, 1.0 };
  GLfloat lKs7[] = { specular, specular, specular, 1.0 };

  /* set up lighting */
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aGa);
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  // light positions and directions
  GLfloat lP0[] = { 30.0, 30.0, 30.0, 1.0 };
  GLfloat lP1[] = { 30.0, 30.0, -30.0, 1.0 };
  GLfloat lP2[] = { -30.0, 30.0, 30.0, 1.0 };
  GLfloat lP3[] = { -30.0, 30.0, -30.0, 1.0 };
  GLfloat lP4[] = { 30.0, -30.0, 30.0, 1.0 };
  GLfloat lP5[] = { 30.0, -30.0, -30.0, 1.0 };
  GLfloat lP6[] = { -30.0, -30.0, 30.0, 1.0 };
  GLfloat lP7[] = { -30.0, -30.0, -30.0, 1.0 };
    
  // macro to set up light i
  #define LIGHTSETUP(i)\
  glLightfv(GL_LIGHT##i, GL_POSITION, lP##i);\
  glLightfv(GL_LIGHT##i, GL_AMBIENT, lKa##i);\
  glLightfv(GL_LIGHT##i, GL_DIFFUSE, lKd##i);\
  glLightfv(GL_LIGHT##i, GL_SPECULAR, lKs##i);\
  glEnable(GL_LIGHT##i)
  
  LIGHTSETUP (0);
  LIGHTSETUP (1);
  LIGHTSETUP (2);
  LIGHTSETUP (3);

  // enable 
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_LIGHTING);


  if (!cgi) {
    cgi = new CGInterface();
    cgi->PerSessionInit();
    soi = new ShaderOneInterface();
    soi->PerSessionInit(cgi);
  }

  // frame setup
  glClearColor(0.0f, 0.0f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // view setup
  float zNear = 1.0f;
  float zFar = 1000.0f;
  ppc->SetViewGL(zNear, zFar);

   for (int tmi = 0; tmi < tmeshesN; tmi++) {
    if (!tmeshes[tmi].enabled)
      continue;
		tmeshes[tmi].RenderHW();
  }


   


  if (cgi) {
    cgi->EnableProfiles();
    soi->PerFrameInit();
    soi->BindPrograms();
  }

 //render using shader
   for(int i=(PrismN-1); i>=0; i--)
   DispersivePrism[i].RenderDP(ppc->C);
 

 

  if (cgi) {
    soi->PerFrameDisable();
    cgi->DisableProfiles();
  }

}

// function linked to the DBG GUI button for testing new features
void Scene::DBG() {

  cerr << "INFO: pressed DBG button" << endl;

}

