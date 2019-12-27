#ifndef MY_AFXFUN_H
#define MY_AFXFUN_H

#include "baseHeadFile.h"
#include "../MeshPick/MyMeshPick.h"
#include "../MeshDeformation.h"
#include "../UI/MeshDeformationView.h"
#include "../UI/MainFrm.h"

MyMeshPick* afxGetPickMesh();
MyTriMesh* afxGetMainMesh();
CMeshDeformationView* afxGetMainView();

#endif; 