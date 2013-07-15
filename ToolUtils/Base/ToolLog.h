#pragma once
#include <deque>

#include "ToolDef.h"
#include "ACLog.h"
#include "ToolApp.h"

BEGIN_NS_AC

// ToolLog

// 下面的多继承必须将CTabCtrl放前面，否则BEGIN_MESSAGE_MAP时
// 会报警，运行时会出现指针转换导致出错

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
	std::deque<CString> m_vtLogCache;	//日志缓冲区
};

//-----------------------------------------------------------

/*
*	MFC日志设施
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
