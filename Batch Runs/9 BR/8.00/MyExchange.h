// MyExchange.h: interface for the CMyExchange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYEXCHANGE_H__98848800_F2AC_4577_A42F_93830142B033__INCLUDED_)
#define AFX_MYEXCHANGE_H__98848800_F2AC_4577_A42F_93830142B033__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyMap.h"

class CMyExchange  : public CMyMap
{
public:
				CMyExchange();
	virtual		~CMyExchange();
	BYTE		DropColorPacks(CRobot &robot,BYTE dropd,BYTE amt,BYTE mode);
	BYTE		PickBlack(MY_POINT p,CRobot &robot,BYTE dropd,BYTE mode);
	BYTE		PickColor(CRobot &robot,MY_PICK_ORD *po,BYTE count);
	BYTE		DropBlackPacks(CRobot &robot,int oldcol,BYTE dropd);
	BYTE		PickSecured(CRobot &robot,WORD pathi=0);
	BYTE		PickColor(CRobot &robot);
	BYTE		IsBlackFeasible();

	BYTE		m_hel_marg;

private:
	void		SaveOldTag(MY_ITEM *item);
	bool		GetOldTag(MY_ITEM *item);
	BYTE		ExchangePacks(CRobot &robot,MY_POINT pic,MY_POINT map);
	MY_TAG		m_tags[25];
};

#endif // !defined(AFX_MYEXCHANGE_H__98848800_F2AC_4577_A42F_93830142B033__INCLUDED_)
