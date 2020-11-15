// MyMap.cpp: implementation of the CMyMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMap.h"

CMyMap::~CMyMap(){}
//------------------------------------------------------------------------------------------------
CMyMap::CMyMap()
{
	//Initialiez map (Is it hard Mr.CPU)
	for (int x=0;x<MAP_EX;x++)
	{
		for (int y=0;y<MAP_EX;y++)
		{
			m_map[x][y].type  = CL_UNKNOWN;
			m_map[x][y].dirty = CL_NOT_DIRTY;
			m_map[x][y].free  = INVALIED;
		}
	}

	//Black list count
	m_blacklist[0].steps = 0;

	//Pic cordinates for scan row vice
BYTE i;
	for(i=0;i>=25;i++)
		m_row_ord.nr[i]	= INVALIED;
	i = 0;
	for(int y=4;y>=0;y--)
	{
		for (BYTE x=0;x<=4;x++)
		{	
			m_row_ord.p[i].x = x;
			m_row_ord.p[i].y = y;
			m_row_ord.st[i]  = 4-y;
			i++;
		}
		m_row_ord.nr[i]	= VALIED;
	}
	m_row_ord.st[i] = 5;
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::IsBeenThere(MY_POINT robo,BYTE d)
{//Check for full pic dirty
BYTE dirty = 0;
MY_POINT map;

	for(BYTE i=0;i<25;i++)
	{
		if(PictureToMap(robo,&m_row_ord.p[i],&map,d))
		{
			if((m_map[map.x][map.y].dirty==CL_DIRTY)||(m_map[map.x][map.y].dirty==CL_BLOCK))
				dirty++;
			else
				return false;
		}
	}

	if(dirty==25)
		return true;

	return false;
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::IsPicTaken(MY_POINT robo,BYTE d)
{
MY_POINT map;
	for(BYTE i=0;i<25;i++)
	{
		if(PictureToMap(robo,&m_row_ord.p[i],&map,d))
		{
			if(m_map[map.x][map.y].type==CL_UNKNOWN)
				return false;
		}
	}
return true;
}
//---------------------------------------------------------------------------------------------------
BYTE CMyMap::GetFreeStepsEx(MY_POINT robo,BYTE d,bool pickable)
{//Full dirty raw is a blocking object
MY_POINT	map;
BYTE		dc,type;

	dc = 0;

	for(BYTE i=0;i<25;i++)
	{
		if(PictureToMap(robo,&m_row_ord.p[i],&map,d))
		{
			type = m_map[map.x][map.y].type;
			//New raw
			if(m_row_ord.nr[i]==VALIED)
				dc = 0;
			//Count dirty cells`
			if((m_map[map.x][map.y].dirty==CL_DIRTY)||(m_map[map.x][map.y].dirty==CL_BLOCK))
				dc++;
			//Full raw is dirty
			if(dc==5)
				return m_row_ord.st[i];
			
			if(pickable)
			{	//Pickables are not bloking objects
				if((type==CL_WALL)||(type==CL_UNKNOWN)||(type==CL_DOOR))
					return m_row_ord.st[i];
			}else{//Pickables are bloking objects
				if((type==CL_WALL)||(type==CL_UNKNOWN)||(type==CL_DOOR)||(type==CL_BLACK)||(type==CL_YELLOW)||(type==CL_PINK)||(type==CL_BROWN)||(type==CL_BATTERY))
					return m_row_ord.st[i];
			}
		}else{
			return m_row_ord.st[i];
		}
	}

return m_row_ord.st[i];
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::GetFeasible(MY_POINT p,BYTE *d)
{// Befor calling d should be set to a direction
BYTE i;
	//Look for fresh tele
	for(i=0;i<=3;i++)
	{
		if(FindFreshTele(p,*d))
			return true;
		*d = GetNextDirection(*d);
	}
	//Look for fresh areas
	for(i=0;i<=3;i++)
	{
		if(m_blacklist[0].steps==0)
		{
			if(IsBeenThere(p,*d)==false)
						return true;
		}else{
			for(BYTE b=0;b<m_blacklist[0].steps;b++)
			{//Go through black list
				if(!((m_blacklist[b].loc.x==m_curent.x)&&(m_blacklist[b].loc.y==m_curent.y)&&(m_blacklist[b].came_d==*d)))
				{//Not in black list
					//Is front area dirty
					if(IsBeenThere(p,*d)==false)
						return true;
					else
						break;
				}
			}
		}
		*d = GetNextDirection(*d);
	}
return false;
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::GetUnknown(MY_POINT p,BYTE *d)
{//Befor calling d should be set to a direction
	//Look around
	for(int i=0;i<=3;i++)
	{
		//Is front area dirty
		if(IsPicTaken(p,*d)==false)
			return true;
		*d = GetNextDirection(*d);
	}
return false;
}
//-------------------------------------------------------------------------------------------------------
bool CMyMap::FindFreshTele(MY_POINT p,BYTE d,MY_POINT *tele)
{
MY_POINT map;
	for(BYTE i=0;i<25;i++)
	{
		if(PictureToMap(p,&m_row_ord.p[i],&map,d))
		{
			if(m_map[map.x][map.y].type==CL_TELE)
			{
				for (int i=0;i<=TELE_MAX;i++)
				{
					if((map.x==m_tele[i].loc.x)&&(map.y==m_tele[i].loc.y))
  						return false;//Dirty teleport
				}
				if(tele!=0)
				{
					tele->x=map.x;tele->y=map.y;
				}
				return true;
			}
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------
void CMyMap::FreeLocation(MY_POINT *map)
{
	m_map[map->x][map->y].type	 = CL_FREE;
	m_map[map->x][map->y].h		 = 0;
	m_map[map->x][map->y].free	 = INVALIED;
}
//-------------------------------------------------------------------------------------------------------
/*BYTE CMyMap::GetDirtyRage()
{//Return steps to Dirty area
MY_POINT map,pic;
BYTE	 dc,st;
	dc = st = 0;
	for (int y=4;y>=0;y--)
	{
		for (int x=0;x<=4;x++)
		{	
			pic.x=x;pic.y=y;
			if(PictureToMapEx(&pic,&map))
			{
				//Count dirty cells
				if((m_map[map.x][map.y].dirty==CL_DIRTY)||(m_map[map.x][map.y].dirty==CL_BLOCK))
					dc++;
			}
		}
		if(dc==5)//Full raw is dirty
			return st;
		dc = 0;
		st++;
	}
	return st;
}*/

