#ifndef _AC_LOG_H_
#define _AC_LOG_H_

#include "ACDef.h"
#include "ACThreadGuard.h"
#include <cstdarg>
#include <fstream>
#include <vector>

#ifdef AC_LOG
#define LOG_INTERFACE virtual
#else
#define LOG_INTERFACE
#endif //AC_LOG

#define DEBUG_MSG(fmt,...) acoral::Log::Instance().AddLog(acoral::LOG_DEBUG,fmt,__VA_ARGS__)
#define ERROR_MSG(fmt,...) acoral::Log::Instance().AddLog(acoral::LOG_ERROR,fmt,__VA_ARGS__)
#define WARN_MSG(fmt,...) acoral::Log::Instance().AddLog(acoral::LOG_WARNING,fmt,__VA_ARGS__)
#define INFO_MSG(fmt,...) acoral::Log::Instance().AddLog(acoral::LOG_INFO,fmt,__VA_ARGS__)

BEGIN_NS_AC

/*
 *	��־���� 
 */
enum ELogType
{
	LOG_DEBUG,				//����
	LOG_ERROR,				//����
	LOG_WARNING,			//����
	LOG_INFO,				//��ͨ
};

//------------------------------------------------------

/*
 *	��־��ʩ���� 
 */
class LogFacilityBase
{
public:
	LOG_INTERFACE ~LogFacilityBase(){}
public:
	LOG_INTERFACE void Output(const std::string& str){}
};

class LogEx;

/*
 *	��־����
 */
class Log
{
	static void CreateInstance();
	static Log* m_pInstance;
	static long m_nSingletonGuard;
	static bool m_bSingletonDestroyed;
protected:
	static long m_nLogGuard;
public:
	static Log& Instance();
	LOG_INTERFACE ~Log()
	{
		m_bSingletonDestroyed = true;
		m_pInstance = 0;
	}

	//************************************
	// Method:    AddLog ������־
	// FullName:  acutils::Log::AddLog
	// Access:    public 
	// Returns:   LOG_INTERFACE void
	// Qualifier:
	// Parameter: int type ��־����
	// Parameter: const char * format Ҫ���������
	// Parameter: ...
	//************************************
	LOG_INTERFACE void AddLog(int type, const char* format,...){}


	//************************************
	// Method:    AddFacility ������־�����ʩ
	// FullName:  acutils::Log::AddFacility
	// Access:    public 
	// Returns:   LOG_INTERFACE void
	// Qualifier:
	// Parameter: LogFacilityBase * pFacility
	//************************************
	LOG_INTERFACE void AddFacility(LogFacilityBase* pFacility){}
};

#ifdef AC_LOG
inline Log& Log::Instance()
{
	if(!m_pInstance)
		CreateInstance();

	return *m_pInstance;
}
#else
inline Log& Log::Instance()
{
	return *reinterpret_cast<Log*>(0);
}
#endif //AC_LOG

//------------------------------------------------------

/*
 *	��չ��־
 */
class LogEx : public Log
{
public:
	LOG_INTERFACE ~LogEx();
public:
	LOG_INTERFACE void AddLog(int type, const char* format,...);
	LOG_INTERFACE void AddFacility(LogFacilityBase* pFacility);
private:
	std::vector<LogFacilityBase*> m_vtLogFacilities;
};

#ifdef AC_LOG
inline void Log::CreateInstance()
{
	ThreadGuard guard(&m_nSingletonGuard);
	if(m_pInstance)
		return;

	ACCHECK(!m_bSingletonDestroyed);
	static LogEx obj;
	m_pInstance = &obj;
}
#endif //AC_LOG

//------------------------------------------------------

/*
 *	���������̨ 
 */
class LogFacilityConsole : public LogFacilityBase
{
public:
	LOG_INTERFACE void Output(const std::string& str);
};

//------------------------------------------------------

/*
 *	������ļ� 
 */
class LogFacilityFile : public LogFacilityBase
{
public:
	LogFacilityFile(const std::string& filename);
	LOG_INTERFACE ~LogFacilityFile();
public:
	LOG_INTERFACE void Output(const std::string& str);
private:
	std::fstream m_objFileStream;
};

END_NS_AC

#endif //_AC_LOG_H_