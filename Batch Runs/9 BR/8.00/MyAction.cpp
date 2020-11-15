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
bool CMyAction::DoTakePic(CRobot &robot)
{
CPicture picture;
	if(CheckRet(robot.TakePicture(picture))==RET_SUCCESS)
	{
		FillArea(picture);
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
BYTE CMyAction::DoMoveEx(CRobot &robot,BYTE steps,WORD upper)
{//Special DoMove (1)no save pos (2)batcheck to specified path location

	if(steps!=0)
	{
		//Check battory
		if(CheckBatEx(upper,steps,GetColorHeight())==RET_STOP_MIS)
			return RET_STOP_MIS;

		//Move
		Response res = robot.Move(steps);
		
		if(res==ACTION_SUCCESSFUL)
		{
		WORD bu;
			bu = 1 + ((10 + (GetStock(ST_COL_BK) * 2))  *  steps);
			BatConsume(bu);
			//Update current (Normaly call by AddToPath)
			PlotLocation(&m_curent,m_facing,steps);
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
BYTE CMyAction::DoMove(CRobot &robot,BYTE steps,BYTE mode)
{//Commonly use DoMove over load
	if(steps!=0)
	{
		if(mode==MODE_MINING_PACKS)
		{
			//Check battory (befor move)
			if(CheckBat(steps,GetColorHeight())==RET_STOP_MIS)
				return RET_STOP_MIS;
		}

		//Move
		Response res = robot.Move(steps);
		
		if(res==ACTION_SUCCESSFUL)
		{
		WORD bu;
			bu = 1 + ((10 + (GetStock(ST_COL_BK) * 2))  *  steps);
			BatConsume(bu);
			//Save loc
			if(mode==MODE_MINING_PACKS)
				AddToPath(steps);
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
		return RET_ERROR;
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

	if(res==ACTION_SUCCESSFUL)
	{
	CLocation loc;
		robot.GetLocation(loc);
		BatConsume(1);//for GetLocation()
		m_curent.x  = (WORD)loc.GetX();
		m_curent.y  = (WORD)loc.GetY();
		AddToPath(0,PA_TELE);
		return RET_SUCCESS;
	}else{
		return RET_ERROR;
	}
}
//-------------------------------------------------------------------------------------------------
void CMyAction::FillArea(CPicture &picture)
{
ObjectType	obt;
MY_POINT	map;
BYTE		h;

	for(BYTE i=0;i<25;i++)
	{
		obt = picture.GetObjectTypeAt(m_row_ord[i].x,m_row_ord[i].y);
		h	= picture.GetObjectHeightAt(m_row_ord[i].x,m_row_ord[i].y);

		//PictureToMapEx(&m_row_ord[i],&map);

		if(PictureToMapEx(&m_row_ord[i],&map))
		{
			switch (obt) 
			{ 
				case OT_WALL:
					m_map[map.x][map.y].type = CL_WALL;
					m_map[map.x][map.y].h	 = h;
				break;
				case OT_DOOR:
					m_map[map.x][map.y].type = CL_DOOR;	
					m_map[map.x][map.y].h	 = h;
				break;
				case OT_NONE:
					m_map[map.x][map.y].type = CL_FREE;	
					m_map[map.x][map.y].h	 = 0;
				break;
				case OT_TELEPORT_BEAM:
					m_map[map.x][map.y].type = CL_TELE;
					m_map[map.x][map.y].h	 = h;
				break;
				case OT_BLACK_PACKAGE:
					m_map[map.x][map.y].type = CL_BLACK;
					m_map[map.x][map.y].realbk= VALIED;
					m_map[map.x][map.y].h	 = h;
				break;
				case OT_BATTERY_PACK:
					m_map[map.x][map.y].type = CL_BATTERY;
					m_map[map.x][map.y].h	 = h;
				break;
				case OT_YELLOW_PACKAGE:
					m_map[map.x][map.y].type = CL_YELLOW;
					m_map[map.x][map.y].h	 = h;
				break;
				case OT_PINK_PACKAGE:
					m_map[map.x][map.y].type = CL_PINK;
					m_map[map.x][map.y].h	 = h;
				break;
				case OT_BROWN_PACKAGE:
					m_map[map.x][map.y].type = CL_BROWN;
					m_map[map.x][map.y].h	 = h;
				break;	
			}
		}
	}
}