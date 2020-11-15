// MyMission.h: interface for the CMyMission class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMISSION_H__81349325_F3F6_4D05_9CE1_2F1FB7063CC4__INCLUDED_)
#define AFX_MYMISSION_H__81349325_F3F6_4D05_9CE1_2F1FB7063CC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyTelePort.h"

class CMyMission : public CMyTelePort
{
public:
				CMyMission();
	virtual		~CMyMission();
	void		GetReady(CRobot &robot);

private:
	BYTE	MakeWay(CRobot &robot,BYTE drop,BYTE st,BYTE mode);
	BYTE	DeployTug(CRobot &robot);
	BYTE	TryDirections(CRobot &robot);
	void	Start(CRobot &robot);
	BYTE	TakePictures(CRobot &robot,bool back=false);
	BYTE	GoForward(CRobot &robot);
	BYTE	FindNewWay(CRobot &robot);
	void	Finish(CRobot &robot);
};

#endif // !defined(AFX_MYMISSION_H__81349325_F3F6_4D05_9CE1_2F1FB7063CC4__INCLUDED_)
