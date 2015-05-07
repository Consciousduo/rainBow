#include "framebuffer.h"
#include <iostream>
#include "scene.h"
#include <math.h>

using namespace std;

// makes an OpenGL window that supports SW, HW rendering, that can be displayed on screen
//        and that receives UI events, i.e. keyboard, mouse, etc.
FrameBuffer::FrameBuffer(int u0, int v0, int _w, int _h) : 
  Fl_Gl_Window(u0, v0, _w, _h, 0){

  w = _w;
  h = _h;

}

// rendering callback; see header file comment
void FrameBuffer::draw() {

  scene->RenderHW();

}


// function called automatically on event within window (callback)
int FrameBuffer::handle(int event)  {  

  switch(event) 
  {   
  case FL_KEYBOARD: {
    KeyboardHandle();
    return 0;
  }
  default:
    break;
  }
  return 0;

}


void FrameBuffer::KeyboardHandle() {

  PPC *ppc = scene->ppc;
  float rstep = 1.0f; // rotation step for camera; should be adjustable;
  float tstep = 1.0f; // translation step for camera; should be adjustable;
  int key = Fl::event_key();
  switch (key) {
    case FL_Left: {
      ppc->RotateAboutAxis(ppc->C, (ppc->b*-1.0f).UnitVector(), rstep);
      break;
    }
    case FL_Right: {
      ppc->RotateAboutAxis(ppc->C, (ppc->b).UnitVector(), rstep);
      break;
    }
    case 'q': {
      ppc->RotateAboutAxis(ppc->C, (ppc->a).UnitVector(), rstep);
      break;
    }
    case 'w': {
      ppc->RotateAboutAxis(ppc->C, (ppc->a*-1.0f).UnitVector(), rstep);
      break;
    }
    case 'l': {
      ppc->RotateAboutAxis(ppc->C, (ppc->GetVD()).UnitVector(), rstep);
      break;
    }
    case 'k': {
      ppc->RotateAboutAxis(ppc->C, (ppc->GetVD()*-1.0f).UnitVector(), rstep);
      break;
    }
    case FL_Up: {
      ppc->C = ppc->C + ppc->GetVD() * tstep;
      break;
    }
    case FL_Down: {
      ppc->C = ppc->C + ppc->GetVD() * -tstep;
      break;
    }
    case '1': {
      ppc->C = ppc->C + (ppc->b*-1.0f) * tstep;
      break;
    }
    case '2': {
      ppc->C = ppc->C + ppc->b * tstep;
      break;
    }
    case ',': {
      ppc->C = ppc->C + (ppc->a*-1.0f) * tstep;
      break;
    }
    case '.': {
      ppc->C = ppc->C + ppc->a * tstep;
      break;
    }
    default:
    cerr << "INFO: do not understand keypress" << endl;
  }
  scene->Render(); // render frame for new camera
}
