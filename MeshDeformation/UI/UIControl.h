#pragma once
#include "../Utility/baseHeadFile.h"
#include "../MeshDeformation.h"
#include "../MeshDeform/Deformation.h"
#include <vector>

class UIControl
{
public:
	Deformation*& m_deformation;
public:
	UIControl(void);
	~UIControl(void);
public:
	void Draw();
	void Update(Matrix4d mat, bool localTransform = true);
}; 