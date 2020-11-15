// MyBattery.cpp: implementation of the CMyBattery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBattery.h"

CMyBattery::CMyBattery(){}
CMyBattery::~CMyBattery(){}
//------------------------------------------------------------------------------------------
int CMyBattery::CheckBat(WORD new_st,WORD mew_stock)
{
int nb,bs,nt;

	nt = 0;
	//Cal how much do I need to go back
	bs = GetBackSteps(m_path_i,&nt);
	
	nb = 1 + ((10+((GetCollected()+mew_stock)*2)) * (bs+new_st)) + (nt*6) + bs + nt + m_bat_passblack;

	if((m_rem_bat)<=nb)
	{
		//MessageBox(NULL,"Bat Low!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
		return RET_STOP_MIS;
	}else{
		return RET_SUCCESS;
	}
}
//------------------------------------------------------------------------------------------
int CMyBattery::CheckBatEx(WORD upper,WORD new_st)
{
int nb,bs,nt;

	nt = 0;
	//Cal how much do I need to go back
	bs = GetBackSteps(upper,&nt);
	
	nb = 1 + ((10+((GetCollected())*2)) * (bs+new_st)) + (nt*6) + bs + nt + m_bat_passblack;

	if((m_rem_bat)<=nb)
	{
		//MessageBox(NULL,"Bat Low!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
		return RET_STOP_MIS;
	}else{
		return RET_SUCCESS;
	}
}
//---------------------------------------------------------------------------------------------
int CMyBattery::GetBackSteps(WORD upper,int* turns)
{
WORD s,d,bs,temp_face;

	temp_face = m_facing;
	*turns = bs = 0;

	for(int i=(upper-1);i>=1;i--)
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
	//Checks shows that one step is lack
	*turns++;
	return bs;
}
//-------------------------------------------------------------------------------------------------
void CMyBattery::InitBat(CRobot &robot)
{
CRobotInfo	rinfo;

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
