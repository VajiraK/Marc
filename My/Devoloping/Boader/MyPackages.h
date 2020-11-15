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
	BYTE		CollectColor(CRobot &robot,BYTE mode);
	void		PassColors(CRobot &robot,BYTE dropd);
	BYTE		PassBlacks(CRobot &robot,BYTE dropd,BYTE bst,BYTE mode);
	BYTE		SmartPick(CRobot &robot);

private:
	void		SortAcending(MY_PICK_ORD *arr,BYTE count);
};

#endif // !defined(AFX_MYPACKAGES_H__B8409CA5_DFD5_435E_968B_2C6406F5E856__INCLUDED_)
