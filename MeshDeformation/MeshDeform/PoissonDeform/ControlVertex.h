#pragma once

#include "../../Utility/baseHeadFile.h"
#include <vector>

class ControlVertex
{
public:
	ControlVertex(void){};
	~ControlVertex(void){};

public:
	TriMesh* m_mesh;
	vector<double>  m_freeVertexWeight;
	vector<Point3D> m_posOfHand;
	vector<int>     m_HandVindex;
	vector<int>     m_fixedVindex;

public:
	void LoadHandVertex();
	vector<bool>  GetVertexProperty();
	void   Draw();
	void   HandTransform(Matrix4d mt);
	void   ComputeFreeVertexWeight();
	void   UpdateHandPos();
	void   Clear();
	void   Reset();
};