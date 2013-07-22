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

// ����Ķ�̳б��뽫CTabCtrl��ǰ�棬����BEGIN_MESSAGE_MAPʱ
// �ᱨ��������ʱ�����ָ��ת�����³���

class ToolTree : public COptionTree, public Module<ToolApp>
{
public:
	ToolTree(ToolApp* app);
	virtual ~ToolTree();
public:
	BOOL Create(RECT rcRect, CWnd* pWnd, UINT nID);
public:
	int Create();
	int InsertItem(int key, const CString& strDes);
public:
	void SelectKey(int key);
public:
	void ResetSelectKey();
	int GetSelectKey();
public:
	int InsertUndefinedRoot();
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
		pTree->SelectKey(atoi(CStringToStlString(key).c_str()));
	}
};

END_NS_AC

#endif//_TOOL_TREE_H_