// MyBattery.cpp: implementation of the CMyBattery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBattery.h"

CMyBattery::CMyBattery(){}
CMyBattery::~CMyBattery(){}
//------------------------------------------------------------------------------------------
DWORD CMyBattery::CheckBat(WORD new_st,WORD stock)
{
int nb,bs,nt;

	nt = 0;

	bs = GetBackSteps(m_path_i,&nt);
	
	nb = 1 + ((10+(stock*2)) * (bs-m_bat_boom.boom[m_bat_boom.i-1].st_srt)) + (nt*6) + bs + nt + m_bat_passblack + GetBoomLost(stock);

	if((m_rem_bat)<=nb+(0.3*nb))
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
DWORD CMyBattery::CheckBatEx(WORD upper,WORD new_st,WORD stock)
{
int nb,bs,nt;

	nt = 0;

	bs = GetBackSteps(upper,&nt);
	
	nb = 1 + ((10+(stock*2)) * (bs-m_bat_boom.boom[m_bat_boom.i-1].st_srt)) + (nt*6) + bs + nt + m_bat_passblack + GetBoomLost(stock);

	if((m_rem_bat)<=nb+(0.3*nb))
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
	m_full_bat	= m_rem_bat;
	m_bat_passblack = 0;

	//Bat booms
	m_bat_boom.i = 0;
	for(BYTE i=0;i<BAT_BOOM_MAX;i++)
		m_bat_boom.boom[i].valied = INVALIED;
}
//-------------------------------------------------------------------------------------------------
void CMyBattery::BatConsume(WORD unit)
{
	m_rem_bat -= unit;
}
//-------------------------------------------------------------------------------------------------
void CMyBattery::CreateBatBoom(BYTE dh,BYTE id)
{
int bs,nt;

	bs = GetBackSteps(m_path_i,&nt);

	//Boom
	m_bat_boom.boom[id].h	= dh;
	m_bat_boom.boom[id].valied   = VALIED;
	//Steps********
	m_bat_boom.boom[id].st_srt	= bs;
	if(id!=0)
	{
		#ifdef _DEBUG
		if(bs<m_bat_boom.boom[id-1].st_srt)
			MessageBox(NULL,"Step topple! ","Marc",MB_OK|MB_ICONEXCLAMATION);
		#endif
		m_bat_boom.boom[id].st_nxt_b	= bs-m_bat_boom.boom[id-1].st_srt;
	}else{
		m_bat_boom.boom[id].st_nxt_b	= bs;
	}
	//Steps********
	m_bat_boom.i++;
}
//----------------------------------------------------------------------------------------------------
void CMyBattery::UpdateBatBoom(BYTE id,BYTE reduceh)
{
	m_bat_boom.boom[id].h -= reduceh;

	if(m_bat_boom.boom[id].h==0)
	{
		m_bat_boom.boom[id].valied = INVALIED;
		m_bat_boom.i--;
	}
}
//------------------------------------------------------------------------------------------------
DWORD CMyBattery::GetBoomLost(WORD stock)
{
DWORD bl = 0;
WORD  ch;

	ch = stock;//Current stock

	for(int i=(m_bat_boom.i-1);i>=0;i--)
	{
		if(m_bat_boom.boom[i].valied==VALIED)
		{
			//Cumulative height to the current boom from top
			ch += m_bat_boom.boom[i].h;
			bl += 1 + ((10+(ch*2)) * m_bat_boom.boom[i].st_nxt_b);
		}
	}

	return bl;
}
//------------------------------------------------------------------------------------------------
BYTE CMyBattery::GetBoomID()
{
/*BYTE i;
	//Select available one
	for(i=0;i<BAT_BOOM_MAX;i++)
	{
		if(m_bat_boom.boom[i].valied==INVALIED)
			break;
	}*/

	return m_bat_boom.i;
}
