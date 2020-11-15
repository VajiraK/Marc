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
	virtual		~CMyAction();
	BYTE		DoTurn(CRobot &robot,Angle a);
	bool		DoTakePic(CRobot &robot);
	void		BlockArea(MY_POINT *loc);
	BYTE		DoMoveEx(CRobot &robot,BYTE steps,WORD upper);
	BYTE		DoMove(CRobot &robot,BYTE steps,BYTE mode=MODE_MINING_PACKS);
	BYTE		DoPickUp(CRobot &robot,MY_POINT p,char* tag="");
	BYTE		DoDrop(CRobot &robot,MY_POINT p,char* tag="");
	BYTE		DoOpenDoor(CRobot &robot);
	BYTE		DoTelePort(CRobot &robot);
	void		FillArea(CPicture &picture);

private:
};

#endif // !defined(AFX_MYACTION_H__56B0C030_5FF4_4796_9919_A158BD5B4396__INCLUDED_)
