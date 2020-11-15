// MyDoor.cpp: implementation of the CMyDoor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyDoor.h"

CMyDoor::CMyDoor(){}
CMyDoor::~CMyDoor(){}
//------------------------------------------------------------------------------------------------
BYTE CMyDoor::CrackDoor(CRobot &robot)
{
	if(IsDoorThere(m_curent,m_facing))
	{//Facing a door
	MY_POINT p;

		p.x = 0;p.y = 4;
		PictureToMap(p);
		if(m_point.x==MAP_INVALIED)
			return RET_ERROR;
		CopyPoint(&m_door_p,&m_point);

		if(m_map[m_door_p.x][m_door_p.y].type==CL_DOOR)
		{
			return QuaryDoor(robot,LOCKSIDE_FILL);
		}else{
			return QuaryDoor(robot,LOCKSIDE_EMPTY);
		}
	}
	return RET_ERROR;
}
//-------------------------------------------------------------------------------------------
BYTE CMyDoor::QuaryDoor(CRobot &robot,BYTE orie)
{
	switch (m_facing) 
	{ 
		case FACE_DOWN:
			if(orie==LOCKSIDE_FILL)
			{
				if(m_map[m_door_p.x+1][m_door_p.y].type!=CL_DOOR)
				{//Most probably in coorrect orientation but if m_map[m_door_p.x+1][m_door_p.y]==CL_UNKNOWN 
				 //there,'s a chance of falling but it's ok to try 
					return Open(robot);
				}else{
					//No in corret orient (Definatly)
					return FillSearch(robot);
				}
			}else{
				return EmptySearch(robot);
			}	
			break;//==========================================

		case FACE_LEFT:
			if(orie==LOCKSIDE_FILL)
			{
				if(m_map[m_door_p.x][m_door_p.y+1].type!=CL_DOOR)
				{//Most probably in coorrect orientation but if m_map[m_door_p.x+1][m_door_p.y]==CL_UNKNOWN 
				 //there,'s a chance of falling but it's ok to try 
					return Open(robot);
				}else{
					//No in corret orient (Definatly)
					return FillSearch(robot);
				}
			}else{
				return EmptySearch(robot);
			}	
			break;//==========================================

		case FACE_RIGHT:
			if(orie==LOCKSIDE_FILL)
			{
				if(m_map[m_door_p.x][m_door_p.y-1].type!=CL_DOOR)
				{//Most probably in coorrect orientation but if m_map[m_door_p.x+1][m_door_p.y]==CL_UNKNOWN 
				 //there,'s a chance of falling but it's ok to try 
					return Open(robot);
				}else{
					//No in corret orient (Definatly)
					return FillSearch(robot);
				}
			}else{
				return EmptySearch(robot);
			}	
			break;//==========================================

		case FACE_UP:
			if(orie==LOCKSIDE_FILL)
			{
				if(m_map[m_door_p.x-1][m_door_p.y].type!=CL_DOOR)
				{//Most probably in coorrect orientation but if m_map[m_door_p.x+1][m_door_p.y]==CL_UNKNOWN 
				 //there,'s a chance of falling but it's ok to try 
					return Open(robot);
				}else{
					//No in corret orient (Definatly)
					return FillSearch(robot);
				}
			}else{
				return EmptySearch(robot);
			}	
			break;
	}
	return RET_SUCCESS;
}
//-----------------------------------------------------------------------------------------------
BYTE CMyDoor::FillSearch(CRobot &robot)
{
WORD steps;
int  ret,i,lag;
	switch (m_facing) 
	{
		case FACE_DOWN:
			for(i=1;i<=20;i++)
			{//Search to door lock (Eadge)
				if(m_map[m_door_p.x+i][m_door_p.y].type!=CL_DOOR)
				{//Found
					steps = GetFreeSteps(m_curent,FACE_RIGHT);
					if(steps==0)
						break;//No room to move
					i--;
					if(steps>=i)
					{//Robo can move that far
						m_door_p.x += i;//Door clearing start from map
						SetDirection(robot,FACE_RIGHT);
						ret = DoMove(robot,i);
						if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_DOWN);
						return Open(robot);
					}else{//Need more room (Door is longer than steps)
						if(steps!=5)//Surly no more space
							return RET_SUCCESS;
					MY_POINT p; p.x = m_curent.x; p.y = m_curent.y;
						lag = i-steps;
						PlotLocation(&p,FACE_RIGHT,steps);
						steps = GetFreeSteps(p,FACE_RIGHT);
						if(lag<=steps)
						{
							SetDirection(robot,FACE_RIGHT);
							ret = DoMove(robot,i);
							if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
							SetDirection(robot,FACE_DOWN);
							p.x = 0;p.y = 4;
							PictureToMap(p);//Re-map m_door_p(Because posisson is changed)
							CopyPoint(&m_door_p,&m_point);
							return Open(robot);
						}else{
							return RET_SUCCESS;
						}
					}
				}
			}
			break;//=================================================

		case FACE_LEFT:
			for(i=1;i<=20;i++)
			{//Search to door lock (Eadge)
				if(m_map[m_door_p.x][m_door_p.y+i].type!=CL_DOOR)
				{//Found
					steps = GetFreeSteps(m_curent,FACE_DOWN);
					if(steps==0)
						break;//No room to move
					i--;
					if(steps>=i)
					{//Robo can move that far
						m_door_p.y += i;//Door clearing start from map
						SetDirection(robot,FACE_DOWN);
						ret = DoMove(robot,i);
						if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_LEFT);
						return Open(robot);
					}else{//Need more room (Door is longer than steps)
						if(steps!=5)//Surly no more space
							return RET_SUCCESS;
					MY_POINT p; p.x = m_curent.x; p.y = m_curent.y;
						lag = i-steps;
						PlotLocation(&p,FACE_DOWN,steps);
						steps = GetFreeSteps(p,FACE_DOWN);
						if(lag<=steps)
						{
							SetDirection(robot,FACE_DOWN);
							ret = DoMove(robot,i);
							if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
							SetDirection(robot,FACE_LEFT);
							p.x = 0;p.y = 4;
							PictureToMap(p);//Re-map m_door_p(Because posisson is changed)
							CopyPoint(&m_door_p,&m_point);
							return Open(robot);
						}else{
							return RET_SUCCESS;
						}
					}
				}
			}
			break;//=================================================

		case FACE_RIGHT:
			for(i=1;i<=20;i++)
			{//Search to door lock (Eadge)
				if(m_map[m_door_p.x][m_door_p.y-i].type!=CL_DOOR)
				{//Found
					steps = GetFreeSteps(m_curent,FACE_UP);
					if(steps==0)
						break;//No room to move
					i--;
					if(steps>=i)
					{//Robo can move that far
						m_door_p.y -= i;//Door clearing start from map
						SetDirection(robot,FACE_UP);
						ret = DoMove(robot,i);
						if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_RIGHT);
						return Open(robot);
					}else{//Need more room (Door is longer than steps)
						if(steps!=5)//Surly no more space
							return RET_SUCCESS;
					MY_POINT p; p.x = m_curent.x; p.y = m_curent.y;
						lag = i-steps;
						PlotLocation(&p,FACE_UP,steps);
						steps = GetFreeSteps(p,FACE_UP);
						if(lag<=steps)
						{
							SetDirection(robot,FACE_UP);
							ret = DoMove(robot,i);
							if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
							SetDirection(robot,FACE_RIGHT);
							p.x = 0;p.y = 4;
							PictureToMap(p);//Re-map m_door_p(Because posisson is changed)
							CopyPoint(&m_door_p,&m_point);
							return Open(robot);
						}else{
							return RET_SUCCESS;
						}
					}
				}
			}
			break;//=================================================

		case FACE_UP:
			for(i=1;i<=20;i++)
			{//Search to door lock (Eadge)
				if(m_map[m_door_p.x-i][m_door_p.y].type!=CL_DOOR)
				{//Found
					steps = GetFreeSteps(m_curent,FACE_LEFT);
					if(steps==0)
						break;//No room to move
					i--;
					if(steps>=i)
					{//Robo can move that far
						m_door_p.x -= i;//Door clearing start from map
						SetDirection(robot,FACE_LEFT);
						ret = DoMove(robot,i);
						if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_UP);
						return Open(robot);
					}else{//Need more room (Door is longer than steps)
						if(steps!=5)//Surly no more space
							return RET_SUCCESS;
					MY_POINT p; p.x = m_curent.x; p.y = m_curent.y;
						lag = i-steps;
						PlotLocation(&p,FACE_LEFT,steps);
						steps = GetFreeSteps(p,FACE_LEFT);
						if(lag<=steps)
						{
							SetDirection(robot,FACE_LEFT);
							ret = DoMove(robot,i);
							if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
							SetDirection(robot,FACE_UP);
							p.x = 0;p.y = 4;
							PictureToMap(p);//Re-map m_door_p(Because posisson is changed)
							CopyPoint(&m_door_p,&m_point);
							return Open(robot);
						}else{
							return RET_SUCCESS;
						}
					}
				}
			}
			break;
	}
	return RET_SUCCESS;
}
//-----------------------------------------------------------------------------------------------
BYTE CMyDoor::EmptySearch(CRobot &robot)
{
WORD steps;
BYTE ret;
int  i,lag;

	switch (m_facing) 
	{
	case FACE_DOWN:
			for(i=1;i<=20;i++)
			{//Search to door lock (Eadge)
				if(m_map[m_door_p.x-i][m_door_p.y].type==CL_DOOR)
				{//Found
					steps = GetFreeSteps(m_curent,FACE_LEFT);
					if(steps==0)
						break;//No room to move
					if(steps>=i)
					{//Robo can move that far
						m_door_p.x -= i;//Door clearing start from map
						SetDirection(robot,FACE_LEFT);
						ret = DoMove(robot,i);
						if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_DOWN);
						return Open(robot);
					}else{//Need more room (Door is longer than steps)
						if(steps!=5)//Surly no more space
							return RET_SUCCESS;
					MY_POINT p; p.x = m_curent.x; p.y = m_curent.y;
						lag = i-steps;
						PlotLocation(&p,FACE_LEFT,steps);
						steps = GetFreeSteps(p,FACE_LEFT);
						if(lag<=steps)
						{
							SetDirection(robot,FACE_LEFT);
							ret = DoMove(robot,i);
							if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
							SetDirection(robot,FACE_DOWN);
							p.x = 0;p.y = 4;
							PictureToMap(p);//Re-map m_door_p(Because posisson is changed)
							CopyPoint(&m_door_p,&m_point);
							return Open(robot);
						}else{
							return RET_SUCCESS;
						}
					}
				}
			}
		break;//============================================

	case FACE_LEFT:
			for(i=1;i<=20;i++)
			{//Search to door lock (Eadge)
				if(m_map[m_door_p.x][m_door_p.y-i].type==CL_DOOR)
				{//Found
					steps = GetFreeSteps(m_curent,FACE_UP);
					if(steps==0)
						break;//No room to move
					if(steps>=i)
					{//Robo can move that far
						m_door_p.y -= i;//Door clearing start from map
						SetDirection(robot,FACE_UP);
						ret = DoMove(robot,i);
						if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_LEFT);
						return Open(robot);
					}else{//Need more room (Door is longer than steps)
						if(steps!=5)//Surly no more space
							return RET_SUCCESS;
					MY_POINT p; p.x = m_curent.x; p.y = m_curent.y;
						lag = i-steps;
						PlotLocation(&p,FACE_UP,steps);
						steps = GetFreeSteps(p,FACE_UP);
						if(lag<=steps)
						{
							SetDirection(robot,FACE_UP);
							ret = DoMove(robot,i);
							if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
							SetDirection(robot,FACE_LEFT);
							p.x = 0;p.y = 4;
							PictureToMap(p);//Re-map m_door_p(Because posisson is changed)
							CopyPoint(&m_door_p,&m_point);
							return Open(robot);
						}else{
							return RET_SUCCESS;
						}
					}
				}
			}
		break;//============================================

	case FACE_RIGHT:
			for(i=1;i<=20;i++)
			{//Search to door lock (Eadge)
				if(m_map[m_door_p.x][m_door_p.y+i].type==CL_DOOR)
				{//Found
					steps = GetFreeSteps(m_curent,FACE_DOWN);
					if(steps==0)
						break;//No room to move
					if(steps>=i)
					{//Robo can move that far
						m_door_p.y += i;//Door clearing start from map
						SetDirection(robot,FACE_DOWN);
						ret = DoMove(robot,i);
						if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_RIGHT);
						return Open(robot);
					}else{//Need more room (Door is longer than steps)
						if(steps!=5)//Surly no more space
							return RET_SUCCESS;
					MY_POINT p; p.x = m_curent.x; p.y = m_curent.y;
						lag = i-steps;
						PlotLocation(&p,FACE_DOWN,steps);
						steps = GetFreeSteps(p,FACE_DOWN);
						if(lag<=steps)
						{
							SetDirection(robot,FACE_DOWN);
							ret = DoMove(robot,i);
							if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
							SetDirection(robot,FACE_RIGHT);
							p.x = 0;p.y = 4;
							PictureToMap(p);//Re-map m_door_p(Because posisson is changed)
							CopyPoint(&m_door_p,&m_point);
							return Open(robot);
						}else{
							return RET_SUCCESS;
						}
					}
				}
			}
		break;//======================================

	case FACE_UP:
			for(i=1;i<=20;i++)
			{//Search to door lock (Eadge)
				if(m_map[m_door_p.x+i][m_door_p.y].type==CL_DOOR)
				{//Found
					steps = GetFreeSteps(m_curent,FACE_RIGHT);
					if(steps==0)
						break;//No room to move
					if(steps>=i)
					{//Robo can move that far
						m_door_p.x += i;//Door clearing start from map
						SetDirection(robot,FACE_RIGHT);
						ret = DoMove(robot,i);
						if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
			 			SetDirection(robot,FACE_UP);
						return Open(robot);
					}else{//Need more room (Door is longer than steps)
						if(steps!=5)//Surly no more space
							return RET_SUCCESS;
					MY_POINT p; p.x = m_curent.x; p.y = m_curent.y;
						lag = i-steps;
						PlotLocation(&p,FACE_RIGHT,steps);
						steps = GetFreeSteps(p,FACE_RIGHT);
						//steps = VirScanPic(p,FACE_RIGHT,MODE_FREE_ST);
						if(lag<=steps)
						{
							SetDirection(robot,FACE_RIGHT);
							ret = DoMove(robot,i);
							if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;
							SetDirection(robot,FACE_UP);
							p.x = 0;p.y = 4;
							PictureToMap(p);//Re-map m_door_p(Because posisson is changed)
							CopyPoint(&m_door_p,&m_point);
							return Open(robot);
						}else{
							return RET_SUCCESS;
						}
					}
				}
			}

		break;
	}
	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------
BYTE CMyDoor::Open(CRobot &robot)
{	
int  ret,i;

	//Hey let's try to open
	ret = DoOpenDoor(robot);
	//ret = CheckRet(robot.OpenDoor());
	if(ret==RET_SUCCESS)
	{//Horrraaayy........
		switch (m_facing) 
		{ 
			case FACE_DOWN:
				for(i=0;i<=20;i++)
				{//Clear open door from virtual map (Max width 20)
					if(m_map[m_door_p.x-i][m_door_p.y].type==CL_DOOR)
						m_map[m_door_p.x-i][m_door_p.y].type=CL_FREE;
					else
						break;
				}
				/*steps = VirScanPic(m_curent,m_facing,MODE_FREE_ST);
				//Go through the open door
				ret = DoMove(robot,steps);
				if(ret==RET_STOP_MIS)
					return RET_STOP_MIS;*/
				break;//=============================================

			case FACE_LEFT:
				for(i=0;i<=20;i++)
				{//Clear open door from virtual map (Max width 20)
					if(m_map[m_door_p.x][m_door_p.y-i].type==CL_DOOR)
						m_map[m_door_p.x][m_door_p.y-i].type=CL_FREE;
					else
						break;
				}
				break;//=============================================

			case FACE_RIGHT:
				for(i=0;i<=20;i++)
				{//Clear open door from virtual map (Max width 20)
					if(m_map[m_door_p.x][m_door_p.y+i].type==CL_DOOR)
						m_map[m_door_p.x][m_door_p.y+i].type=CL_FREE;
					else
						break;
				}
				break;//=============================================

			case FACE_UP:
				for(i=0;i<=20;i++)
				{//Clear open door from virtual map (Max width 20)
					if(m_map[m_door_p.x+i][m_door_p.y].type==CL_DOOR)
						m_map[m_door_p.x+i][m_door_p.y].type=CL_FREE;
					else
						break;
				}
				break;//=============================================
		}		
	}

	return ret;
}
//---------------------------------------------------------------------------------------------------
bool CMyDoor::IsDoorThere(MY_POINT robo,BYTE d)
{
MY_POINT p;

	for (int x=0;x<=4;x++)
	{	
		p.x=x;p.y=4;
		PictureToMap(robo,p,d);
		if(m_point.x!=MAP_INVALIED)
		{
			if(m_map[m_point.x][m_point.y].type==CL_DOOR)
				return true;
		}
	}

return false;
}
