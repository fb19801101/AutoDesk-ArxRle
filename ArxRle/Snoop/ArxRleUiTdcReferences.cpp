//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2015 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "ArxRleUiTdcReferences.h"
#include "AcadUtils/ArxRleUtils.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/****************************************************************************
**
**  ArxRleUiTdcReferences::ArxRleUiTdcReferences
**
**  **jma
**
*************************************/

ArxRleUiTdcReferences::ArxRleUiTdcReferences(const AcDbObjectIdArray& hardPointerIds,
											 const AcDbObjectIdArray& softPointerIds,
									         const AcDbObjectIdArray& hardOwnershipIds,
									         const AcDbObjectIdArray& softOwnershipIds)
:   m_hardPointerIds(hardPointerIds),
    m_softPointerIds(softPointerIds),
    m_hardOwnershipIds(hardOwnershipIds),
    m_softOwnershipIds(softOwnershipIds)
{
    //{{AFX_DATA_INIT(ArxRleUiTdcReferences)
    //}}AFX_DATA_INIT
}

/****************************************************************************
**
**  ArxRleUiTdcReferences::DoDataExchange
**
**  **jma
**
*************************************/

void
ArxRleUiTdcReferences::DoDataExchange(CDataExchange* pDX)
{
    ArxRleUiTdcDbObjectBase::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(ArxRleUiTdcReferences)
	DDX_Control(pDX, ARXRLE_ENTS_EB_ERASE_STATUS, m_txtErased);
	DDX_Control(pDX, ARXRLE_ENTS_LC_DATA, m_dataList);
	DDX_Control(pDX, ARXRLE_ENTS_TR_ENTS, m_entTree);
    //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// ArxRleUiTdcReferences message map

BEGIN_MESSAGE_MAP(ArxRleUiTdcReferences, ArxRleUiTdcDbObjectBase)
    //{{AFX_MSG_MAP(ArxRleUiTdcReferences)
	ON_NOTIFY(TVN_SELCHANGED, ARXRLE_ENTS_TR_ENTS, OnSelchangedEnts)
	ON_BN_CLICKED(ARXRLE_ENTS_BN_DATABASE, OnDatabase)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ArxRleUiTdcReferences message handlers

/****************************************************************************
**
**  ArxRleUiTdcReferences::OnInitDialog
**
**  **jma
**
*************************************/

BOOL
ArxRleUiTdcReferences::OnInitDialog() 
{
    ArxRleUiTdcDbObjectBase::OnInitDialog();

    HTREEITEM treeItem;
	CString str;

	treeItem = addOneTreeItem(_T("Hard Pointers"), NULL, TVI_ROOT);
	if (treeItem != NULL) {
		addEntriesToTree(m_hardPointerIds, treeItem);
	    m_entTree.Expand(treeItem, TVE_EXPAND);
	    m_entTree.SelectItem(treeItem);		// select first one
	}

	treeItem = addOneTreeItem(_T("Soft Pointers"), NULL, TVI_ROOT);
	if (treeItem != NULL) {
		addEntriesToTree(m_softPointerIds, treeItem);
	    m_entTree.Expand(treeItem, TVE_EXPAND);
	}

	treeItem = addOneTreeItem(_T("Hard Ownership"), NULL, TVI_ROOT);
	if (treeItem != NULL) {
		addEntriesToTree(m_hardOwnershipIds, treeItem);
	    m_entTree.Expand(treeItem, TVE_EXPAND);
	}

	treeItem = addOneTreeItem(_T("Soft Ownership"), NULL, TVI_ROOT);
	if (treeItem != NULL) {
		addEntriesToTree(m_softOwnershipIds, treeItem);
	    m_entTree.Expand(treeItem, TVE_EXPAND);
	}

    buildColumns(m_dataList);
    displayCurrent(0);

    //m_entTree.Expand(m_entTree.GetRootItem(), TVE_EXPAND);

    return TRUE;
}

/****************************************************************************
**
**  ArxRleUiTdcReferences::OnSelchangedEnts
**
**  **jma
**
*************************************/

void
ArxRleUiTdcReferences::OnSelchangedEnts(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    displayCurrent(static_cast<int>(pNMTreeView->itemNew.lParam));    
    *pResult = 0;
}

/****************************************************************************
**
**  ArxRleUiTdcReferences::addOneTreeItem
**
**  **jma
**
*************************************/

HTREEITEM
ArxRleUiTdcReferences::addOneTreeItem(LPCTSTR name, const AcDbObject* obj, HTREEITEM parent, bool sort)
{
	if (obj == NULL)
		m_treeObjIdList.append(AcDbObjectId::kNull);    // mark as not being an entry
	else
		m_treeObjIdList.append(obj->objectId());		// keep track of the objectId for each entry
    int index = m_treeObjIdList.length() - 1;
    ASSERT(index >= 0);

    TV_ITEM    tvItem;
    TV_INSERTSTRUCT tvInsert;

    tvItem.mask = TVIF_TEXT | TVIF_PARAM;
    tvItem.pszText = const_cast<TCHAR*>(name);
    tvItem.cchTextMax = lstrlen(name);
    tvItem.lParam = (LPARAM)index;        //index of AcDbObjectId
    
    tvInsert.item = tvItem;
    if (sort)
        tvInsert.hInsertAfter = TVI_SORT;
    else
        tvInsert.hInsertAfter = TVI_LAST;
    tvInsert.hParent = parent;

    return m_entTree.InsertItem(&tvInsert);            
}

/****************************************************************************
**
**  ArxRleUiTdcReferences::addEntriesToTree
**
**  **jma
**
*************************************/

void
ArxRleUiTdcReferences::addEntriesToTree(const AcDbObjectIdArray& ids, HTREEITEM parent)
{
	AcDbObject* tmpObj;
	HTREEITEM treeItem;
	Acad::ErrorStatus es;
	CString str;

	int len = ids.length();
	for (int i=0; i<len; i++) {
		es = acdbOpenAcDbObject(tmpObj, ids[i], AcDb::kForRead, true);		// might have passed in erased ones
		if (es == Acad::eOk) {
			treeItem = addOneTreeItem(ArxRleUtils::objToClassAndHandleStr(tmpObj, str), tmpObj, parent);
			tmpObj->close();
		}
		else
			ArxRleUtils::rxErrorMsg(es);
	}
}

/****************************************************************************
**
**  ArxRleUiTdcReferences::displayCurrent
**
**  **jma
**
*************************************/

void
ArxRleUiTdcReferences::displayCurrent(int index)
{
		// remove any previous entries
    m_fieldStrList.RemoveAll();
    m_valueStrList.RemoveAll();

    ASSERT((index >= 0) && (index < m_treeObjIdList.length()));
    m_currentObjId = m_treeObjIdList[index];

    CString str;

    AcDbObject* obj = NULL;
    Acad::ErrorStatus es = acdbOpenObject(obj, m_currentObjId, AcDb::kForRead, true);	// might want to show erased
    setExtensionButtons(obj);

    if (es == Acad::eOk) {
		display(obj);

		    // hide or show the erased entity message
		if (obj->isErased())
			m_txtErased.ShowWindow(SW_SHOW);
		else
			m_txtErased.ShowWindow(SW_HIDE);

        obj->close();
    }

	drawPropsList(m_dataList);
}
