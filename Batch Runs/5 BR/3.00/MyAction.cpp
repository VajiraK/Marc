// MyAction.cpp: implementation of the CMyAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyAction.h"

CMyAction::CMyAction(){}
CMyAction::~CMyAction(){}
//---------------------------------------------------------------------------------------------
BYTE CMyAction::DoTurn(CRobot &robot,Angle a)
{
	if(a==DEGREES_180)
		BatConsume(5);
	else
		BatConsume(3);

	return CheckRet(robot.Turn(a));
}
//------------------------------------------------------------------------------------------
bool CMyAction::DoTakePic(CRobot &robot,CPicture &picture)
{
	if(CheckRet(robot.TakePicture(picture))==RET_SUCCESS)
	{
		BatConsume(3);
		return true;
	}
	else
	{
		BatConsume(1);
		return false;
	}
}
//-----------------------------------------------------------------------------------------------
BYTE CMyAction::DoMove(CRobot &robot,WORD steps,BYTE flag)
{
	if(steps!=0)
	{
	bool lb = false;
		//Check battory
		if(CheckBat(robot,steps,0)==RET_STOP_MIS)
		{
			if(flag==MODE_SAVE_LOC)
				return RET_STOP_MIS;
			else
				lb = true;//Going to last tele must not return
		}

		//Move
		Response res = robot.Move(steps);
		
		if(res==ACTION_SUCCESSFUL)
		{
		WORD bu;
			bu = 1 + ((10 + (m_storage * 2))  *  steps);
			BatConsume(bu);
			//Save loc
			if(flag==MODE_SAVE_LOC)
				SaveLoc(robot,steps);
			if(lb)
				return RET_STOP_MIS;
			else
				return RET_SUCCESS;
		}
		else
		{
			BatConsume(1);
			return CheckRet(res);
		}
	}

	return RET_SUCCESS;
}
//-----------------------------------------------------------------------------------------------
BYTE CMyAction::DoPickUp(CRobot &robot,MY_POINT p,char* tag)
{
	//Pick up
	Response res = robot.PickupPackage(p.x,p.y,tag);

	if(res==ACTION_SUCCESSFUL)
	{
		BatConsume(4);
		return RET_SUCCESS;
	}else{
		BatConsume(1);
		return CheckRet(res);
	}

	return RET_ERROR;
}
//-----------------------------------------------------------------------------------------------
BYTE CMyAction::DoDrop(CRobot &robot,MY_POINT p,char* tag)
{
	//Drop
	Response res = robot.DropPackage(p.x,p.y,tag);

	if(res==ACTION_SUCCESSFUL)
	{
		BatConsume(3);
		return RET_SUCCESS;
	}else{
		BatConsume(1);
		return CheckRet(res);
	}

	return RET_ERROR;
}
//----------------------------------------------------------------------------------------------
BYTE CMyAction::DoOpenDoor(CRobot &robot)
{
	//Door
	Response res = robot.OpenDoor();

	if(res==ACTION_SUCCESSFUL)
	{
		BatConsume(1);
		return RET_SUCCESS;
	}else{
		BatConsume(1);
		return CheckRet(res);
	}

	return RET_ERROR;
}
//------------------------------------------------------------------------------------------
BYTE CMyAction::DoTelePort(CRobot &robot)
{  
Response	res;
CLocation	l;

	res = robot.Teleport();
	BatConsume(1);

	switch(res)
	{
	case ACTION_SUCCESSFUL:
   		SaveLoc(robot,0,ID_TELE);
		return RET_SUCCESS;

	case TELEPORT_DESTINATION_BLOCKED:
		return RET_TELE_BLOCK;
	}

	return RET_ERROR;
}
