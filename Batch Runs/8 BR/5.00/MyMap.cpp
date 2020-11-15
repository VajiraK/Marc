// MyMap.cpp: implementation of the CMyMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMap.h"

CMyMap::~CMyMap(){}
//------------------------------------------------------------------------------------------------
CMyMap::CMyMap()
{
	m_buds_i = 0;

	//Initialiez map (Is it hard Mr.CPU)
	for (int x=0;x<MAP_EX;x++)
	{
		for (int y=0;y<MAP_EX;y++)
		{
			m_map[x][y].type  = CL_UNKNOWN;
			m_map[x][y].dirty = CL_NOT_DIRTY;
		}
	}
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::IsBeenThere(MY_POINT robo,BYTE d)
{
MY_POINT	p;
BYTE	i = 0;

	for (int y=4;y>=0;y--)
	{
		for (int x=0;x<=4;x++)
		{	
			p.x=x;p.y=y;
			PictureToMap(robo,p,d);
			if(m_point.x!=MAP_INVALIED)
			{
				if(m_map[m_point.x][m_point.y].dirty==CL_DIRTY)
					i++;
			}
		}
	}

	if(i==25)
		return true;
	else
		return false;
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::IsPicTaken(MY_POINT robo,BYTE d)
{
MY_POINT p;

	for (int y=4;y>=0;y--)
	{
		for (int x=0;x<=4;x++)
		{	
			p.x=x;p.y=y;
			PictureToMap(robo,p,d);
			if(m_point.x!=MAP_INVALIED)
			{
				if(m_map[m_point.x][m_point.y].type==CL_UNKNOWN)
					return false;
			}
		}
	}

return true;
}
//---------------------------------------------------------------------------------------------------
BYTE CMyMap::GetFreeSteps(MY_POINT robo,BYTE d)
{
MY_POINT	p;
BYTE		st;

	st = 0;
	for (int y=4;y>=0;y--)
	{
		for (int x=0;x<=4;x++)
		{	
			p.x=x;p.y=y;
			PictureToMap(robo,p,d);
			if(m_point.x!=MAP_INVALIED)
			{
				if((m_map[m_point.x][m_point.y].type==CL_WALL)||(m_map[m_point.x][m_point.y].type==CL_UNKNOWN)||(m_map[m_point.x][m_point.y].type==CL_DOOR)||(m_map[m_point.x][m_point.y].type==CL_BLACK)||(m_map[m_point.x][m_point.y].type==CL_YELLOW)||(m_map[m_point.x][m_point.y].type==CL_PINK)||(m_map[m_point.x][m_point.y].type==CL_BROWN)||(m_map[m_point.x][m_point.y].type==CL_BATTERY))
					return st;
			}
		}
		st++;
	}

return st;
}
//---------------------------------------------------------------------------------------------------
BYTE CMyMap::GetFreeStepsEx(MY_POINT robo,BYTE d,bool pickable)
{//Full dirty raw is a blocking object
MY_POINT	p;
WORD		dc;
BYTE		st;

	dc = st = 0;
	for (int y=4;y>=0;y--)
	{
		for (int x=0;x<=4;x++)
		{	
			p.x=x;p.y=y;
			PictureToMap(robo,p,d);
			if(m_point.x!=MAP_INVALIED)
			{
				if(pickable)
				{//Pickables are not bloking objects
					if((m_map[m_point.x][m_point.y].type==CL_WALL)||(m_map[m_point.x][m_point.y].type==CL_UNKNOWN)||(m_map[m_point.x][m_point.y].type==CL_DOOR))
					{//Definate blocking objects
						return st;
					}
					//Count dirty cells
					if(m_map[m_point.x][m_point.y].dirty==CL_DIRTY)
						dc++;
				}else{
					if((m_map[m_point.x][m_point.y].type==CL_WALL)||(m_map[m_point.x][m_point.y].type==CL_UNKNOWN)||(m_map[m_point.x][m_point.y].type==CL_DOOR)||(m_map[m_point.x][m_point.y].type==CL_BLACK)||(m_map[m_point.x][m_point.y].type==CL_YELLOW)||(m_map[m_point.x][m_point.y].type==CL_PINK)||(m_map[m_point.x][m_point.y].type==CL_BROWN)||(m_map[m_point.x][m_point.y].type==CL_BATTERY))
					{//Definate blocking objects
						return st;
					}
					//Count dirty cells
					if(m_map[m_point.x][m_point.y].dirty==CL_DIRTY)
						dc++;
				}
			}
		}
		if(dc==5)//Full raw is dirty
			return st;
		dc = 0;
		st++;
	}

return st;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyMap::FaceUnexplore(CRobot &robot)
{
MY_POINT p;
BYTE	 d;

	d = m_facing;
	CopyPoint(&p,&m_curent);

	if(GetFresh(p,&d))
	{
		if(d!=m_facing)
			SetDirection(robot,d);
		return RET_SUCCESS;
	}

	return RET_ERROR;
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::GetFresh(MY_POINT p,BYTE *d)
{// Befor calling d should be set to a direction

	//Look around
	for(int i=0;i<=3;i++)
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
{// Befor calling d should be set to a direction

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
//---------------------------------------------------------------------------------------------------
/*void CMyMap::WriteMap()
{
HANDLE hf;
DWORD len;
char	temp[3];	

	hf = CreateFile("c:\\map.txt",GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_FLAG_RANDOM_ACCESS|FILE_ATTRIBUTE_NORMAL,NULL);

	for (int y=0;y<MAP_EX;y++)
	{
		for (int x=0;x<MAP_EX;x++)
		{
			if(m_map[x][y]!=3)
				m_map[x][y] = 4;
			else
				m_map[x][y] = 0;

			_ltoa(m_map[x][y],temp,10);
			WriteFile(hf,temp,1,&len,NULL);
		}
		WriteFile(hf,"\r\n",2,&len,NULL);
	}
	
	CloseHandle(hf);

}*/
//---------------------------------------------------------------------------------------------------
void CMyMap::WritePath(char *fname)
{
HANDLE	hf;
DWORD	len;
char	temp[3];	

	hf = CreateFile(fname,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_FLAG_RANDOM_ACCESS|FILE_ATTRIBUTE_NORMAL,NULL);

	for (int i=0;i<m_path_i;i++)
	{
		_ltoa(i,temp,10);
		WriteFile(hf,temp,2,&len,NULL);
		WriteFile(hf," ",1,&len,NULL);

		_ltoa(m_path[i].loc.x,temp,10);
		WriteFile(hf,temp,2,&len,NULL);

		WriteFile(hf,",",1,&len,NULL);

		_ltoa(m_path[i].loc.y,temp,10);
		WriteFile(hf,temp,2,&len,NULL);

		WriteFile(hf," - ",3,&len,NULL);

		_ltoa(m_path[i].steps,temp,10);
		WriteFile(hf,temp,2,&len,NULL);

		WriteFile(hf,"\r\n",2,&len,NULL);
	}
	
	CloseHandle(hf);
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::IsFeasible(MY_POINT robo,BYTE d)
{
MY_POINT	p;
BYTE		bl;

	for (int y=4;y>=0;y--)
	{
		bl = 0;
		for (int x=0;x<=4;x++)
		{	
			p.x=x;p.y=y;
			PictureToMap(robo,p,d);
			if(m_point.x!=MAP_INVALIED)
			{
				if(m_map[m_point.x][m_point.y].type==CL_WALL)
				{
					bl++;
				}else{
					if((m_map[m_point.x][m_point.y].type==CL_DOOR)||(m_map[m_point.x][m_point.y].type==CL_TELE)||(m_map[m_point.x][m_point.y].type==CL_UNKNOWN))
					{
						return true;
					}
				}
			}
		}
		if(bl==5)
			return false;
	}

	return true;
}
