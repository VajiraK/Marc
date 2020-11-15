// MyStock.h: interface for the CMyStock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSTOCK_H__64D37F76_E9A8_41C7_99B4_92EF9EEB88F9__INCLUDED_)
#define AFX_MYSTOCK_H__64D37F76_E9A8_41C7_99B4_92EF9EEB88F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyBattery.h"

class CMyStock : public CMyBattery
{
public:
				CMyStock();
	virtual		~CMyStock();

	WORD		InitStore(CRobot &robot);
	
	void		AddToStock(BYTE mode,BYTE h,BYTE type,WORD ntag,char* stag);
	void		RemoveFromStock(BYTE mode,WORD ntag);
	void		StockRelease(MY_ITEM* ditem,BYTE mode);

	void		AddToSecureList(MY_POINT p,BYTE h,BYTE bid);
	bool		IsSecured(MY_POINT p,BYTE h,BYTE *bid,bool rem_lis);

	bool		IsAllCollected();
	bool		IsBlackOver();
	bool		IsColorOver();
	BYTE		GetColorCount();
	BYTE		GetStock(BYTE mode);
	void		MakeTag(char *stag,WORD* ntag);
	#ifdef _DEBUG
	WORD		GetScore();
	#endif

private:
	void		ReleaseBlack(MY_ITEM *ditem);
	void		ReleaseLowestPoit(MY_ITEM* ditem);
	void		ReleaseMaxHighest(MY_ITEM* ditem);

	MY_LUGGAGE	m_stock_bk,m_stock_cl;
	MY_SC_LIST  m_sc_list;
	BYTE		m_color_in;
	WORD		m_pack_count,m_next_tag;
};

#endif // !defined(AFX_MYSTOCK_H__64D37F76_E9A8_41C7_99B4_92EF9EEB88F9__INCLUDED_)
