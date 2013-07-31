#pragma once

#include "ToolDef.h"

USE_NS_AC

// EffectCommonWindow 对话框

class EffectCommonWindow : public CDialog
{
	DECLARE_DYNAMIC(EffectCommonWindow)

public:
	EffectCommonWindow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EffectCommonWindow();

// 对话框数据
	enum { IDD = IDD_DIALOG_EFFECTCOMMON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void OnLoadFromDB(MapCNameToValueT& mapValues);
	void OnSaveToDB(MapCNameToValueT& mapValues);
};
