// MyLeftPane.cpp : 实现文件
//

#include "stdafx.h"
#include "../MeshDeformation.h"
#include "MyLeftPane.h"
#include "PoissonDefDlg.h"


// MyLeftPane

IMPLEMENT_DYNAMIC(MyLeftPane, CDockablePane)

MyLeftPane::MyLeftPane()
{
	my_Dlg = NULL;
}

MyLeftPane::~MyLeftPane()
{
}


BEGIN_MESSAGE_MAP(MyLeftPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// MyLeftPane 消息处理程序

int MyLeftPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	return 0;
} 


void MyLeftPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(my_Dlg)
	{
		my_Dlg->SetWindowPos(this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		my_Dlg->ShowWindow(SW_SHOW);
	}
}


void MyLeftPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(my_Dlg)
		my_Dlg->DestroyWindow();
}

bool MyLeftPane::LoadDialog(int DLGID)
{
	bool createSuccess = false;

	if(my_Dlg)
	{
		my_Dlg->DestroyWindow();
		delete my_Dlg;
	}

	switch(DLGID)
	{
	case IDD_POISSON:
		my_Dlg = new PoissonDefDlg();
		break;
	}

	createSuccess = my_Dlg->Create(DLGID, this);
	if(!createSuccess)
	{
		MessageBox(_T("未能初始化窗口"), _T("ERROR"));
		return false;
	}

	return true;
}