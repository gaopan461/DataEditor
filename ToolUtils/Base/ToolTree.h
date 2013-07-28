#ifndef _TOOL_TREE_H_
#define _TOOL_TREE_H_

#define _CRT_SECURE_NO_WARNINGS

#include <afx.h>
#include <afxwin.h>
#include <afxcmn.h>
#include <afxdtctl.h>
#include "OptionTree.h"
#include "ToolDef.h"
#include "ToolApp.h"
#include "ACString.h"

BEGIN_NS_AC

// 下面的多继承必须将CTabCtrl放前面，否则BEGIN_MESSAGE_MAP时
// 会报警，运行时会出现指针转换导致出错

class ToolTree : public COptionTree, public Module<ToolApp>
{
public:
	ToolTree(ToolApp* app);
	virtual ~ToolTree();
public:
	BOOL Create(RECT rcRect, CWnd* pWnd, UINT nID);
public:
	int Create();
	int InsertItem(int key, const CString& strDes, std::vector<CString>& vtLayers);
public:
	void OnSelect(int key);
public:
	void ResetSelectKey();
	int GetSelectKey();
	void SelectKey(int key);
public:
	int InsertUndefinedRoot();
public:
	COptionTreeItem* FindItemByKey(int key);
protected:
	COptionTreeItem* FindOrCreateLayers(std::vector<CString>& vtLayers);
	COptionTreeItem* FindItemByInfoText(COptionTreeItem* otiRoot, const CString& strInfoText);
protected:
	static BOOL CALLBACK EnumFindItemByInfoText(COptionTree* otProp, COptionTreeItem* otiItem, LPARAM lParam);
private:
	int m_nLastSelKey;
	COptionTreeItem* m_pUndefinedRoot;
};

class COptionTreeItemStaticEx : public COptionTreeItemStatic
{
public:
	virtual void OnSelect()
	{
		ToolTree* pTree = (ToolTree*)m_otOption;
		CString key = GetInfoText();
		pTree->OnSelect(atoi(CStringToStlString(key).c_str()));
	}
};

END_NS_AC

#endif//_TOOL_TREE_H_