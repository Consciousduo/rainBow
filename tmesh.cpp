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

void TMesh::SetRectangle(V3 center, float a, float b) {

  vertsN = 4;
  verts = new V3[vertsN];
  cols = new V3[vertsN];
  normals = new V3[vertsN];

  int i = 0;
  verts[i++] = center + V3(-a/2.0f, +b/2.0f, 0.0f);
  verts[i++] = center + V3(-a/2.0f, -b/2.0f, 0.0f);
  verts[i++] = center + V3(+a/2.0f, -b/2.0f, 0.0f);
  verts[i++] = center + V3(+a/2.0f, +b/2.0f, 0.0f);

  for (int i = 0; i < 4; i++) {
    cols[i] = V3(0, 1, 0);
	normals[i] = V3(0, 0, 1);
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

void TMesh::RenderDP(V3 eye){ //do not support when camera goes into the DP
	float distance[5];
	int sequence[5];
	V3 topCenter, buttomCenter, sideOneCenter, sideTwoCenter, sideThreeCenter;
	topCenter = (verts[0]+verts[1]+verts[2])/3;
	buttomCenter = (verts[3]+verts[4]+verts[5])/3;
	sideOneCenter = (verts[6]+verts[7]+verts[8]+verts[9])/4;
	sideTwoCenter = (verts[10]+verts[11]+verts[12]+verts[13])/4;
	sideThreeCenter = (verts[14]+verts[15]+verts[16]+verts[17])/4;
	distance[0] = (eye - topCenter).Length();
	distance[1] = (eye - buttomCenter).Length();
	distance[2] = (eye - sideOneCenter).Length();
	distance[3] = (eye - sideTwoCenter).Length();
	distance[4] = (eye - sideThreeCenter).Length();
	
	int tempI = 0;
	float tempD = 0;
	for(int i=0; i<5; i++){
		if(tempD<distance[i]){
			tempI = i;
			tempD = distance[i];
		}
	}
	//find 0
	sequence[0] = tempI;
	distance[tempI] = 0;

	tempI = 0;
	tempD = 0;
	for(int i=0; i<5; i++){
		if(tempD<distance[i]){
			tempI = i;
			tempD = distance[i];
		}
	}
	//find 1
	sequence[1] = tempI;
	distance[tempI] = 0;

	tempI = 0;
	tempD = 0;
	for(int i=0; i<5; i++){
		if(tempD<distance[i]){
			tempI = i;
			tempD = distance[i];
		}
	}
	//find 2
	sequence[2] = tempI;
	distance[tempI] = 0;

	tempI = 0;
	tempD = 0;
	for(int i=0; i<5; i++){
		if(tempD<distance[i]){
			tempI = i;
			tempD = distance[i];
		}
	}
	//find 3
	sequence[3] = tempI;
	distance[tempI] = 0;

	tempI = 0;
	tempD = 0;
	for(int i=0; i<5; i++){
		if(tempD<distance[i]){
			tempI = i;
			tempD = distance[i];
		}
	}
	//find 4
	sequence[4] = tempI;
	distance[tempI] = 0;
	cout<<sequence[0]<<" "<<sequence[1]<<" "<<sequence[2]<<" "<<sequence[3]<<" "<<sequence[4]<<endl;

	for(int i=0; i<5; i++){
	switch (sequence[i])
	{
	case 0:
		cout<<"000"<<endl;
		glBegin(GL_TRIANGLES);
		glNormal3f(normals[0].xyz[0], normals[0].xyz[1], normals[0].xyz[2]);
		glVertex3f(verts[0].xyz[0], verts[0].xyz[1], verts[0].xyz[2]);
		glNormal3f(normals[1].xyz[0], normals[1].xyz[1], normals[1].xyz[2]);
        glVertex3f(verts[1].xyz[0], verts[1].xyz[1], verts[1].xyz[2]);
		glNormal3f(normals[2].xyz[0], normals[2].xyz[1], normals[2].xyz[2]);
        glVertex3f(verts[2].xyz[0], verts[2].xyz[1], verts[2].xyz[2]);
		glEnd();
		break;

	case 1:
		cout<<"111"<<endl;
		glBegin(GL_TRIANGLES);
		glNormal3f(normals[3].xyz[0], normals[3].xyz[1], normals[3].xyz[2]);
		glVertex3f(verts[3].xyz[0], verts[3].xyz[1], verts[3].xyz[2]);
		glNormal3f(normals[4].xyz[0], normals[4].xyz[1], normals[4].xyz[2]);
        glVertex3f(verts[4].xyz[0], verts[4].xyz[1], verts[4].xyz[2]);
		glNormal3f(normals[5].xyz[0], normals[5].xyz[1], normals[5].xyz[2]);
        glVertex3f(verts[5].xyz[0], verts[5].xyz[1], verts[5].xyz[2]);
		glEnd();
		break;

	case 2:
		cout<<"222"<<endl;
		glBegin(GL_TRIANGLES);
		glNormal3f(normals[6].xyz[0], normals[6].xyz[1], normals[6].xyz[2]);
		glVertex3f(verts[6].xyz[0], verts[6].xyz[1], verts[6].xyz[2]);
		glNormal3f(normals[7].xyz[0], normals[7].xyz[1], normals[7].xyz[2]);
        glVertex3f(verts[7].xyz[0], verts[7].xyz[1], verts[7].xyz[2]);
		glNormal3f(normals[8].xyz[0], normals[8].xyz[1], normals[8].xyz[2]);
        glVertex3f(verts[8].xyz[0], verts[8].xyz[1], verts[8].xyz[2]);

		glNormal3f(normals[8].xyz[0], normals[8].xyz[1], normals[8].xyz[2]);
		glVertex3f(verts[8].xyz[0], verts[8].xyz[1], verts[8].xyz[2]);
		glNormal3f(normals[9].xyz[0], normals[9].xyz[1], normals[9].xyz[2]);
        glVertex3f(verts[9].xyz[0], verts[9].xyz[1], verts[9].xyz[2]);
		glNormal3f(normals[6].xyz[0], normals[6].xyz[1], normals[6].xyz[2]);
        glVertex3f(verts[6].xyz[0], verts[6].xyz[1], verts[6].xyz[2]);
		glEnd();
		break;
	
	case 3:
		cout<<"333"<<endl;
		glBegin(GL_TRIANGLES);
		glNormal3f(normals[10].xyz[0], normals[10].xyz[1], normals[10].xyz[2]);
		glVertex3f(verts[10].xyz[0], verts[10].xyz[1], verts[10].xyz[2]);
		glNormal3f(normals[11].xyz[0], normals[11].xyz[1], normals[11].xyz[2]);
        glVertex3f(verts[11].xyz[0], verts[11].xyz[1], verts[11].xyz[2]);
		glNormal3f(normals[12].xyz[0], normals[12].xyz[1], normals[12].xyz[2]);
        glVertex3f(verts[12].xyz[0], verts[12].xyz[1], verts[12].xyz[2]);

		glNormal3f(normals[12].xyz[0], normals[12].xyz[1], normals[12].xyz[2]);
		glVertex3f(verts[12].xyz[0], verts[12].xyz[1], verts[12].xyz[2]);
		glNormal3f(normals[13].xyz[0], normals[13].xyz[1], normals[13].xyz[2]);
        glVertex3f(verts[13].xyz[0], verts[13].xyz[1], verts[13].xyz[2]);
		glNormal3f(normals[10].xyz[0], normals[10].xyz[1], normals[10].xyz[2]);
        glVertex3f(verts[10].xyz[0], verts[10].xyz[1], verts[10].xyz[2]);
		glEnd();
		break;

	case 4:
		cout<<"444"<<endl;
		glBegin(GL_TRIANGLES);
		glNormal3f(normals[14].xyz[0], normals[14].xyz[1], normals[14].xyz[2]);
		glVertex3f(verts[14].xyz[0], verts[14].xyz[1], verts[14].xyz[2]);
		glNormal3f(normals[15].xyz[0], normals[15].xyz[1], normals[15].xyz[2]);
        glVertex3f(verts[15].xyz[0], verts[15].xyz[1], verts[15].xyz[2]);
		glNormal3f(normals[16].xyz[0], normals[16].xyz[1], normals[16].xyz[2]);
        glVertex3f(verts[16].xyz[0], verts[16].xyz[1], verts[16].xyz[2]);

		glNormal3f(normals[16].xyz[0], normals[16].xyz[1], normals[16].xyz[2]);
		glVertex3f(verts[16].xyz[0], verts[16].xyz[1], verts[16].xyz[2]);
		glNormal3f(normals[17].xyz[0], normals[17].xyz[1], normals[17].xyz[2]);
        glVertex3f(verts[17].xyz[0], verts[17].xyz[1], verts[17].xyz[2]);
		glNormal3f(normals[14].xyz[0], normals[14].xyz[1], normals[14].xyz[2]);
        glVertex3f(verts[14].xyz[0], verts[14].xyz[1], verts[14].xyz[2]);
		glEnd();
		break;
	}
	}
	

}

void TMesh::SetDispersivePrism(float height, float side){
  float triangleHeight = side*sqrt(3)/2;
  height = height/2;
  side = side/2;
  
  vertsN = 18;
  verts = new V3[vertsN];
  cols = new V3[vertsN];
  normals = new V3[vertsN];

  for (int i = 0; i < 18; i++) {
    cols[i] = V3(0.8f, 0.8f, 0.8f);
  }

  int i = 0;
  verts[i++] =  V3( 0,     height, 0);
  normals[i] = V3(0, 1, 0);
  verts[i++] =  V3( side,  height, triangleHeight);
  normals[i] = V3(0, 1, 0);
  verts[i++] =  V3(-side,  height, triangleHeight);//top triangle 0 1 2
  normals[i] = V3(0, 1, 0);


  verts[i++] =  V3( 0,    -height, 0);
  normals[i] = V3(0, -1, 0);
  verts[i++] =  V3( side, -height, triangleHeight);
  normals[i] = V3(0, -1, 0);
  verts[i++] =  V3(-side, -height, triangleHeight);//buttom triangle 3 4 5
  normals[i] = V3(0, -1, 0);

  verts[i++] =  V3( 0,     height, 0);
  normals[i] = V3(sqrt(3), 0, -1);
  verts[i++] =  V3( side,  height, triangleHeight);
  normals[i] = V3(sqrt(3), 0, -1);
  verts[i++] =  V3( side, -height, triangleHeight);
  normals[i] = V3(sqrt(3), 0, -1);
  verts[i++] =  V3( 0,    -height, 0);
  normals[i] = V3(sqrt(3), 0, -1);//first quad 6 7 8 9

  verts[i++] =  V3( side,  height, triangleHeight); 
  normals[i] = V3(0, 0, 1);
  verts[i++] =  V3(-side,  height, triangleHeight);
  normals[i] = V3(0, 0, 1);
  verts[i++] =  V3(-side, -height, triangleHeight);
  normals[i] = V3(0, 0, 1);
  verts[i++] =  V3( side, -height, triangleHeight);
  normals[i] = V3(0, 0, 1);//second quad 10 11 12 13

  verts[i++] =  V3(-side,  height, triangleHeight);
  normals[i] = V3(-sqrt(3), 0, -1);
  verts[i++] =  V3( 0,     height, 0);
  normals[i] = V3(-sqrt(3), 0, -1);
  verts[i++] =  V3( 0,    -height, 0);
  normals[i] = V3(-sqrt(3), 0, -1);
  verts[i++] =  V3(-side, -height, triangleHeight);
  normals[i] = V3(-sqrt(3), 0, -1);//third quad 14 15 16 17

  trisN = 8;
  tris = new unsigned int[3*trisN];
  i = 0;
  tris[i++] = 0;
  tris[i++] = 1;
  tris[i++] = 2;

  tris[i++] = 3;
  tris[i++] = 4;
  tris[i++] = 5;

  tris[i++] = 6;
  tris[i++] = 7;
  tris[i++] = 8;

  tris[i++] = 8;
  tris[i++] = 9;
  tris[i++] = 6;

  tris[i++] = 10;
  tris[i++] = 11;
  tris[i++] = 12;

  tris[i++] = 12;
  tris[i++] = 13;
  tris[i++] = 10;

  tris[i++] = 14;
  tris[i++] = 15;
  tris[i++] = 16;

  tris[i++] = 16;
  tris[i++] = 17;
  tris[i++] = 14;
}