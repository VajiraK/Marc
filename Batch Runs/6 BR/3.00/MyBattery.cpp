// MyBattery.cpp: implementation of the CMyBattery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBattery.h"

CMyBattery::CMyBattery(){}
CMyBattery::~CMyBattery(){}
//------------------------------------------------------------------------------------------
int CMyBattery::CheckBat(CRobot &robot,WORD new_st,WORD mew_stock)
{//To check once, it takes 1 unit of battory
int nb,bs,nt;

	nt = 0;
	//Cal how much do I need to go back
	bs = GetBackSteps(&nt);
	
	nb = 1 + ((10+((m_storage+mew_stock)*2)) * (bs+new_st)) + (nt*6) + bs + nt + m_bat_passblack;
/*CRobotInfo rinfo;
	robot.GetInfo(rinfo);
	m_rem_bat = rinfo.GetRemainingBatteryPower();*/

	if((m_rem_bat-400)<=nb)
	{
		//MessageBox(NULL,"Bat Low!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
		return RET_STOP_MIS;
	}else{
		return RET_SUCCESS;
	}
}
//---------------------------------------------------------------------------------------------
int CMyBattery::GetBackSteps(int* turns)
{
WORD bs = 0;
WORD s,d,temp_face;

	temp_face = m_facing;

	for(int i=(m_path_i-1);i>=1;i--)
	{
		s = i;
		if(FindShortCut(m_path[i].loc,0,&s))
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
//-------------------------------------------------------------------------------------------------
void CMyBattery::BatInit(CRobot &robot)
{
CRobotInfo		rinfo;

	robot.GetInfo(rinfo);

	m_rem_bat		= rinfo.GetRemainingBatteryPower();
	m_full_bat		= m_rem_bat;
	m_bat_passblack = 0;
}
//-------------------------------------------------------------------------------------------------
void CMyBattery::BatConsume(WORD unit)
{
	m_rem_bat -= unit;
}
