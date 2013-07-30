#pragma once
#include <deque>

#include "ToolDef.h"
#include "ACLog.h"
#include "ToolApp.h"

BEGIN_NS_AC

// ToolLog

// ����Ķ�̳б��뽫CTabCtrl��ǰ�棬����BEGIN_MESSAGE_MAPʱ
// �ᱨ��������ʱ�����ָ��ת�����³���

class ToolLog : public CListBox, public Module<ToolApp>
{
	DECLARE_DYNAMIC(ToolLog)

public:
	ToolLog(ToolApp* app);
	virtual ~ToolLog();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void Create(const CString& strAppName);
public:
	int Update();
public:
	void AddLog(const CString& str);
private:
	std::deque<CString> m_vtLogCache;	//��־������
};

//-----------------------------------------------------------

/*
*	MFC��־��ʩ
*/
class LogFacilityMFC : public LogFacilityBase
{
public:
	LogFacilityMFC(){}
	LOG_INTERFACE ~LogFacilityMFC(){}
public:
	LOG_INTERFACE void Output(const std::string& str);
};

END_NS_AC
