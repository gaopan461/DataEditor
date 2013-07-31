#pragma once

#include "ToolDef.h"

USE_NS_AC;

// MagicWindow 对话框

class MagicWindow : public CDialog
{
	DECLARE_DYNAMIC(MagicWindow)

public:
	MagicWindow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MagicWindow();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAGIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void OnLoadFromDB(MapCNameToValueT& mapValues);
	void OnSaveToDB(MapCNameToValueT& mapValues);
};
