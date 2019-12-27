#include "StdAfx.h"
#include "UIControl.h"
#include "../Utility/predefine.h"
#include "../Utility/MyAfxFunction.h"


UIControl::UIControl(void):m_deformation(((CMeshDeformationApp*)AfxGetApp())->m_deformation)
{
}


UIControl::~UIControl(void)
{
}


void UIControl::Draw()
{
	afxGetMainMesh()->Draw();

	if(m_deformation)
		m_deformation->Draw();
}

void UIControl::Update(Matrix4d mat, bool localTransform)
{
	int mainViewTransState = afxGetMainView()->GetTransState();
	if(m_deformation)
	{
		if(HAND_X_ROTATION == mainViewTransState)
			m_deformation->InteracTransform(mat,true);
		else if(HAND_Y_ROTATION == mainViewTransState)
			m_deformation->InteracTransform(mat,false);
	}
	
} 