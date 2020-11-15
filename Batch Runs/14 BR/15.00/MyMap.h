// MyMap.h: interface for the CMyMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMAP_H__2BDC3D5C_0F87_48CF_9AA4_9E1D30246A1E__INCLUDED_)
#define AFX_MYMAP_H__2BDC3D5C_0F87_48CF_9AA4_9E1D30246A1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyDirection.h"

class CMyMap : public CMyDirection
{
public:
				CMyMap();
	virtual		~CMyMap();
	BYTE		GetFreeStepsEx(MY_POINT robo,BYTE d,bool pickable);
	void		FreeLocation(MY_POINT *map);
	//BYTE		GetDirtyRage();

	bool		IsPicTaken(MY_POINT robo,BYTE d);
	bool		IsBeenThere(MY_POINT robo,BYTE d);

	bool		FindFreshTele(MY_POINT p,BYTE d,MY_POINT *tele=0);
	bool		GetFeasible(MY_POINT p,BYTE *d);
	bool		GetUnknown(MY_POINT p,BYTE *d);

	MY_LOCATION m_blacklist[BKLST_MAX];
};

#endif // !defined(AFX_MYMAP_H__2BDC3D5C_0F87_48CF_9AA4_9E1D30246A1E__INCLUDED_)
