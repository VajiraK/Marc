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
			m_map[x][y].realbk= INVALIED;
		}
	}
	//Pic cordinates for scan row vice
BYTE i = 0;
	for (int y=4;y>=0;y--)
	{
		for (BYTE x=0;x<=4;x++)
		{	
			m_row_ord[i].x = x;
			m_row_ord[i].y = y;
			i++;
		}
	}

	/*m_boder_ord[0].x = 0;	m_boder_ord[0].y = 5;
	m_boder_ord[1].x = 1;	m_boder_ord[1].y = 5;
	m_boder_ord[2].x = 2;	m_boder_ord[2].y = 5;
	m_boder_ord[3].x = 3;	m_boder_ord[3].y = 5;
	m_boder_ord[4].x = 4;	m_boder_ord[4].y = 5;
	m_boder_ord[5].x = 5;	m_boder_ord[5].y = 5;
	m_boder_ord[6].x = 0;	m_boder_ord[6].y = 4;
	m_boder_ord[7].x = 0;	m_boder_ord[7].y = 3;
	m_boder_ord[8].x = 0;	m_boder_ord[8].y = 2;
	m_boder_ord[9].x = 0;	m_boder_ord[9].y = 1;
	m_boder_ord[10].x = 0;	m_boder_ord[10].y = 0;
	m_boder_ord[11].x = 1;	m_boder_ord[11].y = 0;
	m_boder_ord[12].x = 2;	m_boder_ord[12].y = 0;
	m_boder_ord[13].x = 3;	m_boder_ord[13].y = 0;
	m_boder_ord[14].x = 4;	m_boder_ord[14].y = 0;
	m_boder_ord[15].x = 5;	m_boder_ord[15].y = 0;
	m_boder_ord[16].x = 5;	m_boder_ord[16].y = 4;
	m_boder_ord[17].x = 5;	m_boder_ord[17].y = 3;
	m_boder_ord[18].x = 5;	m_boder_ord[18].y = 2;
	m_boder_ord[19].x = 5;	m_boder_ord[19].y = 1;*/
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::IsBeenThere(MY_POINT robo,BYTE d)
{//Check for full pic dirty
BYTE dirty = 0;
MY_POINT map;

	for(BYTE i=0;i<25;i++)
	{
		//PictureToMap(robo,&m_row_ord[i],&map,d);
		if(PictureToMap(robo,&m_row_ord[i],&map,d))
		{
			if(m_map[map.x][map.y].dirty==CL_DIRTY)
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
		//PictureToMap(robo,&m_row_ord[i],&map,d);
		if(PictureToMap(robo,&m_row_ord[i],&map,d))
		{
			if(m_map[map.x][map.y].type==CL_UNKNOWN)
				return false;
		}
	}
return true;
}
//---------------------------------------------------------------------------------------------------
BYTE CMyMap::GetFreeSteps(MY_POINT robo,BYTE d)
{
MY_POINT map;
bool  b = false;
	for(BYTE i=0;i<25;i++)
	{
		//PictureToMap(robo,&m_row_ord[i],&map,d);
		if(PictureToMap(robo,&m_row_ord[i],&map,d))
		{
			if((m_map[map.x][map.y].type==CL_WALL)||(m_map[map.x][map.y].type==CL_UNKNOWN)||(m_map[map.x][map.y].type==CL_DOOR)||(m_map[map.x][map.y].type==CL_BLACK)||(m_map[map.x][map.y].type==CL_YELLOW)||(m_map[map.x][map.y].type==CL_PINK)||(m_map[map.x][map.y].type==CL_BROWN)||(m_map[map.x][map.y].type==CL_BATTERY))
				b = true;
		}else{
			break;
		}
		if(b)
			break;
	}
return i/5;
}
//---------------------------------------------------------------------------------------------------
BYTE CMyMap::GetFreeStepsEx(MY_POINT robo,BYTE d,bool pickable)
{//Full dirty raw is a blocking object
MY_POINT	pic,map;
WORD		dc;
BYTE		st;

	dc = st = 0;
	for (int y=4;y>=0;y--)
	{
		for (int x=0;x<=4;x++)
		{	
			pic.x=x;pic.y=y;
			//PictureToMap(robo,&pic,&map,d);
			if(PictureToMap(robo,&pic,&map,d))
			{
				if(pickable)
				{//Pickables are not bloking objects
					if((m_map[map.x][map.y].type==CL_WALL)||(m_map[map.x][map.y].type==CL_UNKNOWN)||(m_map[map.x][map.y].type==CL_DOOR))
					{//Definate blocking objects
						return st;
					}
					//Count dirty cells
					if(m_map[map.x][map.y].dirty==CL_DIRTY)
						dc++;
				}else{
					if((m_map[map.x][map.y].type==CL_WALL)||(m_map[map.x][map.y].type==CL_UNKNOWN)||(m_map[map.x][map.y].type==CL_DOOR)||(m_map[map.x][map.y].type==CL_BLACK)||(m_map[map.x][map.y].type==CL_YELLOW)||(m_map[map.x][map.y].type==CL_PINK)||(m_map[map.x][map.y].type==CL_BROWN)||(m_map[map.x][map.y].type==CL_BATTERY))
					{//Definate blocking objects
						return st;
					}
					//Count dirty cells
					if(m_map[map.x][map.y].dirty==CL_DIRTY)
						dc++;
				}
			}else{
				return st;
			}
		}
		if(dc==5)//Full raw is dirty
			return st;
		dc = 0;
		st++;
	}

return st;
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
		//Is front area dirty
		if(IsBeenThere(p,*d)==false)
			return true;
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
		//PictureToMap(p,&m_row_ord[i],&map,d);
		//In map eages  
		if(PictureToMap(p,&m_row_ord[i],&map,d))
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



