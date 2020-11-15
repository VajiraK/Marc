// MyBatAgent.h: interface for the CMyBatAgent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBATAGENT_H__51E25D5C_8703_408C_9C09_23F3E21A4FB1__INCLUDED_)
#define AFX_MYBATAGENT_H__51E25D5C_8703_408C_9C09_23F3E21A4FB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyStock.h"

class CMyBatAgent  
{
public:
				CMyBatAgent();
	virtual		~CMyBatAgent();
	void		TelePort(MY_POINT *p);
	void		PickSecured();
	void		SetFacing(BYTE d);
	void		Initialize(CMyStock *pstcls,MY_POINT p,BYTE d,BYTE stock);
	WORD		Move(BYTE st);


	BYTE		m_packh;
	BYTE		m_facing;
	DWORD		m_bat;
	BYTE		m_count;
private:
	CMyStock	*m_stock;
	MY_POINT	m_current;
	MY_POINT	m_picked[ST_MAX];
	WORD		m_nst;
	DWORD		m_stbat;
};

#endif // !defined(AFX_MYBATAGENT_H__51E25D5C_8703_408C_9C09_23F3E21A4FB1__INCLUDED_)
