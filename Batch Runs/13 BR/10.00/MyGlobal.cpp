// MyGlobal.cpp: implementation of the CMyGlobal class.

#include "stdafx.h"
#include "MyGlobal.h"

CMyGlobal::~CMyGlobal(){}
//---------------------------------------------------------------------------------------------
CMyGlobal::CMyGlobal()
{
	m_path_i = 0;
	m_sec_pic_loc.count = 0;
}
//--------------------------------------------------------------------------------------------
void CMyGlobal::AddToPath(BYTE steps,BYTE tele,bool dirty)
{
	//Update current
	PlotLocation(&m_curent,m_facing,steps);
	//Add to path
	m_path[m_path_i].loc.x=m_curent.x;m_path[m_path_i].loc.y=m_curent.y;
	m_path[m_path_i].came_d = m_facing;
	m_path[m_path_i].steps  = steps;
	m_path[m_path_i].att    = tele;
	m_path_i++;

	//Dirty area*************************
	if((dirty==true)&&(tele!=PA_TELE))
	{
int x,y,xs,ys;
		x = m_curent.x - 2; 
		y = m_curent.y - 2;
		xs= x + 4; 
		ys= y + 4;
		for (x=x;x<=xs;x++)
		{
			for (y=m_curent.y-2;y<=ys;y++)
			{
				if(m_map[x][y].dirty!=CL_BLOCK)
					m_map[x][y].dirty = CL_DIRTY;
			}
		}
	}
	//Dirty area*************************
}
//---------------------------------------------------------------------------------------------
BYTE CMyGlobal::CheckRet(Response res)
{
	if(res==ACTION_SUCCESSFUL)
	{
		return RET_SUCCESS;
	}else{
		#ifdef _DEBUG
		char msg[200];
		CMarc::ResponseToStr(res,msg,200);
		MessageBox(NULL,msg,"Marc",MB_OK|MB_ICONERROR);
		#endif
		return RET_ERROR;
	}
}
//--------------------------------------------------------------------------------------------
BYTE CMyGlobal::GetOposit(BYTE d)
{
	switch (d) 
	{ 
		case FACE_DOWN:
			return FACE_UP;		
		break;
		case FACE_LEFT:
			return FACE_RIGHT;		
		break;
		case FACE_RIGHT:
			return FACE_LEFT;		
		break;
		case FACE_UP:
			return FACE_DOWN;	
		break;
	}	

	return 0;
}
//--------------------------------------------------------------------------------------------
bool CMyGlobal::PictureToMapEx(MY_POINT *pic,MY_POINT *map)
{//Current location to
int x,y;
	x = y = 0;
	switch (m_facing) 
	{ 
		case FACE_UP:
			x = m_curent.x + (pic->x-2);
			y = m_curent.y - (5-pic->y)-2;
		break;
		case FACE_RIGHT:
			x = m_curent.x + (5-pic->y)+2;
			y = m_curent.y + (pic->x-2);
		break;
		case FACE_DOWN:
			x = m_curent.x + (2-pic->x);
			y = m_curent.y + (5-pic->y)+2;
		break;
		case FACE_LEFT:
			x = m_curent.x - (5-pic->y)-2;
			y = m_curent.y + (2-pic->x);
		break;	
	}

	if((x>=0)&&(y>=0)&&(x<MAP_EX)&&(y<MAP_EX))
	{//In map edges x,y goes to minus
		map->x = x;
		map->y = y;
		return true;
	}else{
		return false;
	}
}
//-------------------------------------------------------------------------------------------------
bool CMyGlobal::PictureToMap(MY_POINT robo,MY_POINT *pic,MY_POINT *map,BYTE d)
{
int x,y;
	x = y = 0;
	switch (d)
	{ 
		case FACE_UP:
			x = robo.x + (pic->x-2);
			y = robo.y - (5-pic->y)-2;
		break;
		case FACE_RIGHT:
			x = robo.x + (5-pic->y)+2;
			y = robo.y + (pic->x-2);
		break;
		case FACE_DOWN:
			x = robo.x + (2-pic->x);
			y = robo.y + (5-pic->y)+2;
		break;
		case FACE_LEFT:
			x = robo.x - (5-pic->y)-2;
			y = robo.y + (2-pic->x);
		break;	
	}
	if((x>=0)&&(y>=0)&&(x<MAP_EX)&&(y<MAP_EX))
	{//In map edges x,y goes to minus
		map->x = x;
		map->y = y;
		return true;
	}else{
		return false;
	}
}
//-------------------------------------------------------------------------------------------------
void CMyGlobal::PlotLocation(MY_POINT* p,BYTE d,WORD step)
{//Calculate next location using a given location
	switch (d)
	{ 
		case FACE_UP:
			p->y = p->y-step;
			break;
		case FACE_RIGHT:
			p->x = p->x+step;
			break;
		case FACE_DOWN:
			p->y = p->y+step;
			break;
		case FACE_LEFT:
			p->x = p->x-step;
			break;	
	}
}
//---------------------------------------------------------------------------------------------------
void CMyGlobal::CopyPoint(MY_POINT *dp,MY_POINT *sp)
{
	dp->x = sp->x;
	dp->y = sp->y;
}
//------------------------------------------------------------------------------------------------
void CMyGlobal::GetMapPoint(MY_POINT *p,MY_ITEM *item)
{
	item->type = m_map[p->x][p->y].type;
	item->h    = m_map[p->x][p->y].h;
	item->realbk = m_map[p->x][p->y].realbk;
}
//---------------------------------------------------------------------------------------------------
BYTE CMyGlobal::GetFreeSteps(MY_POINT robo,BYTE d)
{
MY_POINT map;

	for(BYTE i=0;i<25;i++)
	{
		if(PictureToMap(robo,&m_row_ord.p[i],&map,d))
		{
			if((m_map[map.x][map.y].type==CL_WALL)||(m_map[map.x][map.y].type==CL_UNKNOWN)||(m_map[map.x][map.y].type==CL_DOOR)||(m_map[map.x][map.y].type==CL_BLACK)||(m_map[map.x][map.y].type==CL_YELLOW)||(m_map[map.x][map.y].type==CL_PINK)||(m_map[map.x][map.y].type==CL_BROWN)||(m_map[map.x][map.y].type==CL_BATTERY))
				return m_row_ord.st[i];
		}else{
			return m_row_ord.st[i];
		}
	}

return m_row_ord.st[i];
}


