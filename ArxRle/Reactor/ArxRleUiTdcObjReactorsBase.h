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

#ifndef ARXRLEUITDCOBJREACTORSBASE_H
#define ARXRLEUITDCOBJREACTORSBASE_H

#include "Resource.h"

/****************************************************************************
**
**  CLASS ArxRleUiTdcObjReactorsBase:
**
**  **jma
**
*************************************/

class ArxRleUiTdcObjReactorsBase : public CAcUiTabChildDialog {

public:
                    ArxRleUiTdcObjReactorsBase();
    virtual         ~ArxRleUiTdcObjReactorsBase()    {}

    //{{AFX_DATA(ArxRleUiTdcObjReactorsBase)
    CAcUiSelectButton	m_attachEntButton;
    CAcUiSelectButton	m_detachEntButton;
    CAcUiBitmapButton	m_detachSelectedEntsButton;
    CAcUiBitmapButton	m_detachSelectedObjsButton;
	CListBox			m_lbObjList;
	CListBox			m_lbEntList;
    CEdit				m_ebHandle;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(ArxRleUiTdcTransientReactors)
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

protected:
    // Generated message map functions
    //{{AFX_MSG(ArxRleUiTdcTransientReactors)
    afx_msg void OnObjAttach();
    afx_msg void OnEntAttach();
    afx_msg void OnEntDetach();
    afx_msg void OnEntDetachSelected();
    afx_msg void OnObjDetachSelected();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

	virtual	void	getAttachedObjects(AcDbObjectIdArray& objIds)  = 0;
	virtual void	getAttachedEntities(AcDbObjectIdArray& objIds) = 0;

	virtual void	attachObjReactors(const AcDbObjectIdArray& objIds) = 0;
	virtual void	attachEntReactors() = 0;
	virtual void	detachEntReactors() = 0;
	virtual void	detachSelectedEnts(const AcDbObjectIdArray& objIds) = 0;
	virtual void	detachSelectedObjs(const AcDbObjectIdArray& objIds) = 0;

private:
		// data members
	AcDbObjectIdArray	m_objsAttached;
	AcDbObjectIdArray	m_entsAttached;

		// helper functions
	void	displayObjList();
	void	displayEntList();
	void	setButtonModes();

public:
    static void doStateDbox(const AcDbObject* obj, LPCTSTR dboxTitle = NULL);
    static void doStateDbox(const AcDbObjectId& objId, LPCTSTR dboxTitle = NULL);
};

#endif // ARXRLEUITDCOBJREACTORSBASE_H

