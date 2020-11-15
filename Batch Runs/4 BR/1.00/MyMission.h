// MyMission.h: interface for the CMyMission class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMISSION_H__81349325_F3F6_4D05_9CE1_2F1FB7063CC4__INCLUDED_)
#define AFX_MYMISSION_H__81349325_F3F6_4D05_9CE1_2F1FB7063CC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyUtility.h"

class CMyMission : public CMyUtility
{
public:
				CMyMission();
	virtual		~CMyMission();
	void		GetReady(CRobot &robot);

private:
	//bool	FindSimilar(MY_POINT p,int *from);
	void	Finished(CRobot &robot);
	void	MakeDecision(CRobot &robot);
	bool	ScanPicture(CRobot &robot,CPicture &picture);
	void	Start(CRobot &robot);
};

#endif // !defined(AFX_MYMISSION_H__81349325_F3F6_4D05_9CE1_2F1FB7063CC4__INCLUDED_)
