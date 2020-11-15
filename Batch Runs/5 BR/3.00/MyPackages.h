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
	virtual ~CMyPackages();
	void	PacInit(CRobot &robot);
	BYTE	PassBlacks(CRobot &robot,BYTE mode,int pathi=0);
	BYTE	SmartPick(CRobot &robot);
	BYTE	DropColorPacks(CRobot &robot,int pathi,BYTE mode,BYTE amt);
	BYTE	DropBlackPacks(CRobot &robot,int oldcol,BYTE mode,int pathi=0);

	int			m_cti; //CMyMission::MakeDecision need access
private:
	BYTE	PickColor(CRobot &robot,MY_POINT p,BYTE c);
	BYTE	BlackPick(MY_POINT p,CRobot &robot,int pathi,BYTE mode);
	void	MakeTag(WORD tinum,BYTE mode);

	MY_TAGS		m_tag[400];//1-200 black tags others color tags
	int			m_bti;
	BYTE		m_helth;
};

#endif // !defined(AFX_MYPACKAGES_H__B8409CA5_DFD5_435E_968B_2C6406F5E856__INCLUDED_)
