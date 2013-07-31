#pragma once

#include "ToolDef.h"

USE_NS_AC;

// MagicWindow �Ի���

class MagicWindow : public CDialog
{
	DECLARE_DYNAMIC(MagicWindow)

public:
	MagicWindow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MagicWindow();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAGIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void OnLoadFromDB(MapCNameToValueT& mapValues);
	void OnSaveToDB(MapCNameToValueT& mapValues);
};
