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
MY_POINT p,pl,pr;
//CORDINATE MAPPINGS ARE TRICKY ALTER CAREFULLY
	//Right
	p.x = 4;p.y = 4;
	PictureToMap(p);
	CopyPoint(&pr,&m_point);
	//Left
	p.x = 0;p.y = 4;
	PictureToMap(p);
	CopyPoint(&pl,&m_point);

	if((m_map[pr.x][pr.y].type==CL_FREE)&&(m_map[pl.x][pl.y].type==CL_WALL))
	{//Left wall right empty
			CopyPoint(&m_gut_p,&pr);//For CodSerch()
       		return QuaryGutter(robot,p,RIGHT_EMPTY);
	}else{
		if((m_map[pl.x][pl.y].type==CL_FREE)&&(m_map[pr.x][pr.y].type==CL_WALL))
		{//Right wall left empty
			CopyPoint(&m_gut_p,&pl);//For CodSerch()
			return QuaryGutter(robot,p,LEFT_EMPTY);
		}
	}

	return RET_ERROR;
}
//---------------------------------------------------------------------------------------------
BYTE CMyGutter::QuaryGutter(CRobot &robot,MY_POINT p,BYTE stat)
{
BYTE pl,mi;
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
					if(GetFreeSteps(m_curent,FACE_UP)>=pl)
					{//There's pace to move
						SetDirection(robot,FACE_UP);
						if(DoMove(robot,pl)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_LEFT);
						return RET_SUCCESS;
					}
				}
			}else{
				mi = CodSearch(MINUS_Y);//*
				pl = CodSearch(PLUS_Y);
				pl = mi+pl-1;
				if(pl>=5)
				{
					mi=5-mi;
					if(GetFreeSteps(m_curent,FACE_DOWN)>=mi)
					{//There's pace to move
						SetDirection(robot,FACE_DOWN);
						if(DoMove(robot,mi)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_LEFT);
						return RET_SUCCESS;
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
					if(GetFreeSteps(m_curent,FACE_DOWN)>=mi)
					{//There's pace to move
						SetDirection(robot,FACE_DOWN);
						if(DoMove(robot,mi)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_RIGHT);
						return RET_SUCCESS;
					}
				}
			}else{
				mi = CodSearch(MINUS_Y);//*
				pl = CodSearch(PLUS_Y);
				mi = mi+pl-1;
				if(mi>=5)
				{
					pl=5-pl;
					if(GetFreeSteps(m_curent,FACE_UP)>=pl)
					{//There's pace to move
						SetDirection(robot,FACE_UP);
						if(DoMove(robot,pl)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_RIGHT);
						return RET_SUCCESS;
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
					if(GetFreeSteps(m_curent,FACE_RIGHT)>=mi)
					{//There's pace to move
						SetDirection(robot,FACE_RIGHT);
						if(DoMove(robot,mi)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_UP);
						return RET_SUCCESS;
					}
				}
			}else{
				mi = CodSearch(MINUS_X);//*
				pl = CodSearch(PLUS_X);
				mi = mi+pl-1;
				if(mi>=5)
				{
					pl=5-pl;
					if(GetFreeSteps(m_curent,FACE_LEFT)>=pl)
					{//There's pace to move
						SetDirection(robot,FACE_LEFT);
						if(DoMove(robot,pl)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_UP);
						return RET_SUCCESS;
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
					if(GetFreeSteps(m_curent,FACE_LEFT)>=pl)
					{//There's pace to move
						SetDirection(robot,FACE_LEFT);
						if(DoMove(robot,pl)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_DOWN);
						return RET_SUCCESS;
					}
				}
			}else{
				mi = CodSearch(MINUS_X);//*
				pl = CodSearch(PLUS_X);
				pl = mi+pl-1;
				if(pl>=5)
				{
					mi=5-mi;
					if(GetFreeSteps(m_curent,FACE_RIGHT)>=mi)
					{//There's pace to move
						SetDirection(robot,FACE_RIGHT);
						if(DoMove(robot,mi)==RET_STOP_MIS)
							return RET_STOP_MIS;
						SetDirection(robot,FACE_DOWN);
						return RET_SUCCESS;
					}
				}
			}

			break;//==========================================
	}
	return RET_ERROR;
}
//--------------------------------------------------------------------------------------------
BYTE CMyGutter::CodSearch(BYTE mode)
{//Befor using m_map must be map by PictureToMap()
WORD  y,ys,x,xs;
BYTE  i=0;

	switch (mode) 
	{ 
		case PLUS_Y:
		y = m_gut_p.y;ys=m_gut_p.y+4;
			for(y=y;y<=ys;y++)
			{
				if(m_map[m_gut_p.x][y].type==CL_FREE)
					i++;
				else
					break;
			}
		break;//===============================
		case MINUS_Y:
		y = m_gut_p.y;ys=m_gut_p.y-4;
			for(y=y;y>=ys;y--)
			{
				if(m_map[m_gut_p.x][y].type==CL_FREE)
					i++;
				else
					break;
			}

		break;//===============================
		case PLUS_X:
		x = m_gut_p.x;xs=m_gut_p.x+4;
			for(x=x;x<=xs;x++)
			{
				if(m_map[x][m_gut_p.y].type==CL_FREE)
					i++;
				else
					break;
			}
		break;//===============================
		case MINUS_X:
		x = m_gut_p.x;xs=m_gut_p.x-4;
			for(x=x;x>=xs;x--)
			{
				if(m_map[x][m_gut_p.y].type==CL_FREE)
					i++;
				else
					break;
			}
		break;//===============================
	}	
	return i;
}
