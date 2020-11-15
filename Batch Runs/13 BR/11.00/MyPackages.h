// MyPackages.h: interface for the CMyPackages class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACKAGES_H__B8409CA5_DFD5_435E_968B_2C6406F5E856__INCLUDED_)
#define AFX_MYPACKAGES_H__B8409CA5_DFD5_435E_968B_2C6406F5E856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyExchange.h"

class CMyPackages : public CMyExchange
{
public:
				CMyPackages();
	virtual		~CMyPackages();
	BYTE		PassBlacks(CRobot &robot,BYTE dropd,BYTE bst,BYTE mode,bool *bfound);
	BYTE		SmartPick(CRobot &robot);

private:
	void		SortToColor(MY_PICK_ORD *arr,BYTE count);
	void		SortToHeight(MY_PICK_ORD *arr,BYTE count);
	BYTE		SendAgent();
	BYTE		m_my_pick_ord;
};

#endif // !defined(AFX_MYPACKAGES_H__B8409CA5_DFD5_435E_968B_2C6406F5E856__INCLUDED_)
