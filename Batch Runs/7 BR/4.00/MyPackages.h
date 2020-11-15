// MyPackages.h: interface for the CMyPackages class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACKAGES_H__B8409CA5_DFD5_435E_968B_2C6406F5E856__INCLUDED_)
#define AFX_MYPACKAGES_H__B8409CA5_DFD5_435E_968B_2C6406F5E856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyDirection.h"

class CMyPackages : public CMyDirection
{
public:
				CMyPackages();
	virtual		~CMyPackages();
	void		PassColors(CRobot &robot,BYTE dropd);
	BYTE		PassBlacks(CRobot &robot,BYTE dropd,BYTE mode);
	BYTE		SmartPick(CRobot &robot);
	BYTE		DropColorPacks(CRobot &robot,BYTE dropd,BYTE amt,BYTE mode);
	BYTE		DropBlackPacks(CRobot &robot,int oldcol,BYTE dropd);

private:
	BYTE		PickColor(CRobot &robot,MY_POINT p,BYTE c);
	BYTE		PickBlack(MY_POINT p,CRobot &robot,BYTE dropd,BYTE mode);
	WORD		IsBlackFeasible();

	BYTE		m_helth;
};

#endif // !defined(AFX_MYPACKAGES_H__B8409CA5_DFD5_435E_968B_2C6406F5E856__INCLUDED_)
