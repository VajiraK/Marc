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

	m_tele_count = 0;
}
//------------------------------------------------------------------------------------------
BYTE CMyTelePort::TryTeleport(CRobot &robot)
{
	MY_POINT p;
	int x,y;

	for (y=4;y>=0;y--)
	{for (x=0;x<=4;x++)
	{	
		p.x=x;p.y=y;
		PictureToMap(p);
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
	}}
	return RET_ERROR;
}
//------------------------------------------------------------------------------------------
BYTE CMyTelePort::CentreTeleport(CRobot &robot,MY_POINT p)
{//p is the map location of the teleport
WORD ystp,xstp,yfrstp,xfrstp;
MY_POINT	vp;

	vp.x=m_curent.x;vp.y=m_curent.y;

	switch (m_facing) 
	{ 
		case FACE_DOWN:
			yfrstp = VirScanPic(m_curent,FACE_DOWN,MODE_FREE_ST);
			//We have to move this much
			ystp   = p.y-m_curent.y;
			if(yfrstp>=ystp)
			{//Y movement is ok
				VirMove(&vp,FACE_DOWN,ystp);
				return FromUpDown(robot,p,vp,ystp,yfrstp);
			}
		break;
		//===============================================

		case FACE_UP:
			yfrstp = VirScanPic(m_curent,FACE_UP,MODE_FREE_ST);
			//We have to move this much
			ystp   = m_curent.y-p.y;
			if(yfrstp>=ystp)
			{//Y movement is ok
				VirMove(&vp,FACE_UP,ystp);
				return FromUpDown(robot,p,vp,ystp,yfrstp);
			}
		break;
		//===============================================

		case FACE_LEFT:
			xfrstp = VirScanPic(m_curent,FACE_LEFT,MODE_FREE_ST);
			//We have to move this much
			xstp   = m_curent.x-p.x;
			if(xfrstp>=xstp)
			{//X movement is ok
				VirMove(&vp,FACE_LEFT,xstp);
				return FromLeftRight(robot,p,vp,xstp,xfrstp);
			}
			
		break;
		//===============================================

		case FACE_RIGHT:
			xfrstp = VirScanPic(m_curent,FACE_RIGHT,MODE_FREE_ST);
			//We have to move this much
			xstp   = p.x-m_curent.x;
			if(xfrstp>=xstp)
			{//X movement is ok
				VirMove(&vp,FACE_RIGHT,xstp);
				return FromLeftRight(robot,p,vp,xstp,xfrstp);
			}
		break;
		//===============================================
	}	
	return RET_ERROR;
}
//------------------------------------------------------------------------------------------
BYTE CMyTelePort::FromUpDown(CRobot &robot,MY_POINT p,MY_POINT vp,WORD ystp,WORD yfrstp)
{
WORD xstp,xfrstp;
	if(p.x==m_curent.x)
	{//No x movement
		if(DoMove(robot,ystp)==RET_STOP_MIS)
			return RET_STOP_MIS;
		return TeleNow(robot);
	}
	if(p.x>m_curent.x)
	{//Right turn
		xfrstp = VirScanPic(vp,FACE_RIGHT,MODE_FREE_ST);
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
		xfrstp = VirScanPic(vp,FACE_LEFT,MODE_FREE_ST);
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
BYTE CMyTelePort::FromLeftRight(CRobot &robot,MY_POINT p,MY_POINT vp,WORD xstp,WORD xfrstp)
{
WORD ystp,yfrstp;
	if(p.y==m_curent.y)
	{//No y movement
		if(DoMove(robot,xstp)==RET_STOP_MIS)
			return RET_STOP_MIS;
		return TeleNow(robot);
	}
	if(p.y>m_curent.y)
	{//Down turn
		yfrstp = VirScanPic(vp,FACE_DOWN,MODE_FREE_ST);
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
		xfrstp = VirScanPic(vp,FACE_UP,MODE_FREE_ST);
		xstp   = m_curent.x-p.x;
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
BYTE ret;
MY_POINT s;
	//Save source port
	s.x  = m_curent.x;
	s.y  = m_curent.y;

	ret = DoTelePort(robot);

	if(ret==RET_SUCCESS)
	{
	CLocation loc;
	MY_POINT	d;
		robot.GetLocation(loc);
		BatConsume(1);//for GetLocation()
		d.x  = (WORD)loc.GetX();
		d.y  = (WORD)loc.GetY();
		//Couple is dirty
		DirtyTeleCouple(s,d);
		//use by GotoLastTele()
		m_last_tele_des[m_tele_count] = m_path_i-1;
		//Increase tele count(Every thing has to conut what a headache)
		m_tele_count++;
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
			m_tele[i].loc.x = s.x;
			m_tele[i].loc.y = s.y;
			i++;
			break;
		}
	}

	for(i=i;i<=TELE_MAX;i++)
	{
		if(m_tele[i].dirty==false)
		{//The destination port
			m_tele[i].dirty = true;
			m_tele[i].loc.x = d.x;
			m_tele[i].loc.y = d.y;
			break;
		}
	}

}
