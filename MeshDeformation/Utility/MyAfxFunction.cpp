#include "StdAfx.h"
#include "MyAfxFunction.h"

MyMeshPick* afxGetPickMesh()
{
	MyMeshPick* pickMesh = &(((CMeshDeformationApp*)AfxGetApp())->m_pickMesh);
	return pickMesh;
}

MyTriMesh* afxGetMainMesh()
{
	MyTriMesh* triMesh =&((CMeshDeformationApp*)AfxGetApp())->m_triMesh;
	return triMesh;
}

CMeshDeformationView* afxGetMainView()
{
	CMainFrame *pmainfrm = (CMainFrame*)AfxGetMainWnd();
	CView *pView = pmainfrm->GetActiveView();
	CMeshDeformationView*  g_deformView =(CMeshDeformationView*)pView;
	return g_deformView;
} 