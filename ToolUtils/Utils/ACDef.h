#ifndef _AC_DEF_H_
#define _AC_DEF_H_

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include <iostream>

#define USE_NS_AC using namespace acoral;
#define BEGIN_NS_AC namespace acoral{
#define END_NS_AC }

#define AC_LOG
#define AC_DEBUG

#define InfoMessageBox(strMsg)	 \
	AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION)
#define DebugMessageBox(strMsg)	 \
	AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION)
#define WarningMessageBox(strMsg)	 \
	AfxMessageBox(strMsg, MB_OK | MB_ICONWARNING)
#define ErrorMessageBox(strMsg)	 \
	AfxMessageBox(strMsg, MB_OK | MB_ICONERROR)

BEGIN_NS_AC

inline void InnerAssert(const char* _strExp, const char* _strFile, int _nLine)
{
	std::cerr << "Assert failed:" << _strExp << " in "<< _strFile << " : "<<_nLine<<std::endl;
	*reinterpret_cast<int*>(0) = 0;
}

END_NS_AC

#ifdef  AC_DEBUG
#define ACCHECK(S) ((S) || (acoral::InnerAssert(#S, __FILE__, __LINE__), 0));
#else
#define ACCHECK(S)
#endif

#define _safe_delete(x) if(x){delete x; x = 0;}

#endif
