// MyGutter.cpp: implementation of the CMyGutter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyGutter.h"

CMyGutter::~CMyGutter(){}
CMyGutter::CMyGutter(){}
//-----------------------------------------------------------------------------------------
BYTE CMyGutter::FindGutter(CRobot &robot)
{
MY_POINT p;//Picture cordinates
	p.x = 4;p.y = 4;
	PictureToMap(p);

	if(m_map[m_point.x][m_point.y].type==CL_FREE)
	{
  		return QuaryGutter(robot,p,RIGHT_EMPTY);
	}else{
		p.x = 0;p.y = 4;
		PictureToMap(p);
		if(m_map[m_point.x][m_point.y].type==CL_FREE)
		{
			return QuaryGutter(robot,p,LEFT_EMPTY);
		}
	}

	return RET_SUCCESS;
}
//---------------------------------------------------------------------------------------------
BYTE CMyGutter::QuaryGutter(CRobot &robot,MY_POINT p,BYTE stat)
{
WORD pl,mi;
	switch (m_facing) 
	{ 
		case FACE_LEFT:
			if(stat==RIGHT_EMPTY)
			{
				mi = CodSearch(MINUS_Y);//*
				pl = CodSearch(PLUS_Y);
				mi = mi+pl-1;
				if(mi>=5)
				{
					pl=5-pl;
					if(VirScanPic(m_curent,FACE_UP,MODE_FREE_ST)>=pl)
					{//There's pace to move
						SetDirection(robot,FACE_UP);
						if(DoMove(robot,pl)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_LEFT);
					}
				}
			}else{
				mi = CodSearch(MINUS_Y);//*
				pl = CodSearch(PLUS_Y);
				pl = mi+pl-1;
				if(pl>=5)
				{
					mi=5-mi;
					if(VirScanPic(m_curent,FACE_DOWN,MODE_FREE_ST)>=mi)
					{//There's pace to move
						SetDirection(robot,FACE_DOWN);
						if(DoMove(robot,mi)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_LEFT);
					}
				}
			}
			break;//==========================================

		case FACE_RIGHT:
			if(stat==RIGHT_EMPTY)
			{
				mi = CodSearch(MINUS_Y);//*
				pl = CodSearch(PLUS_Y);
				pl = pl+mi-1;
				if(pl>=5)
				{
					mi=5-mi;
					if(VirScanPic(m_curent,FACE_DOWN,MODE_FREE_ST)>=mi)
					{//There's pace to move
						SetDirection(robot,FACE_DOWN);
						if(DoMove(robot,mi)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_RIGHT);
					}
				}
			}else{
				mi = CodSearch(MINUS_Y);//*
				pl = CodSearch(PLUS_Y);
				mi = mi+pl-1;
				if(mi>=5)
				{
					pl=5-pl;
					if(VirScanPic(m_curent,FACE_UP,MODE_FREE_ST)>=pl)
					{//There's pace to move
						SetDirection(robot,FACE_UP);
						if(DoMove(robot,pl)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_RIGHT);
					}
				}
			}

			break;//==========================================

		case FACE_UP:
			if(stat==RIGHT_EMPTY)
			{
				mi = CodSearch(MINUS_X);//*
				pl = CodSearch(PLUS_X);
				pl = pl+mi-1;
				if(pl>=5)
				{
					mi=5-mi;
					if(VirScanPic(m_curent,FACE_RIGHT,MODE_FREE_ST)>=mi)
					{//There's pace to move
						SetDirection(robot,FACE_RIGHT);
						if(DoMove(robot,mi)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_UP);
					}
				}
			}else{
				mi = CodSearch(MINUS_X);//*
				pl = CodSearch(PLUS_X);
				mi = mi+pl-1;
				if(mi>=5)
				{
					pl=5-pl;
					if(VirScanPic(m_curent,FACE_LEFT,MODE_FREE_ST)>=pl)
					{//There's pace to move
						SetDirection(robot,FACE_LEFT);
						if(DoMove(robot,pl)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_UP);
					}
				}
			}
			break;//==========================================

		case FACE_DOWN:
			if(stat==RIGHT_EMPTY)
			{
				mi = CodSearch(MINUS_X);//*
				pl = CodSearch(PLUS_X);
				mi = mi+pl-1;
				if(mi>=5)
				{
					pl=5-pl;
					if(VirScanPic(m_curent,FACE_LEFT,MODE_FREE_ST)>=pl)
					{//There's pace to move
						SetDirection(robot,FACE_LEFT);
						if(DoMove(robot,pl)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_DOWN);
					}
				}
			}else{
				mi = CodSearch(MINUS_X);//*
				pl = CodSearch(PLUS_X);
				pl = mi+pl-1;
				if(pl>=5)
				{
					mi=5-mi;
					if(VirScanPic(m_curent,FACE_RIGHT,MODE_FREE_ST)>=mi)
					{//There's pace to move
						SetDirection(robot,FACE_RIGHT);
						if(DoMove(robot,mi)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_DOWN);
					}
				}
			}

			break;//==========================================
	}
	return RET_SUCCESS;
}
//--------------------------------------------------------------------------------------------
BYTE CMyGutter::CodSearch(BYTE mode)
{//Befor using m_map must be map by PictureToMap()
WORD y,ys;
WORD x,xs;
BYTE  i=0;

	switch (mode) 
	{ 
		case PLUS_Y:
		y = m_point.y;ys=m_point.y+4;
			for(y=y;y<=ys;y++)
			{
				if(m_map[m_point.x][y].type==CL_FREE)
					i++;
				else
					break;
			}
		break;//===============================
		case MINUS_Y:
		y = m_point.y;ys=m_point.y-4;
			for(y=y;y>=ys;y--)
			{
				if(m_map[m_point.x][y].type==CL_FREE)
					i++;
				else
					break;
			}

		break;//===============================
		case PLUS_X:
		x = m_point.x;xs=m_point.x+4;
			for(x=x;x<=xs;x++)
			{
				if(m_map[x][m_point.y].type==CL_FREE)
					i++;
				else
					break;
			}
		break;//===============================
		case MINUS_X:
		x = m_point.x;xs=m_point.x-4;
			for(x=x;x>=xs;x--)
			{
				if(m_map[x][m_point.y].type==CL_FREE)
					i++;
				else
					break;
			}
		break;//===============================
	}	
	return i;
}
