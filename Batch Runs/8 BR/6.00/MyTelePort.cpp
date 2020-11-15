// MyTelePort.cpp: implementation of the CMyTelePort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTelePort.h"

CMyTelePort::~CMyTelePort(){}
//------------------------------------------------------------------------------------------
CMyTelePort::CMyTelePort()
{
	//Prepair dirty tele arr (Dity wor for CPU
	for(int i=0;i<=TELE_MAX;i++)
		m_tele[i].dirty = false;

	//m_tele_count = 0;
}
//------------------------------------------------------------------------------------------
BYTE CMyTelePort::TryTeleport(CRobot &robot)
{
	for(BYTE i=0;i<25;i++)
	{
		PictureToMap(m_row_ord[i]);
		//In map eages  
		if(m_point.x!=MAP_INVALIED)
		{
			if(m_map[m_point.x][m_point.y].type==CL_TELE)
			{
				for (int i=0;i<=TELE_MAX;i++)
				{
					if((m_point.x==m_tele[i].loc.x)&&(m_point.y==m_tele[i].loc.y))
  						return RET_ERROR;//Dirty teleport
				}
				return CentreTeleport(robot,m_point);
			}
		}
	}
	return RET_ERROR;
}
//------------------------------------------------------------------------------------------
BYTE CMyTelePort::CentreTeleport(CRobot &robot,MY_POINT p)
{//p is the map location of the teleport
MY_POINT vp;
BYTE	 ystp,xstp,yfrstp,xfrstp;

	vp.x=m_curent.x;vp.y=m_curent.y;

	switch (m_facing) 
	{ 
		case FACE_DOWN:
			yfrstp = GetFreeSteps(m_curent,FACE_DOWN);
			//We have to move this much
			ystp   = p.y-m_curent.y;
			if(yfrstp>=ystp)
			{//Y movement is ok
				PlotLocation(&vp,FACE_DOWN,ystp);
				return FromUpDown(robot,p,vp,ystp,yfrstp);
			}
		break;
		//===============================================

		case FACE_UP:
			yfrstp = GetFreeSteps(m_curent,FACE_UP);
			//We have to move this much
			ystp   = m_curent.y-p.y;
			if(yfrstp>=ystp)
			{//Y movement is ok
				PlotLocation(&vp,FACE_UP,ystp);
				return FromUpDown(robot,p,vp,ystp,yfrstp);
			}
		break;
		//===============================================

		case FACE_LEFT:
			xfrstp = GetFreeSteps(m_curent,FACE_LEFT);
			//We have to move this much
			xstp   = m_curent.x-p.x;
			if(xfrstp>=xstp)
			{//X movement is ok
				PlotLocation(&vp,FACE_LEFT,xstp);
				return FromLeftRight(robot,p,vp,xstp,xfrstp);
			}
			
		break;
		//===============================================

		case FACE_RIGHT:
			xfrstp = GetFreeSteps(m_curent,FACE_RIGHT);
			//We have to move this much
			xstp   = p.x-m_curent.x;
			if(xfrstp>=xstp)
			{//X movement is ok
				PlotLocation(&vp,FACE_RIGHT,xstp);
				return FromLeftRight(robot,p,vp,xstp,xfrstp);
			}
		break;
		//===============================================
	}	
	return RET_ERROR;
}
//------------------------------------------------------------------------------------------
BYTE CMyTelePort::FromUpDown(CRobot &robot,MY_POINT p,MY_POINT vp,BYTE ystp,BYTE yfrstp)
{
BYTE xstp,xfrstp;
	if(p.x==m_curent.x)
	{//No x movement
		if(DoMove(robot,ystp)==RET_STOP_MIS)
			return RET_STOP_MIS;
		return TeleNow(robot);
	}
	if(p.x>m_curent.x)
	{//Right turn
		xfrstp = GetFreeSteps(vp,FACE_RIGHT);
		xstp   = p.x-m_curent.x;
		if(xfrstp>=xstp)
		{//Can go to this TelePort
			if(DoMove(robot,ystp)==RET_STOP_MIS)
				return RET_STOP_MIS;
			SetDirection(robot,FACE_RIGHT);
			if(DoMove(robot,xstp)==RET_STOP_MIS)
				return RET_STOP_MIS;
			return TeleNow(robot);
		}
	}
	if(p.x<m_curent.x)
	{//Left turn
		xfrstp = GetFreeSteps(vp,FACE_LEFT);
		xstp   = m_curent.x-p.x;
		if(xfrstp>=xstp)
		{//Can go to this TelePort
			if(DoMove(robot,ystp)==RET_STOP_MIS)
				return RET_STOP_MIS;
			SetDirection(robot,FACE_LEFT);
			if(DoMove(robot,xstp)==RET_STOP_MIS)
				return RET_STOP_MIS;
			return TeleNow(robot);
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------
BYTE CMyTelePort::FromLeftRight(CRobot &robot,MY_POINT p,MY_POINT vp,BYTE xstp,BYTE xfrstp)
{
BYTE ystp,yfrstp;
	if(p.y==m_curent.y)
	{//No y movement
		if(DoMove(robot,xstp)==RET_STOP_MIS)
			return RET_STOP_MIS;
		return TeleNow(robot);
	}
	if(p.y>m_curent.y)
	{//Down turn
		yfrstp = GetFreeSteps(vp,FACE_DOWN);
		ystp   = p.y-m_curent.y;
		if(xfrstp>=xstp)
		{//Can go to this TelePort
			if(DoMove(robot,xstp)==RET_STOP_MIS)
				return RET_STOP_MIS;
			SetDirection(robot,FACE_DOWN);
			if(DoMove(robot,ystp)==RET_STOP_MIS)
				return RET_STOP_MIS;
			return TeleNow(robot);
		}
	}
	if(p.y<m_curent.y)
	{//Up turn
		yfrstp = GetFreeSteps(vp,FACE_UP);
		ystp   = m_curent.y-p.y;
		if(xfrstp>=xstp)
		{//Can go to this TelePort
			if(DoMove(robot,xstp)==RET_STOP_MIS)
				return RET_STOP_MIS;
			SetDirection(robot,FACE_UP);
			if(DoMove(robot,ystp)==RET_STOP_MIS)
				return RET_STOP_MIS;
			return TeleNow(robot);
		}
	}
	return RET_ERROR;
}
//------------------------------------------------------------------------------------------
BYTE CMyTelePort::TeleNow(CRobot &robot)
{
MY_POINT s;
BYTE	 ret;

	//Save source port
	CopyPoint(&s,&m_curent);

	ret = DoTelePort(robot);

	if(ret==RET_SUCCESS)
	{
		//Set couple to dirty
		DirtyTeleCouple(s,m_curent);
		//Increase tele count(Every thing has to count what a headache)
		//m_tele_count++;
	}

	return ret;
}
//------------------------------------------------------------------------------------------
void CMyTelePort::DirtyTeleCouple(MY_POINT s,MY_POINT d)
{
int i;
	for(i=0;i<=TELE_MAX;i++)
	{
		if(m_tele[i].dirty==false)
		{//The source port
			m_tele[i].dirty = true;
			CopyPoint(&m_tele[i].loc,&s);
			i++;
			break;
		}
	}
	for(i=i;i<=TELE_MAX;i++)
	{
		if(m_tele[i].dirty==false)
		{//The destination port
			m_tele[i].dirty = true;
			CopyPoint(&m_tele[i].loc,&d);
			break;
		}
	}

}
