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

BEGIN_NS_AC

class COptionTreeItemStaticEx : public COptionTreeItemStatic
{
public:
	virtual void OnSelect()
	{
		ToolTree* pTree = (ToolTree*)m_otOption;
		int nCurrSelID = atoi(GetInfoText().GetBuffer());
		pTree->SelectID(nCurrSelID);
	}
};

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
public:
	void SelectID(int id);
private:
	int m_nLastSelID;
};

END_NS_AC

#endif//_TOOL_TREE_H_