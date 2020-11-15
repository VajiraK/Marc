// MyAction.h: interface for the CMyAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYACTION_H__56B0C030_5FF4_4796_9919_A158BD5B4396__INCLUDED_)
#define AFX_MYACTION_H__56B0C030_5FF4_4796_9919_A158BD5B4396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGlobal.h"

class CMyAction : public CMyGlobal
{
public:
			CMyAction();
	virtual ~CMyAction();
	int		DoOpenDoor(CRobot &robot);
	int		DoMove(CRobot &robot,int steps);
	int		DoTurn(CRobot &robot,Angle a);
	int		DoPickUp(CRobot &robot,int x,int y,CPicture &picture,ObjectType obj=OT_NONE);
	int		DoTakePic(CRobot &robot,CPicture &picture);
private:
	int		CheckBat(CRobot &robot,int new_st,int mew_stock);
};

#endif // !defined(AFX_MYACTION_H__56B0C030_5FF4_4796_9919_A158BD5B4396__INCLUDED_)
