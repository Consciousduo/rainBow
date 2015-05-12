#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "v3.h"
#include "ppc.h"
#include "tmesh.h"
#include "CGInterface.h"

#include <iostream>

using namespace std;

class Scene {
public:


  FrameBuffer *hwfb; // HW framebuffer  
  PPC *ppc; // planar pinhole camera
  GUI * gui; // graphical user interface
  TMesh *tmeshes; // geometry
  TMesh* DispersivePrism;
  int PrismN;
  int tmeshesN;
  CGInterface *cgi;
  ShaderOneInterface *soi;

  Scene();
  void DBG();
  void Render();
  void RenderHW();
  void Render(FrameBuffer *cfb, PPC *cppc);
};

extern Scene *scene;

