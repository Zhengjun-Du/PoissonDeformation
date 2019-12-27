#include "stdafx.h"  
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "../MeshDeformation.h"
#endif

#include "../MeshDeformationDoc.h"
#include "MeshDeformationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MainFrm.h"
#include "../Utility/predefine.h"
#include "../Utility/MyAfxFunction.h"


IMPLEMENT_DYNCREATE(CMeshDeformationView, CView)

	BEGIN_MESSAGE_MAP(CMeshDeformationView, CView)
		// 标准打印命令
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMeshDeformationView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_CREATE()
		ON_WM_DESTROY()
		ON_WM_SIZE()
		ON_WM_ERASEBKGND()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEWHEEL()
		ON_WM_MBUTTONDOWN()
		ON_WM_MBUTTONUP() 
		ON_WM_MOUSEMOVE()
		ON_COMMAND(ID_BUTTON9, &CMeshDeformationView::OnWireFrame)
		ON_COMMAND(ID_BUTTON7, &CMeshDeformationView::OnNoWireFrame)
		ON_WM_RBUTTONDOWN()
		ON_COMMAND(ID_SELECT_VERTEX, &CMeshDeformationView::OnSelectSingleVertex)
		ON_COMMAND(ID_SELECT_FRAME, &CMeshDeformationView::OnSelectFrame)
		ON_WM_KEYDOWN()
		ON_WM_KEYUP()
		ON_COMMAND(ID_POINTCLOUD, &CMeshDeformationView::OnPointcloud)
	END_MESSAGE_MAP()

	// CMeshDeformationView 构造/析构
	void CMeshDeformationView::initTransParameter()
	{
		m_hand_scale = 1;
		m_hand_angle = Vector2D(0,0);
		m_hand_translate =  Vector3D(0,0,0);
	}

	CMeshDeformationView::CMeshDeformationView()
	{
		m_isRotation = false;

		initTransParameter();

		m_startPick = false;
		m_lbuttonPushed = false;
		m_transState = MESH_TRANS_STATE;

		modelTransObj.initialize();
	}

	CMeshDeformationView::~CMeshDeformationView()
	{
	}

	BOOL CMeshDeformationView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: 在此处通过修改
		//  CREATESTRUCT cs 来修改窗口类或样式

		return CView::PreCreateWindow(cs);
	}

	// CMeshDeformationView 绘制

	void CMeshDeformationView::OnDraw(CDC* pDC)
	{
		CMeshDeformationDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: 在此处为本机数据添加绘制代码
		wglMakeCurrent(pDC->m_hDC, m_hRC);
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);	
		glLoadIdentity();

		//光照
		ShowLight();
		
		//全局变换
		glMultMatrixf(modelTransObj.GetTransformMat().M);
			
		//显示
		m_uiControl.Draw();

		//拾取
		if(m_startPick)
		{
			afxGetPickMesh()->PickMesh();
			m_startPick = false;
		}
		afxGetPickMesh()->DrawSelectedVertex();

		//获取显示相关的矩阵
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, m_projection);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);

		SwapBuffers(pDC->m_hDC);
		wglMakeCurrent(NULL, NULL);	
	}


	// CMeshDeformationView 打印


	void CMeshDeformationView::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL CMeshDeformationView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// 默认准备
		return DoPreparePrinting(pInfo);
	}

	void CMeshDeformationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: 添加额外的打印前进行的初始化过程
	}

	void CMeshDeformationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: 添加打印后进行的清理过程
	}

	void CMeshDeformationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// CMeshDeformationView 诊断

#ifdef _DEBUG
	void CMeshDeformationView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CMeshDeformationView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CMeshDeformationDoc* CMeshDeformationView::GetDocument() const // 非调试版本是内联的
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMeshDeformationDoc)));
		return (CMeshDeformationDoc*)m_pDocument;
	}
#endif //_DEBUG

	// CMeshDeformationView 消息处理程序


	int CMeshDeformationView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  在此添加您专用的创建代码

		// new code for embed OpenGL==================================================================
		// The PIXELFORMATDESCRIPTOR structure describes
		// the pixel format of a drawing surface.
		PIXELFORMATDESCRIPTOR pfd =
		{ 
			sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
			1,                     			// version number 
			PFD_DRAW_TO_WINDOW |   	// support window 
			PFD_SUPPORT_OPENGL |   	// support OpenGL 
			PFD_DOUBLEBUFFER,		// double buffered
			PFD_TYPE_RGBA,
			24,                    	// 24-bit color depth 
			0, 0, 0, 0, 0, 0,		// color bits ignored 
			0,                     	// no alpha buffer 
			0,                     	// shift bit ignored 
			0,                     	// no accumulation buffer 
			0, 0, 0, 0,            	// accum bits ignored 
			32,                    	// 32-bit z-buffer (depth)
			0,                     	// no stencil buffer 
			0,                     	// no auxiliary buffer 
			PFD_MAIN_PLANE,			// main layer 
			0,                     	// reserved 
			0, 0, 0                	// layer masks ignored 
		}; 
		CClientDC dc(this);
		// Get the best available match of pixel format for the device context
		// In other words, if this computer doesn't support features that I
		// asked for, try to get the next best thing.  i.e. 16-bit color mode
		// instead of 24-bit color mode.
		int pixelFormat = ChoosePixelFormat(dc.m_hDC, &pfd);

		// Set the pixel format to the best pixel format I can get (see above)
		// and if that operation fails, bring up a message box that tells the
		// user the error.

		if (!SetPixelFormat(dc.m_hDC, pixelFormat, &pfd))
		{
			MessageBox(_T("Error: Unable to Set Pixel Format in CGLTemplate1View::OnCreate( )"),_T("Application Error"), MB_ICONERROR);
		}

		// Creates an OpenGL rendering context so that OpenGL knows how to draw
		// to this view. You can't use OpenGL in MFC without using the handle
		// that this function returns
		m_hRC = wglCreateContext(dc.m_hDC);

		// New codes end.
		//============================================================================================

		m_pDC = new CClientDC(this);	
		CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		m_pDC->SetROP2(R2_NOT);
		m_pDC->SelectObject(pBrush);

		return 0;
	}


	void CMeshDeformationView::OnDestroy()
	{
		CView::OnDestroy();

		// TODO: 在此处添加消息处理程序代码
		// New codes begin=============================================================================

		// Set : a specified OpenGL rendering context ==> NULL
		// Set : current rendering context ==> NULL
		wglMakeCurrent(NULL, NULL);

		// Delete the handle to an OpenGL rendering context 
		wglDeleteContext(m_hRC);
		m_hRC=NULL;
		// New codes end.==============================================================================

		m_pDC->DeleteDC();
		m_pDC = 0;
	}


	void CMeshDeformationView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: 在此处添加消息处理程序代码
		// New codes begin=============================================================================

		modelTransObj.SetBounds(cx,cy);
		m_viewWidth  = cx;
		m_viewHeight = cy;
		afxGetPickMesh()->SetPickRange(m_viewWidth,m_viewHeight);

		CClientDC dc(this);
		wglMakeCurrent(dc.m_hDC, m_hRC);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		double d = 100000;
		double n = 100;
		double rato = cy/(double)cx;

		MyTriMesh* mainMesh = afxGetMainMesh();
		if(!mainMesh->empty())
			glOrtho(-mainMesh->boundBoxEdgeLen, mainMesh->boundBoxEdgeLen,-mainMesh->boundBoxEdgeLen*rato, mainMesh->boundBoxEdgeLen*rato, -d, d);
		else
			glOrtho(-cx/n, cx/n, -cy/n, cy/n, -d, d);

		glMatrixMode(GL_MODELVIEW);
		glViewport(0, 0, cx, cy);

		wglMakeCurrent(NULL, NULL);

		// New codes end.===============================================================================
	}


	BOOL CMeshDeformationView::OnEraseBkgnd(CDC* pDC)
	{
		//return CView::OnEraseBkgnd(pDC);
		return true;
	}

	void CMeshDeformationView::ShowLight(void)
	{
		GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat position[] = { 0.0, 3.0, 2.0, 0.0 };
		GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };

		//LIGHT0
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, position);

		//LIGHT1
		GLfloat position1[] = { 0.0, -3.0, 2.0, 0.0 };
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, position1);

		//Material 
		GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
		GLfloat mat_diffuse[] = { 0.1, 0.5, 0.9, 1.0 };
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat high_shininess[] = { 100.0 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		//glEnable(GL_LIGHT1);
		glShadeModel(GL_SMOOTH);	
	}

	void CMeshDeformationView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		m_startPick = true;

		afxGetPickMesh()->SetPickDownPoint(point);
		//以下防止显示有上次的拾取点，在左键抬起的时候确定拾取矩形，不然move过程中使用的上次的lbuttonup的位置
		afxGetPickMesh()->SetPickUpPoint(point);

		//画出鼠标拾取的矩形
		m_lbuttonPushed = true;
		m_lButtonDown = point;
		m_movePoint = point;	 

		CView::OnLButtonDown(nFlags, point);
	}

	void CMeshDeformationView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		m_startPick = true;
		m_lbuttonPushed = false;
		afxGetPickMesh()->SetPickUpPoint(point);

		Invalidate();
		CView::OnLButtonUp(nFlags, point);
	}

	void CMeshDeformationView::OnRButtonDown(UINT nFlags, CPoint point)
	{
		m_rButtonDown = point;

		modelTransObj.MouseEvent(RIGHT_BTN_DOWN);

		CView::OnRButtonDown(nFlags, point);
	}

	void CMeshDeformationView::OnRButtonUp(UINT  nFlags , CPoint point)
	{
		m_rButtonUp = point;

		modelTransObj.MouseEvent(RIGHT_BTN_UP);


		//操作变形控制hand
		if(MESH_TRANS_STATE != m_transState)
		{
			Vector3D m_change = ProjectScreen2World(m_rButtonUp) - ProjectScreen2World(m_rButtonDown);
			Matrix4d mat_trans = SimpleCompute::Translate2Matrix(m_change.m_x,m_change.m_y,m_change.m_z);
			m_uiControl.Update(mat_trans);
			Invalidate();
		}
	}

	void CMeshDeformationView::OnMouseMove(UINT nFlags, CPoint point)
	{
		modelTransObj.MousePt.s.X = (GLfloat)(point.x);
		modelTransObj.MousePt.s.Y = (GLfloat)(point.y);

		
		if(m_isRotation)
		{		
			if(MESH_TRANS_STATE != m_transState)
			{
				double xDis = point.x - m_prePoint.x;
				double yDis = point.y - m_prePoint.y;

				m_hand_angle = Vector2D(xDis,yDis);
			
				Matrix4d rota = Matrix4d::Identity();
				if(HAND_X_ROTATION == m_transState)
					rota =SimpleCompute::Rotation2Matrix(-m_hand_angle.m_x,0,1,0);
				else
					rota =SimpleCompute::Rotation2Matrix(-m_hand_angle.m_y,1,0,0);

				m_uiControl.Update(rota);
			}	
		}
		else if(m_lbuttonPushed)
		{
			m_pDC->Rectangle(m_lButtonDown.x,m_lButtonDown.y,m_movePoint.x,m_movePoint.y);
			m_pDC->Rectangle(m_lButtonDown.x,m_lButtonDown.y,point.x,point.y);
			m_movePoint = point;
		}

		m_prePoint = point;
		m_startPick = false;

		if(!m_lbuttonPushed)
			Invalidate();

		CView::OnMouseMove(nFlags, point);
	}

	BOOL CMeshDeformationView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{

		if(MESH_TRANS_STATE == m_transState)
		{
			if(zDelta > 0)   modelTransObj.MouseEvent(M_ZOOM_OUT);
			else   modelTransObj.MouseEvent(M_ZOOM_IN);
		}

		//CAGE/边界条件等缩放
		else
		{		
			if(zDelta>0)
				m_hand_scale = 1.2;
			else 
				m_hand_scale = 0.9; 

			Matrix4d mat_trans = SimpleCompute::Scale2Matrix(m_hand_scale,m_hand_scale,m_hand_scale);
			m_uiControl.Update(mat_trans);
		}

		Invalidate();
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	void CMeshDeformationView::OnMButtonDown(UINT nFlags, CPoint point)
	{
		modelTransObj.MouseEvent(MID_BTN_DOWN);

		m_isRotation = true;
		m_mButtonDown = point;
		CView::OnMButtonDown(nFlags, point);
	}


	void CMeshDeformationView::OnMButtonUp(UINT nFlags, CPoint point)
	{
		 modelTransObj.MouseEvent(MID_BTN_UP);

		m_isRotation = false;
		CView::OnMButtonUp(nFlags, point);
	}


	//交互操作变形（CTRL+button）
	void CMeshDeformationView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == VK_CONTROL)
			m_transState = HAND_X_ROTATION;
		else if(nChar == VK_SHIFT)
			m_transState = HAND_Y_ROTATION;

		modelTransObj.transType = m_transState;

		CView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	void CMeshDeformationView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == VK_CONTROL || nChar == VK_SHIFT)
		{
			m_transState = MESH_TRANS_STATE;
			modelTransObj.transType = m_transState;
		}
		CView::OnKeyUp(nChar, nRepCnt, nFlags);
	}

	Point3D CMeshDeformationView::ProjectScreen2World(CPoint point)
	{
		GLfloat  winX, winY; 
		GLdouble posX, posY, posZ; 

		winX = point.x;
		winY = m_viewWidth - point.y;

		gluUnProject(winX, winY, 0.0, m_modelview,m_projection,m_viewport, &posX, &posY, &posZ); 
		Point3D wordCoordinate(posX,posY,posZ);
		return wordCoordinate; 
	}

	//when a new mesh file is open
	void CMeshDeformationView::updateView()
	{
		initTransParameter();
		OnSize(0,m_viewWidth,m_viewHeight);
		
		modelTransObj.initialize();
		modelTransObj.SetBounds(m_viewWidth,m_viewHeight);

		m_transFactor = 1000/(double)afxGetMainMesh()->boundBoxEdgeLen;

		modelTransObj.translateFactor = m_transFactor;

		Invalidate();
	}