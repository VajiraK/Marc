// MyAction.h: interface for the CMyAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYACTION_H__56B0C030_5FF4_4796_9919_A158BD5B4396__INCLUDED_)
#define AFX_MYACTION_H__56B0C030_5FF4_4796_9919_A158BD5B4396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyBattery.h"

class CMyAction : public CMyBattery
{
public:
			CMyAction();
	virtual ~CMyAction();
	BYTE	DoTurn(CRobot &robot,Angle a);
	bool	DoTakePic(CRobot &robot,CPicture &picture);
	BYTE	DoMove(CRobot &robot,WORD steps,BYTE flag=MODE_SAVE_LOC);
	BYTE	DoPickUp(CRobot &robot,MY_POINT p,char* tag="");
	BYTE	DoDrop(CRobot &robot,MY_POINT p,char* tag="");
	BYTE	DoOpenDoor(CRobot &robot);
	BYTE	DoTelePort(CRobot &robot);
};

#endif // !defined(AFX_MYACTION_H__56B0C030_5FF4_4796_9919_A158BD5B4396__INCLUDED_)
