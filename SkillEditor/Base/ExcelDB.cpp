#include "stdafx.h"
#include "ExcelDB.h"
#include "ToolApp.h"
#include "ToolTree.h"
#include "ACString.h"
#include "ACLog.h"
#include <algorithm>

BEGIN_NS_AC

ExcelDB::ExcelDB()
: m_pWorkbook(NULL)
{
	m_pTree = new ToolTree();
}

ExcelDB::~ExcelDB()
{
	_safe_delete(m_pTree);
}

int ExcelDB::OpenDB(const SExcelConfig& rExcelConfig)
{
	m_pWorkbook = ToolApp::Instance().GetExcelServer()->OpenWorkbook(rExcelConfig.m_strExcelCName,rExcelConfig.m_strExcelPath);
	if(m_pWorkbook == NULL)
		return -1;

	m_pTree->Create();
	m_pTree->ShowWindow(SW_HIDE);

	m_strFilePath = rExcelConfig.m_strExcelPath;
	m_strKeyCName = rExcelConfig.m_strKeyCName;
	m_strDesCName = rExcelConfig.m_strDesCName;
	m_vtLayerCName = rExcelConfig.m_vtLayerCName;
	m_nHeadRow = rExcelConfig.m_nHeadRow-1;
	m_nTypeRow = rExcelConfig.m_nTypeRow-1;
	m_nDataRow = rExcelConfig.m_nDataRow-1;

	m_mapCNameToColumn.clear();
	InitMapNameToColumn();

	m_vtDataTypes.clear();
	InitVectorDataTypes();

	SortDB();

	m_vtTreeItemInfos.clear();
	InitTreeItemInfos();

	DBToTree(m_pTree);

	return 0;
}

int ExcelDB::InitMapNameToColumn()
{
	ACCHECK(m_pWorkbook->GetSheetCount() > 0);

	int nFirstSheet = 0;
	if(m_pWorkbook->GetUsedRowCount(nFirstSheet) < m_nHeadRow)
	{
		ERROR_MSG("Require head,excel:%s",CStringToStlString(m_strFilePath).c_str());
		return -1;
	}

	std::vector<CString> vtValues;
	m_pWorkbook->GetRowText(nFirstSheet,m_nHeadRow,vtValues);

	for(size_t nCol = 0; nCol < vtValues.size(); ++nCol)
	{
		CString strValue = vtValues[nCol];
		strValue.Trim();
		if(strValue.IsEmpty())
			continue;

		m_mapCNameToColumn.insert(std::make_pair(vtValues[nCol],nCol));
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

	int nSheetTotal = m_pWorkbook->GetSheetCount();
	for(int nSheet = 0; nSheet < nSheetTotal; ++nSheet)
	{
		int nRowTotal = m_pWorkbook->GetUsedRowCount(nSheet);
		for(int nRow = m_nDataRow; nRow < nRowTotal; ++nRow)
		{
			STreeItemInfo info;
			CString strKey = m_pWorkbook->GetCellText(nSheet,nRow,nKeyCol);
			info.m_nKey = atoi(CStringToStlString(strKey).c_str());
			info.m_strDes = m_pWorkbook->GetCellText(nSheet,nRow,nDesCol);
			info.m_nSheet = nSheet;

			for(size_t i = 0; i < vtLayerCol.size(); ++i)
			{
				CString strLayerValue = m_pWorkbook->GetCellText(nSheet,nRow,vtLayerCol[i]);
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

	std::vector<CString> vtValues;
	m_pWorkbook->GetRowText(nSheet,nRow,vtValues);

	for(MapCNameToValueT::iterator iter = mapValues.begin(); iter != mapValues.end(); ++iter)
	{
		int nCol = m_mapCNameToColumn[iter->first];
		vtValues[nCol] = iter->second;
	}

	COleSafeArray sa;
	MakeCOleSafeArray(sa,vtValues,m_vtDataTypes);

	m_pWorkbook->SetRowText(nSheet,nRow,sa);

	m_pWorkbook->SaveWorkbook();

	// 更新tree
	STreeItemInfo infoNew = GetTreeItemInfo(nSheet,nRow);
	ACCHECK(infoNew.m_nKey == rTreeItemInfo.m_nKey);

	if(infoNew.m_vtLayers != rTreeItemInfo.m_vtLayers)
	{
		m_pTree->DeleteItemByKey(infoNew.m_nKey);
		m_pTree->UpdateOrInsertItemByKey(infoNew.m_nKey,infoNew.m_strDes,infoNew.m_vtLayers);
		m_pTree->SelectKey(key);
		m_pTree->UpdatedItems();
	}
	else if(infoNew.m_strDes != rTreeItemInfo.m_strDes)
	{
		m_pTree->UpdateOrInsertItemByKey(infoNew.m_nKey,infoNew.m_strDes,infoNew.m_vtLayers);
		m_pTree->UpdatedItems();
	}

	rTreeItemInfo = infoNew;
	return 0;
}

void ExcelDB::SetDBDefaultValue(MapCNameToValueT& mapDefault)
{
	m_mapDefaultValue = mapDefault;
}

void ExcelDB::GetDBDefaultValue(MapCNameToValueT& mapDefault)
{
	mapDefault = m_mapDefaultValue;
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

	std::vector<CString> vtValues;
	m_pWorkbook->GetRowText(nSheet,nRow,vtValues);

	for(MapCNameToColumnT::iterator iter = m_mapCNameToColumn.begin(); iter != m_mapCNameToColumn.end(); ++iter)
	{
		mapValues.insert(std::make_pair(iter->first,vtValues[iter->second]));
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

	pTree->UpdatedItems();
	return 0;
}

int ExcelDB::SortDB()
{
	int nKeyCol = m_mapCNameToColumn[m_strKeyCName];
	m_pWorkbook->SortAllSheetByColumn(nKeyCol,m_nDataRow);
	return 0;
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

	MapCNameToValueT mapDefault = m_mapDefaultValue;

	for(MapCNameToValueT::iterator iter = mapValues.begin(); iter != mapValues.end(); ++iter)
	{
		MapCNameToValueT::iterator iterDefault = mapDefault.find(iter->first);
		if(iterDefault != mapDefault.end())
		{
			iterDefault->second = iter->second;
		}
	}

	int nColTotal = m_pWorkbook->GetUsedColumnCount(nSheet);

	std::vector<CString> vtValues;
	for(int nCol = 0; nCol < nColTotal; ++nCol)
	{
		vtValues.push_back(_T(""));
	}

	for(MapCNameToValueT::iterator iter = mapDefault.begin(); iter != mapDefault.end(); ++iter)
	{
		MapCNameToColumnT::iterator iterCol = m_mapCNameToColumn.find(iter->first);
		if(iterCol == m_mapCNameToColumn.end())
			continue;

		vtValues[iterCol->second] = iter->second;
	}

	COleSafeArray sa;
	MakeCOleSafeArray(sa,vtValues,m_vtDataTypes);

	m_pWorkbook->InsertRow(nSheet,nRow,sa);

	m_pWorkbook->SaveWorkbook();

	STreeItemInfo infoNew = GetTreeItemInfo(nSheet,nRow);
	m_vtTreeItemInfos.insert(findResult.second,infoNew);

	// 更新tree
	m_pTree->UpdateOrInsertItemByKey(infoNew.m_nKey,infoNew.m_strDes,infoNew.m_vtLayers);
	m_pTree->SelectKey(key);
	m_pTree->UpdatedItems();

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
	CString strKey = m_pWorkbook->GetCellText(nSheet,nRow,nKeyCol);
	info.m_nKey = atoi(CStringToStlString(strKey).c_str());
	info.m_strDes = m_pWorkbook->GetCellText(nSheet,nRow,nDesCol);
	info.m_nSheet = nSheet;

	for(size_t i = 0; i < vtLayerCol.size(); ++i)
	{
		CString strLayerValue = m_pWorkbook->GetCellText(nSheet,nRow,vtLayerCol[i]);
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
	m_pWorkbook->DeleteRow(rTreeItemInfo.m_nSheet,nRow);

	m_pWorkbook->SaveWorkbook();

	int nNextKey = -1;
	VectorTreeItemInfoT::iterator iterNext = m_vtTreeItemInfos.erase(findResult.second);
	if(iterNext != m_vtTreeItemInfos.end())
		nNextKey = iterNext->m_nKey;

	// 更新tree
	m_pTree->DeleteItemByKey(key);
	m_pTree->SelectKey(nNextKey);
	m_pTree->UpdatedItems();

	return nNextKey;
}

int ExcelDB::GetUnusedKey()
{
	if(m_vtTreeItemInfos.empty())
		return 1;

	return m_vtTreeItemInfos.back().m_nKey + 1;
}

int ExcelDB::InitVectorDataTypes()
{
	ACCHECK(m_pWorkbook->GetSheetCount() > 0);

	int nFirstSheet = 0;
	if(m_pWorkbook->GetUsedRowCount(nFirstSheet) < m_nTypeRow)
	{
		ERROR_MSG("Require type,excel:%s",CStringToStlString(m_strFilePath).c_str());
		return -1;
	}

	std::vector<CString> vtValues;
	m_pWorkbook->GetRowText(nFirstSheet,m_nTypeRow,vtValues);

	for(size_t nCol = 0; nCol < vtValues.size(); ++nCol)
	{
		CString strValue = vtValues[nCol];
		strValue.Trim();
		strValue.MakeLower();
		if(strValue == _T("int") || strValue == _T("long"))
			m_vtDataTypes.push_back(DATA_INT);
		else if(strValue == _T("float"))
			m_vtDataTypes.push_back(DATA_FLOAT);
		else if(strValue == _T("bool"))
			m_vtDataTypes.push_back(DATA_BOOL);
		else
			m_vtDataTypes.push_back(DATA_STRING);
	}
	return 0;
}

END_NS_AC