// PossionDefDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../MeshDeformation.h"
#include "PoissonDefDlg.h"
#include "afxdialogex.h"
#include "../Utility/MyAfxFunction.h"


// PossionDefDlg 对话框

IMPLEMENT_DYNAMIC(PoissonDefDlg, CDialog)

PoissonDefDlg::PoissonDefDlg(CWnd* pParent /*=NULL*/): CDialog(PoissonDefDlg::IDD, pParent)
,m_deformation(((CMeshDeformationApp*)AfxGetApp())->m_deformation)
{
	m_poisson_def = NULL;
}

PoissonDefDlg::~PoissonDefDlg()
{
}

void PoissonDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PoissonDefDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &PoissonDefDlg::OnImportBC)
	ON_BN_CLICKED(IDC_BUTTON3, &PoissonDefDlg::OnSetFixedRegion)
	ON_BN_CLICKED(IDC_BUTTON4, &PoissonDefDlg::OnComputeLaplacianMatrix)
	ON_BN_CLICKED(IDC_BUTTON5, &PoissonDefDlg::OnPoissonDeformation)
	ON_BN_CLICKED(IDC_BUTTON6, &PoissonDefDlg::OnReset)
END_MESSAGE_MAP()


// PossionDefDlg 消息处理程序

 
void PoissonDefDlg::OnImportBC()
{
	m_poisson_def = ((PoissonDeformation*)m_deformation);
	if(m_poisson_def)
	{
		m_poisson_def->poissonCtrl.LoadHandVertex();
		AfxGetMainWnd()->Invalidate();
	}
}


void PoissonDefDlg::OnSetFixedRegion()
{
	m_poisson_def = ((PoissonDeformation*)m_deformation);
	if(m_poisson_def)
	{
		if(!(afxGetPickMesh()->GetPickSet().empty())){
			m_poisson_def->poissonCtrl.m_fixedVindex = afxGetPickMesh()->GetPickSet();
			int m = 6;
		}
		else
			MessageBox("there is no selected vertex");

		AfxGetMainWnd()->Invalidate();
	}
}


void PoissonDefDlg::OnComputeLaplacianMatrix()
{
	m_poisson_def = ((PoissonDeformation*)m_deformation);
	if(m_poisson_def)
	{
		m_poisson_def->ComputeCoefficientMatrix();
		AfxMessageBox("laplacian matrix compute finished.");	
	}
}


void PoissonDefDlg::OnPoissonDeformation()
{
	m_poisson_def = ((PoissonDeformation*)m_deformation);
	if(m_poisson_def)
	{
		m_poisson_def->MyDeformation();
		afxGetMainView()->m_hand_angle = Vector2D(0,0);
		afxGetMainView()->m_hand_translate = Vector3D(0,0,0);
		AfxGetMainWnd()->Invalidate();
	}
}


void PoissonDefDlg::OnReset()
{
	// TODO: 在此添加控件通知处理程序代码

	m_poisson_def = ((PoissonDeformation*)m_deformation);
	if(m_poisson_def)
	{
		m_poisson_def->DeformReset();
		AfxGetMainWnd()->Invalidate();
	}
}