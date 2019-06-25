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

#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "ArxRleSdStrList.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/****************************************************************************
**
**    ArxRleSdStrList::GetAtIndex
**
**    **jma
**
*************************************/

BOOL
ArxRleSdStrList::GetAtIndex(int index, CString& str)const
{
    POSITION pos = FindIndex(index);
    if (pos) {
        str = GetAt(pos);
        return(TRUE);
    }
    else
        return(FALSE);
}

/****************************************************************************
**
**    ArxRleSdStrList::FindNoCase
**        find without regard to case sesitivity.  This function was taken from
**    the MFC source and modified to call a different string compare function.
**
**    **jma
**
*************************************/

POSITION
ArxRleSdStrList::FindNoCase(LPCTSTR searchValue, POSITION startAfter) const
{
    ASSERT_VALID(this);

    CNode* pNode = (CNode*) startAfter;
    if (pNode == NULL) {
        pNode = m_pNodeHead;  // start at head
    }
    else {
        ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
        pNode = pNode->pNext;  // start after the one specified
    }

    for (; pNode != NULL; pNode = pNode->pNext)
        if (pNode->data.CompareNoCase(searchValue) == 0)
            return (POSITION) pNode;
    return NULL;
}




/****************************************************************************
**
**    ArxRleSdSortedStrList::ArxRleSdSortedStrList
**
**    **jma
**
*************************************/

ArxRleSdSortedStrList::ArxRleSdSortedStrList(BOOL caseSensitive)
:    m_caseSensitive(caseSensitive)
{
}

/****************************************************************************
**
**    ArxRleSdSortedStrList::Insert
**
**    **jma
**
*************************************/

POSITION
ArxRleSdSortedStrList::Insert(const CString& str)
{
    CString tmpStr;
    POSITION pos = GetHeadPosition();

    if (!m_caseSensitive) {
        while (pos != NULL) {
            tmpStr = GetAt(pos);
            if (tmpStr.CompareNoCase(str) > 0)
                return(InsertBefore(pos, str));
            GetNext(pos);
        }
    }
    else {
        while (pos != NULL) {
            tmpStr = GetAt(pos);
            if (tmpStr.Compare(str) > 0)
                return(InsertBefore(pos, str));
            GetNext(pos);
        }
    }
    return(AddTail(str));
}


/****************************************************************************
**
**    ArxRleSdStrListIterator::ArxRleSdStrListIterator
**
**    **jma
**
*************************************/

ArxRleSdStrListIterator::ArxRleSdStrListIterator(const CStringList& strList)
:    m_strList(strList)
{
    m_pos = m_strList.GetHeadPosition();
}


