#pragma once
#include "../Utility/baseHeadFile.h"
#include "math3d.h"
#include "Selection.h"

#include  <vector>
using namespace std;

class MyMeshPick
{
public:
	MyMeshPick(void);
	~MyMeshPick(void);

private:
	M3DMatrix44f m_mat_proj;
	M3DMatrix44f m_mat_modelview;
	M3DVector2f m_left_bottom;
	M3DVector2f m_right_top;
	int m_viewWidth;
	int m_viewHeight;
	int m_nVertices;
	M3DVector3f* m_desMesh;
	Selection selection;
	vector<int> m_pickedSet;
	CPoint m_mouseDownPoint;
	CPoint m_mouseUpPoint;
	bool m_singleSelect;

public:
	void SetPickRange(int w, int h){m_viewWidth = w, m_viewHeight = h;}
	void SetPickStyle(bool single){m_singleSelect = single;};
	void SetDesMesh(TriMesh* mesh);
	vector<int>& GetPickSet(){return m_pickedSet;}
	void PickMesh();
	void DrawSelectedVertex();
	void Clear();
	void Update(TriMesh* mesh);
	void SetPickDownPoint(CPoint downpoint){m_mouseDownPoint = downpoint;}
	void SetPickUpPoint(CPoint uppoint){m_mouseUpPoint = uppoint;}
}; 