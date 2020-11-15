// MyExchange.h: interface for the CMyExchange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYEXCHANGE_H__98848800_F2AC_4577_A42F_93830142B033__INCLUDED_)
#define AFX_MYEXCHANGE_H__98848800_F2AC_4577_A42F_93830142B033__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyDirection.h"

class CMyExchange  : public CMyDirection
{
public:
				CMyExchange();
	virtual		~CMyExchange();
	BYTE		DropColorPacks(CRobot &robot,BYTE dropd,BYTE amt,BYTE mode);
	BYTE		PickBlack(MY_POINT p,CRobot &robot,BYTE dropd,BYTE mode);
	BYTE		PickColor(CRobot &robot,MY_PICK_ORD *po,BYTE count);
	BYTE		DropBlackPacks(CRobot &robot,int oldcol,BYTE dropd);
	BYTE		PickSecured(CRobot &robot);
	BYTE		PickColor(CRobot &robot);
	BYTE		IsBlackFeasible();

	BYTE		m_hel_marg;
private:
	void		PaintBlack();
};

#endif // !defined(AFX_MYEXCHANGE_H__98848800_F2AC_4577_A42F_93830142B033__INCLUDED_)
