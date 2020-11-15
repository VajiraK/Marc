// MyMission.cpp: implementation of the CMyMission class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMission.h"

CMyMission::CMyMission(){}
CMyMission::~CMyMission(){}
//----------------------------------------------------------------------------------------
void CMyMission::Start(CRobot &robot)
{
CPicture picture;
int ret;

	for(int i=0;i<2000;i++)
	{
		ret = DoTakePic(robot,picture);
		switch (ret) 
		{ 
			case ID_SUCCESS:
				if(ScanPicture(robot,picture))
				{
					MakeDecision(robot);
				}else{
					return;
				}
			break;
			case ID_STOP_MIS:
				Finished(robot);
				return;	
			break;
			case ID_ERROR:
				return;		
			break;
		}	
	}
}
//----------------------------------------------------------------------------------------
bool CMyMission::ScanPicture(CRobot &m_robot,CPicture &picture)											
{									//|-------|(0,4)
int h = picture.GetHeight()-1;		//|	  ^	  |Picture
int w = picture.GetWidth()-1;		//|		  |
ObjectType obt;						//|		  |
int	ret;							//|-------|(0,0)

	//Make all rows empty
	for (int i=0;i<=4;i++)
		m_rows[i].empty = true;

	ret = SmartPick(m_robot,picture);
	if(ret==ID_STOP_MIS)
	{
		Finished(m_robot);
		return false;	
	}
		
	for (int y=h;y>=0;y--)
	{//Rows
		for (int x=w;x>=0;x--)
		{//Colunms
			obt = picture.GetObjectTypeAt(x,y);
			//Obstacles
			if ((obt==OT_WALL)||(obt==OT_BLACK_PACKAGE)||(obt==OT_DOOR))
			{
				m_rows[y].empty		= false;
				m_rows[y].ojc.type  = obt;
				m_rows[y].ojc.loc.x = x;
				m_rows[y].ojc.loc.y = y;
			}
		}
	}

	return true;
}
//----------------------------------------------------------------------------------------
void CMyMission::GetReady(CRobot &robot)
{
	m_facing  = FACE_DOWN;
	path_i	  = 0;
	m_storage = 0;

	if(CheckRet(robot.StartMission(),robot)==ID_SUCCESS)
	{
		CollectInfo(robot);
		Start(robot);
	}
}
//---------------------------------------------------------------------------------------
void CMyMission::MakeDecision(CRobot &robot)
{
int	row_i,ret;
int	steps = 0;
	
	//Find empty raws
	for (row_i=4;row_i>=0;row_i--)
	{//4th row is the closest
		if(m_rows[row_i].empty)
			steps++;
		else
			break;
	}
	
	if(steps!=0)
	{//There is room to go
		ret = Move(robot,steps);
		switch (ret) 
		{ 
			case ID_SUCCESS:
				if(steps<5)
				{//Not hole picture epmty
					if(DoOpenDoor(robot)==ID_ERROR)
						Turn90(robot);
				}
			break;
			case ID_STOP_MIS:
				Finished(robot);
				return;	
			break;
			case ID_ERROR:
				return;		
			break;
		}	
	}else{//No empty space to move
		if(m_rows[row_i].ojc.type==OT_DOOR)
		{
			if(DoOpenDoor(robot)==ID_ERROR)
				Turn90(robot);
		}else{
			Turn90(robot);
		}
	}
}
//---------------------------------------------------------------------------------------------
void CMyMission::Finished(CRobot &robot)
{
int d;
int s;
	for(int i=(path_i-1);i>=1;i--)
	{
		//s = i-1;
		s=i;
		if(FindSimilar(m_path[i].loc,&s))
			i = s;

		d = GetOposit(m_path[i].came_d);

		if(d==m_facing)
		{
			robot.Move(m_path[i].steps);
		}else{
			SetDirection(robot,d);
			robot.Move(m_path[i].steps);
		}
	}

CRobotInfo rinf;

	CheckRet(robot.EndMission(rinf),robot);

	/*int t = rinf.GetScore();
	m_rem_bat;

	if(t>0)
		MessageBox(NULL,"Accomplished...!","Marc",MB_OK|MB_ICONINFORMATION);
	else
		MessageBox(NULL,"Fail...!","Marc",MB_OK|MB_ICONINFORMATION);*/
}
