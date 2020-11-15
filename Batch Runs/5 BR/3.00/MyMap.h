// MyMap.h: interface for the CMyMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMAP_H__2BDC3D5C_0F87_48CF_9AA4_9E1D30246A1E__INCLUDED_)
#define AFX_MYMAP_H__2BDC3D5C_0F87_48CF_9AA4_9E1D30246A1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyMapHelper.h"

class CMyMap : public CMyMapHelper
{
public:
	//void	WriteMap();
			CMyMap();
	virtual ~CMyMap();
	int		QuarryArea(CRobot &robot);
	WORD	GetFreeSteps();
	void	FillArea(CPicture &picture);
	void	VirMove(MY_POINT *p,BYTE d,WORD step);
private:
	int		MakeVerPath(CRobot &robot);
	BYTE	GotoLastTele(CRobot &robot);

};

#endif // !defined(AFX_MYMAP_H__2BDC3D5C_0F87_48CF_9AA4_9E1D30246A1E__INCLUDED_)
