// MyAction.cpp: implementation of the CMyAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyAction.h"

CMyAction::CMyAction(){}
CMyAction::~CMyAction(){}
//------------------------------------------------------------------------------------------
int CMyAction::DoMove(CRobot &robot,int steps)
{
	//Check battory
	if(CheckBat(robot,steps,0)==ID_STOP_MIS)
		return ID_STOP_MIS;
	//Move
	Response res = robot.Move(steps);
	
	if(res==ACTION_SUCCESSFUL)
	{
		SaveLoc(robot,steps);
		return ID_SUCCESS;
	}
	else
	{
		return CheckRet(res,robot);
	}
}
//---------------------------------------------------------------------------------------------
int CMyAction::DoTurn(CRobot &robot,Angle a)
{
	return CheckRet(robot.Turn(a),robot);
}
//------------------------------------------------------------------------------------------
int CMyAction::DoPickUp(CRobot &robot,int x,int y,CPicture &picture,ObjectType obj)
{
	//Is it battory (Special case)
	if(obj==OT_BATTERY_PACK)
	{
		//Pick
		Response res = robot.PickupPackage(x,y,"");

		if(res==ACTION_SUCCESSFUL)
		{
			m_rem_bat += picture.GetObjectHeightAt(x,y)*100;
			if(m_rem_bat>m_full_bat)
				m_rem_bat = m_full_bat;
			return ID_SUCCESS;
		}
		else
		{
			return CheckRet(res,robot);
		}
	}

	//Check battory
	if(CheckBat(robot,0,picture.GetObjectHeightAt(x,y))==ID_STOP_MIS)
		return ID_STOP_MIS;

	//Check cur stock
	if(m_storage>=55)
		return ID_STOP_MIS;
	//Pick
	Response res = robot.PickupPackage(x,y,"");

	//Get new store
	if(res==ACTION_SUCCESSFUL)
	{
		m_storage += picture.GetObjectHeightAt(x,y);
		return ID_SUCCESS;
	}
	else
	{
		return CheckRet(res,robot);
	}
}
//------------------------------------------------------------------------------------------
int CMyAction::DoTakePic(CRobot &robot,CPicture &picture)
{
	return CheckRet(robot.TakePicture(picture),robot);
}
//------------------------------------------------------------------------------------------
int CMyAction::DoOpenDoor(CRobot &robot)
{
	return CheckRet(robot.OpenDoor(),robot);
}
//------------------------------------------------------------------------------------------
int CMyAction::CheckBat(CRobot &robot,int new_st,int mew_stock)
{//To check once, it takes 1 unit of battory
CRobotInfo rinfo;
int nb,bs,nt;
	
	nt = 0;
	//Cal how much do I need to go back
	bs = GetBackSteps(&nt);
	//nb = ((bs*10)+m_storage+mew_stock) + ((new_st*10) + m_storage) + nt*3 + bs;

	nb = 1 + (10+((m_storage+mew_stock)*2)) * bs + (new_st*10) + nt*4 + bs;
	robot.GetInfo(rinfo);
	m_rem_bat = rinfo.GetRemainingBatteryPower();

	if((m_rem_bat-600)<=nb)
	{
		//MessageBox(NULL,"Bat Low!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
		return ID_STOP_MIS;
	}else{
		return ID_SUCCESS;
	}
}
