// MyDoor.h: interface for the CMyDoor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDOOR_H__61B7A08D_88E6_4450_8BF5_D3489EDA4678__INCLUDED_)
#define AFX_MYDOOR_H__61B7A08D_88E6_4450_8BF5_D3489EDA4678__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyMap.h"

#define		LOCKSIDE_FILL		1 // ORIENTATION FLAGS
#define		LOCKSIDE_EMPTY		2

class CMyDoor  : public CMyMap 
{
public:
				CMyDoor();
	virtual		~CMyDoor();
	BYTE		CrackDoor(CRobot &robot);
private:
	BYTE		EmptySearch(CRobot &robot);
	BYTE		FillSearch(CRobot &robot);
	BYTE		QuaryDoor(CRobot &robot,BYTE orie);
	BYTE		Open(CRobot &robot);
	bool		IsDoorThere(MY_POINT robo,BYTE d);

	MY_POINT	m_door_p;

};

#endif // !defined(AFX_MYDOOR_H__61B7A08D_88E6_4450_8BF5_D3489EDA4678__INCLUDED_)
