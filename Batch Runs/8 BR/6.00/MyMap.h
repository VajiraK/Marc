// MyMap.h: interface for the CMyMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMAP_H__2BDC3D5C_0F87_48CF_9AA4_9E1D30246A1E__INCLUDED_)
#define AFX_MYMAP_H__2BDC3D5C_0F87_48CF_9AA4_9E1D30246A1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPackages.h"

class CMyMap : public CMyPackages
{
public:
				CMyMap();
	virtual		~CMyMap();
	BYTE		GetFreeSteps(MY_POINT robo,BYTE d);
	BYTE		GetFreeStepsEx(MY_POINT robo,BYTE d,bool pickable);

	bool		IsPicTaken(MY_POINT robo,BYTE d);
	bool		IsBeenThere(MY_POINT robo,BYTE d);
	//bool		IsFeasible(MY_POINT robo,BYTE d);

	bool		GetFresh(MY_POINT p,BYTE *d);
	bool		GetUnknown(MY_POINT p,BYTE *d);

	//bool		IsUseLess(MY_POINT p,BYTE d);
	//void		WritePath(char *fname);
//private:
	//void		BorderToMap(MY_POINT robo,MY_POINT p,BYTE d);

};

#endif // !defined(AFX_MYMAP_H__2BDC3D5C_0F87_48CF_9AA4_9E1D30246A1E__INCLUDED_)
