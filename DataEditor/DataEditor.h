// DataEditor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDataEditorApp:
// �йش����ʵ�֣������ DataEditor.cpp
//

class CDataEditorApp : public CWinApp
{
public:
	CDataEditorApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDataEditorApp theApp;