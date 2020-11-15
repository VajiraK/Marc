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
	BYTE		PickBlack(MY_POINT pic,MY_POINT map,CRobot &robot,BYTE dropd,BYTE mode);
	BYTE		PickColor(MY_POINT pic,MY_POINT map,CRobot &robot,BYTE dropd);
	BYTE		PickColor(CRobot &robot,MY_PICK_ORD *po,BYTE count);
	BYTE		DropBlackPacks(CRobot &robot,int oldcol,BYTE mode);
	BYTE		DropFreeColor(CRobot &robot,int oldcol);
	BYTE		PickSecured(CRobot &robot,bool chkloc,BYTE depth=5);
	BYTE		PickColor(CRobot &robot);
	BYTE		IsPassFeasible(bool chkdirty);
	BYTE		SendAgent();


private:
	BYTE		FillBelly(CRobot &robot,MY_POINT pic,MY_POINT map,BYTE level);
	BYTE		ExchangeBalance(CRobot &robot,MY_POINT pic,MY_POINT map);
	BYTE		ExchangeUnBalance(CRobot &robot,MY_POINT pic,MY_POINT map);
	void		SaveOldTag(MY_ITEM *item);
	bool		GetOldTag(MY_ITEM *item);
	MY_TAG		m_tags[25];
};

#endif // !defined(AFX_MYEXCHANGE_H__98848800_F2AC_4577_A42F_93830142B033__INCLUDED_)
