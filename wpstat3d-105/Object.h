// Object.h: interface for the CObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECT_H__72C8A601_D2E3_459D_A186_52786358D524__INCLUDED_)
#define AFX_OBJECT_H__72C8A601_D2E3_459D_A186_52786358D524__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObject  
{
public:
	CObject();
	virtual ~CObject();
	
	virtual void Draw(void) = 0;
};

#endif // !defined(AFX_OBJECT_H__72C8A601_D2E3_459D_A186_52786358D524__INCLUDED_)
