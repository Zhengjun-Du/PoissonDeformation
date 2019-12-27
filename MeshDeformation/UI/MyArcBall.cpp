#include "stdafx.h"
#include "MyArcBall.h"

void MyArcBall::initialize()
{

	Matrix4fT Transform_   = 
	{ 
		1.0f,  0.0f,  0.0f,  0.0f,		// NEW: Final Transform
		0.0f,  1.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  0.0f,  1.0f 
	};RotaMat = Transform_;
	  TranslateMat = Transform_;
	  ScaleMat = Transform_;

	Matrix3fT LastRot_     = 
	{ 
		1.0f,  0.0f,  0.0f,				// NEW: Last Rotation
		0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  1.0f 
	};LastRot = LastRot_;

	Matrix3fT ThisRot_     = 
	{ 
		1.0f,  0.0f,  0.0f,				// NEW: This Rotation
		0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  1.0f 
	};ThisRot = ThisRot_;

	ArcBall = new ArcBallT(640.0f, 480.0f);		// NEW: ArcBall Instance
	MousePt.s.X = 0;			
	MousePt.s.Y = 0;
	isMClicked = false;							// NEW: Clicking The Right Mouse Button?
	isRotaDragging = false;	
	isRClicked = false;
	isTranslateDragging = false;
	transType = MESH_TRANS_STATE;
} 

void MyArcBall::SetBounds(float w, float h)
{
	if(ArcBall)
		ArcBall->setBounds(w,h);
}

void MyArcBall::Rotation()
{
	if (!isRotaDragging)												// Not Dragging
	{
		if(isMClicked)												// First Click
		{
			isRotaDragging = true;										// Prepare For Dragging
			LastRot = ThisRot;										// Set Last Static Rotation To Last Dynamic One
			ArcBall->click(&MousePt);								// Update Start Vector And Prepare For Dragging
		}
	}
	else
	{
		if(isMClicked)												// Still Clicked, So Still Dragging
		{
			Quat4fT     ThisQuat;
			ArcBall->drag(&MousePt, &ThisQuat);						// Update End Vector And Get Rotation As Quaternion
			Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);		// Convert Quaternion Into Matrix3fT
			Matrix3fMulMatrix3f(&ThisRot, &LastRot);				// Accumulate Last Rotation Into This One
			Matrix4fSetRotationFromMatrix3f(&RotaMat, &ThisRot);	// Set Our Final Transform's Rotation From This One
		}
		else														// No Longer Dragging
			isRotaDragging = false;
	}
}

void MyArcBall::Translate()
{
	if (!isTranslateDragging)												
	{
		if(isRClicked)												
		{
			isTranslateDragging = true;		
			lastPoint = MousePt;
		}
	}
	else
	{
		if(isRClicked)	
		{
			TranslateMat.s.M03 += (MousePt.s.X - lastPoint.s.X)/translateFactor;
			TranslateMat.s.M13 += -(MousePt.s.Y - lastPoint.s.Y)/translateFactor;

			lastPoint = MousePt;
		}
		else
			isTranslateDragging = false;
	}
}

void MyArcBall::Scale(int zoomx)
{
	if(M_ZOOM_OUT == zoomx)
	{
		ScaleMat.s.M00  += 0.1;
		ScaleMat.s.M11  += 0.1;	
		ScaleMat.s.M22  += 0.1;
	}

	else if(TranslateMat.s.M00  >= 0.2)
	{
		ScaleMat.s.M00  -= 0.1;
		ScaleMat.s.M11  -= 0.1;	
		ScaleMat.s.M22  -= 0.1;
	}
}

Eigen::Matrix4d  MyArcBall::GetCurrenRotaMat()
{
	Eigen::Matrix4d mat;
	Convert2EigenMat(RotaMat,mat);
	return mat;
	
}
Eigen::Matrix4d  MyArcBall::GetCurrenTranslateMat()
{
	Eigen::Matrix4d mat;
	Convert2EigenMat(TranslateMat,mat);
	return mat;
}

Matrix4fT MyArcBall::GetTransformMat()
{
	if(MESH_TRANS_STATE == transType)
	{
		Translate();
		Rotation();
	}

	Matrix4fT result = TranslateMat;
	Matrix4fMulMatrix4f(&result,&RotaMat);
	Matrix4fMulMatrix4f(&result,&ScaleMat);
	return result;
}

void MyArcBall::MouseEvent(int keyStyle)
{
	switch(keyStyle)
	{
	case MID_BTN_DOWN:
		isMClicked   = true;
		isRClicked   = false;
		break;

	case MID_BTN_UP:
		isMClicked   = false;
		isRClicked   = false;
		isRotaDragging = false;
		break;

	case RIGHT_BTN_DOWN:
		isMClicked   = false;
		isRClicked   = true;
		break;

	case RIGHT_BTN_UP:
		isMClicked   = false;
		isRClicked   = false;
		isTranslateDragging = false;
		break;

	case M_ZOOM_IN:
		Scale(M_ZOOM_IN);
		break;

	case M_ZOOM_OUT:
		Scale(M_ZOOM_OUT);
		break;
	}
}

void  MyArcBall::Convert2EigenMat(Matrix4fT& mat, Eigen::Matrix4d& EigehMat)
{
	EigehMat(0,0) = mat.s.M00; EigehMat(0,1) = mat.s.M01; EigehMat(0,2) = mat.s.M02; EigehMat(0,3) = mat.s.M03; 
	EigehMat(1,0) = mat.s.M10; EigehMat(1,1) = mat.s.M11; EigehMat(1,2) = mat.s.M12; EigehMat(1,3) = mat.s.M13; 
	EigehMat(2,0) = mat.s.M20; EigehMat(2,1) = mat.s.M21; EigehMat(2,2) = mat.s.M22; EigehMat(2,3) = mat.s.M23; 
	EigehMat(3,0) = mat.s.M30; EigehMat(3,1) = mat.s.M31; EigehMat(3,2) = mat.s.M32; EigehMat(3,3) = mat.s.M33; 
}


