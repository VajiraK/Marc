// MyBattery.cpp: implementation of the CMyBattery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBatAgent.h"
#include "MyBattery.h"

CMyBattery::CMyBattery(){}
CMyBattery::~CMyBattery(){}
//------------------------------------------------------------------------------------------
BYTE CMyBattery::CheckBat(BYTE new_st,BYTE stock)
{
MY_POINT p;
bool	 vm = false;

	if(new_st!=0)
	{//Checking befor move
		p.x=m_curent.x;p.y=m_curent.y;
		AddToPath(new_st,false);
		vm = true;
	}

	m_last_bat = SendAgent(m_path_i,stock);
	
	if(vm)
	{//Erase virtual move from path his
		m_path_i--;
		m_curent.x=p.x;m_curent.y=p.y;
	}
	
	if(m_rem_bat<=m_last_bat)
	{
		#ifdef _DEBUG
		MessageBox(NULL,"Bat Low!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
		#endif
		return RET_STOP_MIS;
	}else{
		return RET_SUCCESS;
	}
}
//------------------------------------------------------------------------------------------
BYTE CMyBattery::CheckBatEx(WORD upper,BYTE new_st,BYTE stock)
{
DWORD nb;

	nb = SendAgent(upper,stock);
	
	if(m_rem_bat<=nb+2000)
	{
		#ifdef _DEBUG
		MessageBox(NULL,"Bat Low!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
		#endif
		return RET_STOP_MIS;
	}else{
		return RET_SUCCESS;
	}
}
//---------------------------------------------------------------------------------------------
int CMyBattery::GetBackSteps(WORD upper,int* turns)
{
WORD d,bs,temp_face;
//WORD s;
	temp_face = m_facing;
	*turns = bs = 0;

	for(int i=(upper-1);i>=1;i--)
	{
		/*s = i;
		if(FindShortCut(m_path[i].loc,0,&s))
			i = s;*/

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

	m_rem_bat	= rinfo.GetRemainingBatteryPower();
	m_full_bat	= m_rem_bat+1;
	m_last_bat	= 0;
}
//-------------------------------------------------------------------------------------------------
void CMyBattery::BatConsume(WORD unit)
{
	m_rem_bat -= unit;
}
//-------------------------------------------------------------------------------------------------
DWORD CMyBattery::SendAgent(WORD from,BYTE stock)
{
CMyBatAgent agent;
BYTE		d;
WORD		st_bat_avg = 0;

	//Initialize
	agent.Initialize((CMyStock*)this,m_curent,m_facing,stock);

	for(int i=(from-1);i>=1;i--)
	{
		//Teleport
		if(m_path[i].att==PA_TELE)
		{
			agent.PickSecured();
			agent.TelePort(&m_path[i-1].loc);
		}else{

			d = GetOposit(m_path[i].came_d);

			agent.PickSecured();

			if(d!=agent.m_facing)
			{
				agent.SetFacing(d);
				agent.PickSecured();
			}
			
			//Lets move to the next location
			if(m_path[i].steps!=0)
				st_bat_avg = agent.Move(m_path[i].steps);
		}
	}

	return agent.m_bat+st_bat_avg;
}
