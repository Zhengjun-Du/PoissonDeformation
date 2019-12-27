// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MeshDeformationView.h : CMeshDeformationView 类的接口
//

#pragma once
#include "../Utility/baseHeadFile.h"
#include "UIControl.h"
#include "../MeshDeformationDoc.h"
#include "glut.h"
#include "MyArcBall.h"


class CMeshDeformationView : public CView
{
protected: // 仅从序列化创建
	CMeshDeformationView();
	DECLARE_DYNCREATE(CMeshDeformationView)

// 特性
public:
	CMeshDeformationDoc* GetDocument() const;

// 操作
private:
	//网格的变换
	int m_transState;
	CPoint m_prePoint;
	bool m_isRotation;
	MyArcBall modelTransObj;


public:
	MyArcBall handleTransObj;
	//显示控制
	UIControl m_uiControl;
	int m_viewWidth;
	int m_viewHeight;
	GLint    m_viewport[4]; 
	GLdouble m_modelview[16]; 
	GLdouble m_projection[16]; 
	double m_transFactor;

	//拾取
	bool m_startPick;
	CPoint m_lButtonDown;
	CPoint m_lButtonUp;
	CPoint m_rButtonDown;
	CPoint m_rButtonUp;
	CPoint m_mButtonDown;
	bool m_lbuttonPushed;
	CPoint m_movePoint;
	CClientDC *m_pDC;

	//hand的变换
	double m_hand_scale;
	Vector2D m_hand_angle;
	Vector3D m_hand_translate;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMeshDeformationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HGLRC  m_hRC;
// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);


	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);


	afx_msg void OnWireFrame(){theApp.m_triMesh.SetMeshDisMode(BOUNDARY_MESH); Invalidate();}
	afx_msg void OnNoWireFrame(){theApp.m_triMesh.SetMeshDisMode(SMOOTH_MESH); Invalidate();}
	afx_msg void OnPointcloud(){ theApp.m_triMesh.SetMeshDisMode(POINT_ONLY_MESH); Invalidate();}
	afx_msg void OnSelectSingleVertex(){ theApp.m_pickMesh.SetPickStyle(true);}
	afx_msg void OnSelectFrame(){ theApp.m_pickMesh.SetPickStyle(false);}
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int  GetTransState(){return m_transState;}


public:
	void updateView();    //when new mesh open
	void initTransParameter(); 
	void ShowLight(void); //lighting
	Point3D ProjectScreen2World(CPoint point);
	afx_msg void OnMeshHide();

	
};

#ifndef _DEBUG  // MeshDeformationView.cpp 中的调试版本
inline CMeshDeformationDoc* CMeshDeformationView::GetDocument() const
   { return reinterpret_cast<CMeshDeformationDoc*>(m_pDocument); }
#endif 