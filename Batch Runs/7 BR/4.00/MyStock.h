// MyStock.h: interface for the CMyStock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSTOCK_H__64D37F76_E9A8_41C7_99B4_92EF9EEB88F9__INCLUDED_)
#define AFX_MYSTOCK_H__64D37F76_E9A8_41C7_99B4_92EF9EEB88F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGlobal.h"

class CMyStock  : public CMyGlobal
{
public:
	WORD		GetScore();
				CMyStock();
	virtual		~CMyStock();
	void		AddToStock(BYTE mode,BYTE h,BYTE type,WORD ntag,char* stag);
	void		RemoveFromStock(BYTE mode,WORD ntag);
	void		StockRelease(MY_ITEM* ditem,BYTE mode);
	WORD		InitStore(CRobot &robot);
	bool		IsAllCollected();
	BYTE		GetCollected();
	BYTE		GetColorCount();
	bool		IsBlackOver();
	bool		IsColorOver();
	void		MakeTag(char *stag,WORD* ntag);

private:
	void ReleaseBlack(MY_ITEM *ditem);
	void ReleaseLowestPoit(MY_ITEM* ditem);
	void ReleaseMaxHighest(MY_ITEM* ditem);

	MY_LUGGAGE	m_stock_bk,m_stock_cl;
	WORD		m_storage,m_rem_pack;
	WORD		m_next_tag;
};

#endif // !defined(AFX_MYSTOCK_H__64D37F76_E9A8_41C7_99B4_92EF9EEB88F9__INCLUDED_)
