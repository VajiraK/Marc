// MyUtility.cpp: implementation of the CMyUtility class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyUtility.h"

CMyUtility::CMyUtility(){}
CMyUtility::~CMyUtility(){}
//----------------------------------------------------------------------------------------
int CMyUtility::Move(CRobot &robot,int steps)
{
int direc,ret;
static int	dirty_c = 0;
	
	dirty_c++;
	direc = m_facing;

	if(dirty_c!=2)
	{//***************************************************
		for(int i=0;i<=2;i++)
		{
			if(IsDurtyLoc(steps))
			{//Robot has been there(So try to go other way)
				Turn90(robot);
			}else{
				break;
			}
		}
	
	if(direc!=m_facing)//Drection has chnged so take new pic(To avoid colision)
		return false;
	}else{
		dirty_c = 0;
	}
	//***************************************************
	
	dirty_c--;

	return DoMove(robot,steps);
}
//---------------------------------------------------------------------------------------------
int CMyUtility::TurnRandom(CRobot &robot)
{
BYTE		tu;
CLocation	loc;
SYSTEMTIME	st;
	Sleep(100);
	GetSystemTime(&st);
	srand(loc.GetX()+st.wMilliseconds);

	do
	{
		tu = rand();
		if((tu>0)&&(tu<3))
		break;
	}while(1);

	m_ret = DoTurn(robot,(Angle)tu);
	if(m_ret==ID_SUCCESS)
		SetFacing((Angle)tu);
	return m_ret;
}
//---------------------------------------------------------------------------------------------
int CMyUtility::Turn180(CRobot &robot)
{
	m_ret = DoTurn(robot,(DEGREES_180));
	if(m_ret==ID_SUCCESS)
		SetFacing(DEGREES_180);
	return m_ret;
}
//---------------------------------------------------------------------------------------------
int CMyUtility::Turn90(CRobot &robot)
{
	m_ret = DoTurn(robot,(DEGREES_90));
	if(m_ret==ID_SUCCESS)
		SetFacing(DEGREES_90);
	return m_ret;
}
//---------------------------------------------------------------------------------------------
int CMyUtility::Turn_90(CRobot &robot)
{
	m_ret = DoTurn(robot,(DEGREES_MINUS_90));
	if(m_ret==ID_SUCCESS)
		SetFacing(DEGREES_MINUS_90);
	return m_ret;
}
//------------------------------------------------------------------------------------------
void CMyUtility::SaveLoc(CRobot &robot,int steps)
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
//------------------------------------------------------------------------------
int CMyUtility::SmartPick(CRobot &robot,CPicture &picture)
{
int ret,x,y;
ObjectType obt;
int h = picture.GetHeight()-1;
int w = picture.GetWidth()-1;
	//YELLOWS ***************************************
	for (y=h;y>=0;y--)
	{
		for (x=w;x>=0;x--)
		{
			obt = picture.GetObjectTypeAt(x,y);
			if ((obt==OT_YELLOW_PACKAGE))
			{
				ret = DoPickUp(robot,x,y,picture);

				switch (ret) 
				{ 
					case ID_SUCCESS:
						m_rem_pack--;
						if(m_rem_pack==0)
							return ID_STOP_MIS;
					break;
					case ID_STOP_MIS:
						return ID_STOP_MIS;	
					break;
				}	
			}
		}
	}

	//PINK ***************************************
	for (y=h;y>=0;y--)
	{
		for (x=w;x>=0;x--)
		{
			obt = picture.GetObjectTypeAt(x,y);
			if ((obt==OT_PINK_PACKAGE))
			{
				ret = DoPickUp(robot,x,y,picture);
				switch (ret) 
				{ 
					case ID_SUCCESS:
						m_rem_pack--;
						if(m_rem_pack==0)
							return ID_STOP_MIS;
					break;
					case ID_STOP_MIS:
						return ID_STOP_MIS;	
					break;
				}	
			}
		}
	}

	//BROWN ***************************************
	for (y=h;y>=0;y--)
	{
		for (x=w;x>=0;x--)
		{
			obt = picture.GetObjectTypeAt(x,y);
			if ((obt==OT_BROWN_PACKAGE))
			{
				ret = DoPickUp(robot,x,y,picture);
				switch (ret) 
				{ 
					case ID_SUCCESS:
						m_rem_pack--;
						if(m_rem_pack==0)
							return ID_STOP_MIS;
					break;
					case ID_STOP_MIS:
						return ID_STOP_MIS;	
					break;
				}	
			}
		}
	}

	//BATTORY ***************************************
	for (y=h;y>=0;y--)
	{
		for (x=w;x>=0;x--)
		{
			obt = picture.GetObjectTypeAt(x,y);
			if ((obt==OT_BATTERY_PACK))
			{
				ret = DoPickUp(robot,x,y,picture,OT_BATTERY_PACK);
				switch (ret) 
				{ 
					case ID_SUCCESS:
						m_rem_pack--;
						if(m_rem_pack==0)
							return ID_STOP_MIS;
					break;
					case ID_STOP_MIS:
						return ID_STOP_MIS;	
					break;
				}	
			}
		}
	}
	return ID_SUCCESS;
}

//------------------------------------------------------------------------------
bool CMyUtility::CrackDoor()
{
	return 1;
}

