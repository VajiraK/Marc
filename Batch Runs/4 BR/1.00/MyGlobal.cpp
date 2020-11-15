// MyGlobal.cpp: implementation of the CMyGlobal class.

#include "stdafx.h"
#include "MyGlobal.h"

CMyGlobal::~CMyGlobal(){}

CMyGlobal::CMyGlobal()
{
	path_i = 0;
}
//---------------------------------------------------------------------------------------------
void CMyGlobal::CollectInfo(CRobot &robot)
{
CSpaceShipInfo	sinf;
CRobotInfo		rinfo;
int				f = 0;
	//Packs
	CMarc::GetSpaceShipInfo(sinf);
	m_rem_pack  = 0;
	m_rem_pack  = sinf.GetBrownPackageCount();
	m_rem_pack += sinf.GetYellowPackageCount();
	m_rem_pack += sinf.GetPinkPackageCount();
	m_rem_pack += sinf.GetBatteryPackCount();
	//Battory
	robot.GetInfo(rinfo);
	m_rem_bat  = rinfo.GetRemainingBatteryPower();
	m_full_bat = m_rem_bat;
	//Starting point
	SaveLoc(robot,f);
}
//----------------------------------------------------------------------------------------
void CMyGlobal::SaveLoc(CRobot &robot,int steps)
{
CLocation loc;

	robot.GetLocation(loc);
	m_path[path_i].loc.x  = loc.GetX();
	m_path[path_i].loc.y  = loc.GetY();
	m_path[path_i].came_d = m_facing;
	m_path[path_i].steps  = steps;
	path_i++;

	//if(m_facing==FACE_UP)
		//Beep(100,10);
}
//----------------------------------------------------------------------------------------
bool CMyGlobal::IsDurtyLoc(int steps)
{
	//Predict next location
MY_POINT new_loc;
	//steps--;
	switch (m_facing) 
	{ 
		case FACE_UP:
 			new_loc.y = m_path[path_i-1].loc.y - steps;
 			new_loc.x = m_path[path_i-1].loc.x;
 		break;
 		case FACE_DOWN:
 			new_loc.y = m_path[path_i-1].loc.y + steps;
 			new_loc.x = m_path[path_i-1].loc.x;
 		break;
 		case FACE_LEFT:
			new_loc.x = m_path[path_i-1].loc.x - steps;
			new_loc.y = m_path[path_i-1].loc.y;
		break;
		case FACE_RIGHT:
			new_loc.x = m_path[path_i-1].loc.x + steps;
			new_loc.y = m_path[path_i-1].loc.y;
		break;
	}

	//Search it in history (Back word)
	for(int i=path_i;i>=0;i--)
	{
		if((new_loc.x == m_path[i].loc.x) && (new_loc.y == m_path[i].loc.y))
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------
int CMyGlobal::CheckRet(Response res,CRobot &robot)
{
	if(res==ACTION_SUCCESSFUL)
	{
		return ID_SUCCESS;
	}else{
		/*char msg[200];
		CMarc::ResponseToStr(res,msg,200);
		MessageBox(NULL,msg,"Marc",MB_OK|MB_ICONEXCLAMATION);*/
		return ID_ERROR;
	}
}
//---------------------------------------------------------------------------------------------
int CMyGlobal::GetBackSteps(int* turns)
{
int bs = 0;
int s,d,temp_face;

	temp_face = m_facing;

	for(int i=(path_i-1);i>=1;i--)
	{
		s = i;
		if(FindSimilar(m_path[i].loc,&s))
			i = s;

		d = GetOposit(m_path[i].came_d);

		if(d==temp_face)
		{
			bs += m_path[i].steps;
		}else{
			*turns = *turns +1;
			temp_face = d;
			bs += m_path[i].steps;
		}
	}

	return bs;
}
//---------------------------------------------------------------------------------------------
bool CMyGlobal::FindSimilar(MY_POINT p,int *from)
{
	if((p.x==17)&&(p.y==12))
		int y=7;
	for(int i=0;i<=*from;i++)
	{
		if((p.x==m_path[i].loc.x) && (p.y==m_path[i].loc.y))
		{
			*from = i;
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------
int CMyGlobal::GetOposit(int d)
{
	switch (d) 
	{ 
		case FACE_DOWN:
			return FACE_UP;		
		break;
		case FACE_LEFT:
			return FACE_RIGHT;		
		break;
		case FACE_RIGHT:
			return FACE_LEFT;		
		break;
		case FACE_UP:
			return FACE_DOWN;	
		break;
	}	

	return 0;
}