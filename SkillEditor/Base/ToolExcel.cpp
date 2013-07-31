#include "stdafx.h"
#include "ToolExcel.h"
#include "ToolApp.h"
#include "ToolTree.h"
#include "ACString.h"
#include "ACLog.h"
#include <algorithm>

BEGIN_NS_AC

const int cst_carry_num = 'Z' - 'A' + 1;

void MakeColumnName(int nCol,CString& strColName)
{
	ACCHECK(nCol > 0);

	if(nCol >= 1 && nCol <= cst_carry_num)
	{
		strColName.Insert(0,_T('A' + nCol - 1));
		return;
	}

	if(nCol % cst_carry_num == 0)
	{
		nCol = nCol - cst_carry_num;
		strColName.Insert(0,_T('Z'));
		nCol = nCol / cst_carry_num;
	}
	else
	{
		int nLow = nCol % cst_carry_num;
		strColName.Insert(0,_T('A' + nLow - 1));
		nCol = nCol / cst_carry_num;
	}

	MakeColumnName(nCol,strColName);
}

CString MakeCellName(int nRow,int nCol)
{
	ACCHECK(nRow > 0 && nCol > 0);

	CString strColName;
	MakeColumnName(nCol,strColName);
	CString strRowName;
	strRowName.Format(_T("%d"), nRow);
	return strColName + strRowName;
}

int TestMakeCellName()
{
	ACCHECK(MakeCellName(1000,1) == _T("A1000"));	// A1000
	ACCHECK(MakeCellName(1000,26) == _T("Z1000"));		// Z1000
	ACCHECK(MakeCellName(1000,27) == _T("AA1000"));		// AA1000
	ACCHECK(MakeCellName(1000,52) == _T("AZ1000"));		// AZ1000
	ACCHECK(MakeCellName(1000,53) == _T("BA1000"));		// BA1000
	ACCHECK(MakeCellName(1000,2 * 26 + 26) == _T("BZ1000")); // BZ1000
	ACCHECK(MakeCellName(1000,2 * 26 * 26 + 26 * 26 + 26) == _T("BZZ1000")); // BZZ1000
	return 0;
}

//-----------------------------------------------------------------

ExcelWorkbook::ExcelWorkbook(LPDISPATCH workbook)
{
	m_objWorkbook.AttachDispatch(workbook);
	m_objWorkSheets.AttachDispatch(m_objWorkbook.get_Worksheets());
}

ExcelWorkbook::~ExcelWorkbook()
{
	m_objWorkSheets.ReleaseDispatch();
	m_objWorkbook.ReleaseDispatch();
}

int ExcelWorkbook::GetSheetCount()
{
	return m_objWorkSheets.get_Count();
}

int ExcelWorkbook::GetUsedRowCount(int sheetidx)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_UsedRange());
	range.AttachDispatch(range.get_Rows());
	return range.get_Count();
}

int ExcelWorkbook::GetUsedColumnCount(int sheetidx)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_UsedRange());
	range.AttachDispatch(range.get_Columns());
	return range.get_Count();
}

CString ExcelWorkbook::GetCellText(int sheetidx,int row,int col)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetUsedRowCount(sheetidx));
	ACCHECK(col >= 0 && col < GetUsedColumnCount(sheetidx));
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_Cells());
	return range.get_Item(COleVariant((short)(row+1)),COleVariant((short)(col+1)));
}

void ExcelWorkbook::SetCellText(int sheetidx,int row,int col,CString val)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetUsedRowCount(sheetidx));
	ACCHECK(col >= 0 && col < GetUsedColumnCount(sheetidx));
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_Cells());
	range.put_Item(COleVariant((short)(row+1)),COleVariant((short)(col+1)),COleVariant(val));
}

void ExcelWorkbook::SortAllSheetByColumn(int sortByCol,int startRow)
{
	int nSheetTotal = GetSheetCount();
	for(int nSheet = 0; nSheet < nSheetTotal; ++nSheet)
	{
		if(GetUsedRowCount(nSheet) <= startRow)
			continue;

		ACCHECK(startRow >= 0 && startRow < GetUsedRowCount(nSheet));
		ACCHECK(sortByCol >= 0 && sortByCol < GetUsedColumnCount(nSheet));
		CWorksheet sheet;
		sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(nSheet+1))));
		CString strCell1 = MakeCellName(startRow,1);
		CString strCell2 = MakeCellName(GetUsedRowCount(nSheet),GetUsedColumnCount(nSheet));
		CRange range;
		range.AttachDispatch(sheet.get_Range(COleVariant(strCell1),COleVariant(strCell2)));
		VARIANT key;
		V_VT(&key) = VT_DISPATCH;
		V_DISPATCH(&key) = range.get_Range(COleVariant(_T("A1")),COleVariant(_T("A1")));
		range.Sort(key,xlAscending,covOptional,covOptional,xlAscending,covOptional,
			xlAscending,xlYes,covOptional,COleVariant((long)0),xlSortColumns,xlPinYin,0,0,0);
	}
}

void ExcelWorkbook::DeleteRow(int sheetidx,int row)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetUsedRowCount(sheetidx));
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_UsedRange());
	CString strCell = MakeCellName(row+1,1);
	range.AttachDispatch(range.get_Range(COleVariant(strCell),COleVariant(strCell)));
	range.AttachDispatch(range.get_EntireRow());
	range.Delete(COleVariant((long)xlShiftUp));
}

void ExcelWorkbook::GetRowText(int sheetidx,int row,std::vector<CString>& vtStr)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetUsedRowCount(sheetidx));
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_UsedRange());
	int nColTotal = GetUsedColumnCount(sheetidx);
	for(int nCol = 0; nCol < nColTotal; ++nCol)
	{
		vtStr.push_back(range.get_Item(COleVariant((short)(row+1)),COleVariant((short)(nCol))));
	}
}

void ExcelWorkbook::InsertRow(int sheetidx,int row,std::vector<CString>& vtStr)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0);
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	for(size_t nCol = 0; nCol < vtStr.size(); ++nCol)
	{
		//获取单元格
		CString strCell = MakeCellName(row+1,nCol+1);
		range.AttachDispatch(sheet.get_Range(COleVariant(strCell),covOptional));

		//当前位置插入单元格
		range.Insert(COleVariant((short)xlShiftDown),covOptional);

		//设置单元格内容
		range.AttachDispatch(sheet.get_Range(COleVariant(strCell),covOptional));
		range.put_Value2(COleVariant(vtStr[nCol]));

		//自动列宽
		range.AttachDispatch(range.get_EntireColumn());
		range.AutoFit();
	}
}

void ExcelWorkbook::InsertEmptyRow(int sheetidx,int row)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0);
	int nColTotal = GetUsedColumnCount(sheetidx);
	std::vector<CString> vtStr;
	for(int nCol = 0; nCol < nColTotal; ++nCol)
		vtStr.push_back(_T(" "));

	InsertRow(sheetidx,row,vtStr);
}

void ExcelWorkbook::AppendEmptyRow(int sheetidx)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	int nRowTotal = GetUsedRowCount(sheetidx);
	int nColTotal = GetUsedColumnCount(sheetidx);
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_Cells());

	for(int nCol = 0; nCol < nColTotal; ++nCol)
	{
		range.put_Item(COleVariant((short)(nRowTotal+1)),COleVariant((short)(nCol+1)),COleVariant(_T(" ")));
	}
}

void ExcelWorkbook::SaveWorkbook()
{
	m_objWorkbook.Save();
	m_objWorkbook.put_Saved(TRUE);
}

void ExcelWorkbook::CloseWorkbook()
{
	m_objWorkbook.put_Saved(TRUE);
	m_objWorkbook.Close(covOptional,covOptional,covOptional);
}

//--------------------------------------------------------

ExcelDB::ExcelDB(LPDISPATCH pWorkbook, SExcelConfig& rExcelConfig)
: ExcelWorkbook(pWorkbook)
{
	m_strFilePath = rExcelConfig.m_strExcelPath;
	m_strKeyCName = rExcelConfig.m_strKeyCName;
	m_strDesCName = rExcelConfig.m_strDesCName;
	m_vtLayerCName = rExcelConfig.m_vtLayerCName;
	m_nHeadRow = rExcelConfig.m_nHeadRow-1;
	m_nDataRow = rExcelConfig.m_nDataRow-1;

	m_mapCNameToColumn.clear();
	InitMapNameToColumn();

	SortDB();

	m_vtTreeItemInfos.clear();
	InitTreeItemInfos();
}

ExcelDB::~ExcelDB()
{
	m_mapCNameToColumn.clear();
}

int ExcelDB::InitMapNameToColumn()
{
	ACCHECK(GetSheetCount() > 0);

	int nFirstSheet = 0;
	if(GetUsedRowCount(nFirstSheet) < m_nHeadRow)
	{
		ERROR_MSG("Require head,excel:%s",CStringToStlString(m_strFilePath).c_str());
		return -1;
	}

	int nColTotal = GetUsedColumnCount(nFirstSheet);
	for(int nCol = 0; nCol < nColTotal; ++nCol)
	{
		CString str = GetCellText(nFirstSheet,m_nHeadRow,nCol);
		if(str.IsEmpty())
			continue;

		m_mapCNameToColumn.insert(std::make_pair(str,nCol));
	}

	return 0;
}

int ExcelDB::InitTreeItemInfos()
{
	MapCNameToColumnT::iterator iter = m_mapCNameToColumn.find(m_strKeyCName);
	if(iter == m_mapCNameToColumn.end())
	{
		ERROR_MSG("DBToTree failed,excel:%s,key:%s",CStringToStlString(m_strFilePath).c_str(),CStringToStlString(m_strKeyCName).c_str());
		return -1;
	}
	int nKeyCol = iter->second;
	int nDesCol = m_mapCNameToColumn[m_strDesCName];

	std::vector<int> vtLayerCol;
	for(size_t i = 0; i < m_vtLayerCName.size(); ++i)
	{
		vtLayerCol.push_back(m_mapCNameToColumn[m_vtLayerCName[i]]);
	}

	int nSheetTotal = GetSheetCount();
	for(int nSheet = 0; nSheet < nSheetTotal; ++nSheet)
	{
		int nRowTotal = GetUsedRowCount(nSheet);
		for(int nRow = m_nDataRow; nRow < nRowTotal; ++nRow)
		{
			STreeItemInfo info;
			CString strKey = GetCellText(nSheet,nRow,nKeyCol);
			info.m_nKey = atoi(CStringToStlString(strKey).c_str());
			info.m_strDes = GetCellText(nSheet,nRow,nDesCol);
			info.m_nSheet = nSheet;

			for(size_t i = 0; i < vtLayerCol.size(); ++i)
			{
				CString strLayerValue = GetCellText(nSheet,nRow,vtLayerCol[i]);
				if(strLayerValue.IsEmpty())
					continue;

				info.m_vtLayers.push_back(strLayerValue);
			}

			m_vtTreeItemInfos.push_back(info);
		}
	}
	return 0;
}

struct CompareTreeInfo
{
	bool operator()(const STreeItemInfo& lh,const STreeItemInfo& rh)
	{
		return lh.m_nKey < rh.m_nKey;
	}
};

PairTreeInfoFoundT ExcelDB::FindTreeInfoByKey(int key)
{
	PairTreeInfoFoundT result;
	STreeItemInfo info;
	info.m_nKey = key;
	result.second = std::lower_bound(m_vtTreeItemInfos.begin(),m_vtTreeItemInfos.end(),info,CompareTreeInfo());

	if(result.second == m_vtTreeItemInfos.end() || result.second->m_nKey != key)
		result.first = false;
	else
		result.first = true;

	return result;
}

int ExcelDB::WriteDBRecord(int key, MapCNameToValueT& mapValues)
{
	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(!findResult.first)
	{
		ERROR_MSG("WriteDBRecord,can't find key:%d",key);
		return -1;
	}

	STreeItemInfo& rTreeItemInfo = *(findResult.second);
	int nSheet = rTreeItemInfo.m_nSheet;
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;

	for(MapCNameToValueT::iterator iter = mapValues.begin(); iter != mapValues.end(); ++iter)
	{
		int nCtrlCol = m_mapCNameToColumn[iter->first];
		SetCellText(nSheet,nRow,nCtrlCol,iter->second);
	}

	SaveWorkbook();

	// 更新tree
	STreeItemInfo infoNew = GetTreeItemInfo(nSheet,nRow);
	ACCHECK(infoNew.m_nKey == rTreeItemInfo.m_nKey);

	if(infoNew.m_vtLayers != rTreeItemInfo.m_vtLayers)
	{
		ToolApp::Instance().GetMainTree()->DeleteItemByKey(infoNew.m_nKey);
		ToolApp::Instance().GetMainTree()->UpdateOrInsertItemByKey(infoNew.m_nKey,infoNew.m_strDes,infoNew.m_vtLayers);
		ToolApp::Instance().GetMainTree()->SelectKey(key);
	}
	else if(infoNew.m_strDes != rTreeItemInfo.m_strDes)
	{
		ToolApp::Instance().GetMainTree()->UpdateOrInsertItemByKey(infoNew.m_nKey,infoNew.m_strDes,infoNew.m_vtLayers);
	}

	rTreeItemInfo = infoNew;
	return 0;
}

int ExcelDB::UpdateTreeItemInfo(STreeItemInfo& rTreeItemInfo,MapCNameToValueT& mapValues,bool bForcedUpdateTree /* = false */)
{
	bool bChanged = false;
	for(MapCNameToValueT::iterator iter = mapValues.begin(); iter != mapValues.end(); ++iter)
	{
		// 更新描述信息
		if(iter->first == m_strDesCName)
		{
			if(rTreeItemInfo.m_strDes == iter->second)
				bChanged = true;

			rTreeItemInfo.m_strDes = iter->second;
		}

		// 更新层级信息
	}

	ToolApp::Instance().GetMainTree()->UpdateOrInsertItemByKey(rTreeItemInfo.m_nKey,rTreeItemInfo.m_strDes,rTreeItemInfo.m_vtLayers);
	return 0;
}

int ExcelDB::ReadDBRecord(int key, MapCNameToValueT& mapValues)
{
	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(!findResult.first)
	{
		ERROR_MSG("ReadDBRecord,can't find key:%d",key);
		return -1;
	}

	STreeItemInfo& rTreeItemInfo = *(findResult.second);
	int nSheet = rTreeItemInfo.m_nSheet;
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;

	for(MapCNameToColumnT::iterator iter = m_mapCNameToColumn.begin(); iter != m_mapCNameToColumn.end(); ++iter)
	{
		CString strDBVal = GetCellText(nSheet,nRow,iter->second);
		mapValues.insert(std::make_pair(iter->first,strDBVal));
	}

	return 0;
}

int ExcelDB::DBToTree(ToolTree* pTree)
{
	pTree->DeleteAllItems();
	pTree->InsertUndefinedRoot();

	VectorTreeItemInfoT::iterator it,ed;
	for(it = m_vtTreeItemInfos.begin(),ed = m_vtTreeItemInfos.end(); it != ed; ++it)
	{
		STreeItemInfo& rTreeItemInfo = *it;
		pTree->UpdateOrInsertItemByKey(rTreeItemInfo.m_nKey,rTreeItemInfo.m_strDes,rTreeItemInfo.m_vtLayers);
	}

	//pTree->ExpandAllItems();
	pTree->UpdatedItems();
	return 0;
}

int ExcelDB::SortDB()
{
	int nKeyCol = m_mapCNameToColumn[m_strKeyCName];
	SortAllSheetByColumn(nKeyCol,m_nDataRow);
	return 0;
}

int ExcelDB::GetKeyInExcel(int sheet,int row)
{
	int nKeyCol = m_mapCNameToColumn[m_strKeyCName];
	CString strKey = GetCellText(sheet,row,nKeyCol);
	return atoi(CStringToStlString(strKey).c_str());
}

int ExcelDB::InsertByKey(int key, MapCNameToValueT& mapValues)
{
	if(key <= 0)
		return -1;

	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(findResult.first)
		return -1;

	int nSheet = 0;
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;

	std::vector<CString> vtValues;
	int nColTotal = GetUsedRowCount(nSheet);
	vtValues.reserve(nColTotal);
	for(int nCol = 0; nCol < nColTotal; ++nCol)
	{
		vtValues.push_back(_T(" "));
	}

	for(MapCNameToValueT::iterator iter = mapValues.begin(); iter != mapValues.end(); ++iter)
	{
		MapCNameToColumnT::iterator iterCNToCol = m_mapCNameToColumn.find(iter->first);
		if(iterCNToCol == m_mapCNameToColumn.end())
			continue;

		int nCol = iterCNToCol->second;
		ACCHECK(nCol >= 0 && nCol < nColTotal);
		vtValues[nCol] = iter->second;
	}

	InsertRow(nSheet,nRow,vtValues);
	SaveWorkbook();

	STreeItemInfo infoNew = GetTreeItemInfo(nSheet,nRow);
	m_vtTreeItemInfos.insert(findResult.second,infoNew);

	// 更新tree
	ToolApp::Instance().GetMainTree()->UpdateOrInsertItemByKey(infoNew.m_nKey,infoNew.m_strDes,infoNew.m_vtLayers);
	ToolApp::Instance().GetMainTree()->SelectKey(key);

	return key;
}

STreeItemInfo ExcelDB::GetTreeItemInfo(int nSheet,int nRow)
{
	int nKeyCol = m_mapCNameToColumn[m_strKeyCName];
	int nDesCol = m_mapCNameToColumn[m_strDesCName];

	std::vector<int> vtLayerCol;
	for(size_t i = 0; i < m_vtLayerCName.size(); ++i)
	{
		vtLayerCol.push_back(m_mapCNameToColumn[m_vtLayerCName[i]]);
	}

	STreeItemInfo info;
	CString strKey = GetCellText(nSheet,nRow,nKeyCol);
	info.m_nKey = atoi(CStringToStlString(strKey).c_str());
	info.m_strDes = GetCellText(nSheet,nRow,nDesCol);
	info.m_nSheet = nSheet;

	for(size_t i = 0; i < vtLayerCol.size(); ++i)
	{
		CString strLayerValue = GetCellText(nSheet,nRow,vtLayerCol[i]);
		if(strLayerValue.IsEmpty())
			continue;

		info.m_vtLayers.push_back(strLayerValue);
	}

	return info;
}

int ExcelDB::DeleteByKey(int key)
{
	if(key <= 0)
		return -1;

	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(!findResult.first)
		return -1;

	STreeItemInfo& rTreeItemInfo = *(findResult.second);
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;
	DeleteRow(rTreeItemInfo.m_nSheet,nRow);

	SaveWorkbook();

	int nNextKey = -1;
	VectorTreeItemInfoT::iterator iterNext = m_vtTreeItemInfos.erase(findResult.second);
	if(iterNext != m_vtTreeItemInfos.end())
		nNextKey = iterNext->m_nKey;

	// 更新tree
	ToolApp::Instance().GetMainTree()->DeleteItemByKey(key);
	ToolApp::Instance().GetMainTree()->SelectKey(nNextKey);

	return nNextKey;
}

//--------------------------------------------------------

ToolExcel::ToolExcel(ToolApp* app)
: Module<ToolApp>(app)
{
	CreateExcelServer();
}

ToolExcel::~ToolExcel()
{
	DestroyExcelServer();
}

int ToolExcel::CreateExcelServer()
{
	if(!AfxOleInit())
	{
		ErrorMessageBox(_T("Error! Ole initial failed!"));
		ExitProcess(-1);
	}

	if(!m_objApplication.CreateDispatch(_T("Excel.Application")))
	{
		ErrorMessageBox(_T("Error! Creat excel application server failed!"));
		ExitProcess(-1);
	}

	m_objApplication.put_AlertBeforeOverwriting(FALSE);
	m_objApplication.put_DisplayAlerts(FALSE);

	m_objWorkbooks.AttachDispatch(m_objApplication.get_Workbooks());
	return 0;
}

int ToolExcel::DestroyExcelServer()
{
	for(MapNameToWorkbookT::iterator iter = m_mapWorkbooks.begin(); iter != m_mapWorkbooks.end(); ++iter)
	{
		ExcelDB* pBook = iter->second;
		pBook->CloseWorkbook();
		_safe_delete(pBook);
	}

	m_mapWorkbooks.clear();
	m_objWorkbooks.Close();
	m_objWorkbooks.ReleaseDispatch();
	m_objApplication.Quit();
	m_objApplication.ReleaseDispatch();
	return 0;
}

ExcelDB* ToolExcel::OpenWorkbook(SExcelConfig& rExcelConfig)
{
	MapNameToWorkbookT::iterator iter = m_mapWorkbooks.find(rExcelConfig.m_strExcelCName);
	if(iter != m_mapWorkbooks.end())
		return iter->second;

	LPDISPATCH workbook = m_objWorkbooks.Open(rExcelConfig.m_strExcelPath,covOptional,
		covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,
		covOptional,covOptional,covOptional,covOptional,covOptional,covOptional);

	ExcelDB* pExcelDB = new ExcelDB(workbook,rExcelConfig);
	m_mapWorkbooks.insert(std::make_pair(rExcelConfig.m_strExcelCName,pExcelDB));
	return pExcelDB;
}

ExcelDB* ToolExcel::GetWorkbook(CString strExcelCName)
{
	MapNameToWorkbookT::iterator iter = m_mapWorkbooks.find(strExcelCName);
	if(iter != m_mapWorkbooks.end())
		return iter->second;

	return NULL;
}

END_NS_AC