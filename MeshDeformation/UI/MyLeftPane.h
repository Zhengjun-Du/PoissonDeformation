#pragma once
// MyLeftPane

class MyLeftPane : public CDockablePane
{
	DECLARE_DYNAMIC(MyLeftPane)

public:
	MyLeftPane();
	virtual ~MyLeftPane();

public:
	CDialog*  my_Dlg;
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	bool LoadDialog(int DLGID);
};
