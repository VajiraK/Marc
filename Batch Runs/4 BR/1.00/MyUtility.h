// MyUtility.h: interface for the CMyUtility class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYUTILITY_H__B1EA56A9_9338_4F1C_A5D5_8DF03F119ABB__INCLUDED_)
#define AFX_MYUTILITY_H__B1EA56A9_9338_4F1C_A5D5_8DF03F119ABB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyDirection.h"

class CMyUtility : public CMyDirection
{
public:
			CMyUtility();
	virtual ~CMyUtility();
	bool	CrackDoor();
	int		Move(CRobot &robot,int steps);
	int		TurnRandom(CRobot &robot);
	int		Turn90(CRobot &robot);
	int		Turn_90(CRobot &robot);
	int		Turn180(CRobot &robot);
	void	SaveLoc(CRobot &robot,int steps);
	int		SmartPick(CRobot &robot,CPicture &picture);

private:
	int		m_ret;

};

#endif // !defined(AFX_MYUTILITY_H__B1EA56A9_9338_4F1C_A5D5_8DF03F119ABB__INCLUDED_)
