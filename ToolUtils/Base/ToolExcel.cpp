#include "ToolExcel.h"

BEGIN_NS_AC

ExcelWorkbook::ExcelWorkbook(CString strPath,CWorkbook workbook)
: m_strPath(strPath)
, m_objWorkbook(workbook)
{}

ExcelWorkbook::~ExcelWorkbook()
{
	m_objWorkbook.SaveCopyAs(COleVariant(m_strPath));
	m_objWorkbook.put_Saved(true);
	m_objWorkbook.Close(COleVariant((short)true),COleVariant(m_strPath),COleVariant((short)false));
	m_objWorkbook.DetachDispatch();
}

ToolExcel::ToolExcel(ToolApp* app)
: Module<ToolApp>(app)
{}

ToolExcel::~ToolExcel()
{}

int ToolExcel::Initialize()
{
	if(!m_objApplication.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("Error! Creat excel application server failed!"));
		ExitProcess(-1);
	}

	m_objWorkbooks = m_objApplication.get_Workbooks();
	return 0;
}

int ToolExcel::Finalize()
{
	m_objWorkbooks.Close();
	m_objWorkbooks.DetachDispatch();
	m_objApplication.DetachDispatch();
	m_objApplication.Quit();
	return 0;
}

ExcelWorkbook* ToolExcel::OpenWorkbook(CString strPath)
{
	int pos = strPath.ReverseFind('\\');
	CString strFileName = strPath.Right(strPath.GetLength()-pos-1);
	ACCHECK(strFileName.Right(4) == _T(".xls") || strFileName.Right(5) == _T(".xlsx"));

	MapNameToWorkbookT::iterator iter = m_mapWorkbooks.find(strFileName);
	if(iter != m_mapWorkbooks.end())
		return &iter->second;

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
	CWorkbook workbook = m_objWorkbooks.Open(strPath,covOptional,covOptional,covOptional,
		covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,
		covOptional,covOptional,covOptional,covOptional,covOptional);

	iter = m_mapWorkbooks.insert(std::make_pair(strFileName,ExcelWorkbook(strFileName,workbook))).first;
	return &iter->second;
}

END_NS_AC