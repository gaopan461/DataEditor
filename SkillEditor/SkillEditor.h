
// SkillEditor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSkillEditorApp:
// �йش����ʵ�֣������ SkillEditor.cpp
//

class CSkillEditorApp : public CWinAppEx
{
public:
	CSkillEditorApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSkillEditorApp theApp;