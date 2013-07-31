#pragma once

#include "ToolDef.h"

USE_NS_AC

// EffectCommonWindow �Ի���

class EffectCommonWindow : public CDialog
{
	DECLARE_DYNAMIC(EffectCommonWindow)

public:
	EffectCommonWindow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~EffectCommonWindow();

// �Ի�������
	enum { IDD = IDD_DIALOG_EFFECTCOMMON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void OnLoadFromDB(MapCNameToValueT& mapValues);
	void OnSaveToDB(MapCNameToValueT& mapValues);
};
