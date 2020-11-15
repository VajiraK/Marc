// MyStock.h: interface for the CMyStock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSTOCK_H__64D37F76_E9A8_41C7_99B4_92EF9EEB88F9__INCLUDED_)
#define AFX_MYSTOCK_H__64D37F76_E9A8_41C7_99B4_92EF9EEB88F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGlobal.h"

class CMyStock : public CMyGlobal
{
public:
				CMyStock();
	virtual		~CMyStock();

	WORD		InitStore(CRobot &robot);
	
	void		AddToStock(BYTE mode,MY_ITEM *item);
	void		AddToSecureList(MY_POINT p,MY_ITEM *item);

	void		RemoveFromStock(BYTE mode,WORD ntag,BYTE type=0);
	void		RemoveFromSecure(WORD ntag);

	bool		IsSecured(MY_POINT p,MY_ITEM *item,bool update);
	bool		IsAllCollected();
	bool		IsBlackOver();
	bool		IsColorOver();

	BYTE		GetColorHeight();
	BYTE		GetStock(BYTE mode);
	void		MakeTag(char *stag,WORD* ntag);
	#ifdef _DEBUG
	WORD		GetScore();
	#endif

	void		ReleaseMaxHeight(MY_ITEM* ditem,BYTE mode);
	bool		ReleaseExchange(MY_CAT_ITEM *item,BYTE *count);
	void		GetBelStringTag(char* stag,WORD ntag,BYTE type);
	void		GetSecStringTag(char *stag,WORD ntag);

private:
	void		RemoveFromCatalog(WORD ntag);
	void		AddToCatalog(BYTE store,MY_ITEM *item);

	MY_ST_CATALOG	m_cat;
	MY_STOCK		m_stock;
	MY_EP			m_five,m_four,m_three,m_two,m_one;
	WORD			m_next_tag;
	BYTE			m_my_st_item,m_my_sc_point,m_my_cat_item;
};

#endif // !defined(AFX_MYSTOCK_H__64D37F76_E9A8_41C7_99B4_92EF9EEB88F9__INCLUDED_)
