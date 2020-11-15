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

	m_boder_ord[0].x = 0;	m_boder_ord[0].y = 5;
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
	m_boder_ord[19].x = 5;	m_boder_ord[19].y = 1;
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::IsBeenThere(MY_POINT robo,BYTE d)
{
BYTE dirty = 0;

	for(BYTE i=0;i<25;i++)
	{
		PictureToMap(robo,m_row_ord[i],d);
		if(m_point.x!=MAP_INVALIED)
		{
			if(m_map[m_point.x][m_point.y].dirty==CL_DIRTY)
				dirty++;
		}
	}

	if(dirty==25)
		return true;
	else
		return false;
}
//---------------------------------------------------------------------------------------------------
bool CMyMap::IsPicTaken(MY_POINT robo,BYTE d)
{
	for(BYTE i=0;i<25;i++)
	{
		PictureToMap(robo,m_row_ord[i],d);
		if(m_point.x!=MAP_INVALIED)
		{
			if(m_map[m_point.x][m_point.y].type==CL_UNKNOWN)
				return false;
		}
	}
return true;
}
//---------------------------------------------------------------------------------------------------
BYTE CMyMap::GetFreeSteps(MY_POINT robo,BYTE d)
{
BYTE st = 0;
	for(BYTE i=0;i<25;i++)
	{
		PictureToMap(robo,m_row_ord[i],d);
		if(m_point.x!=MAP_INVALIED)
		{
			if((m_map[m_point.x][m_point.y].type==CL_WALL)||(m_map[m_point.x][m_point.y].type==CL_UNKNOWN)||(m_map[m_point.x][m_point.y].type==CL_DOOR)||(m_map[m_point.x][m_point.y].type==CL_BLACK)||(m_map[m_point.x][m_point.y].type==CL_YELLOW)||(m_map[m_point.x][m_point.y].type==CL_PINK)||(m_map[m_point.x][m_point.y].type==CL_BROWN)||(m_map[m_point.x][m_point.y].type==CL_BATTERY))
				return st;
		}
		st = i/5;
	}
return st+1;
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
//---------------------------------------------------------------------------------------------------
bool CMyMap::IsUseLess(MY_POINT p,BYTE d)
{
bool dbord = true;
	for(BYTE i=0;i<20;i++)
	{
		BorderToMap(p,m_boder_ord[i],d);
		/*if(m_map[m_point.x][m_point.y].dirty!=CL_DIRTY)
		{
			dbord = false;
			break;
		}*/
	}
	if(dbord)
	{//Boder dirty
		for(BYTE i=0;i<25;i++)
		{
			PictureToMap(p,m_row_ord[i],d);
			if((m_map[m_point.x][m_point.y].type==CL_TELE)||(m_map[m_point.x][m_point.y].type==CL_UNKNOWN))
				return false;//Use full area
		}
	}else{
		return false;//Boder not dirty
	}

	return true;//Use less
}
//-------------------------------------------------------------------------------------------------
void CMyMap::BorderToMap(MY_POINT robo,MY_POINT p,BYTE d)
{
int x,y;

	switch (d) 
	{ 
		case FACE_UP:
			x = robo.x + (p.x-2);
			y = robo.y - (6-p.y)-2;
		break;
		case FACE_RIGHT:
			x = robo.x + (6-p.y)+2;
			y = robo.y + (p.x-2);
		break;
		case FACE_DOWN:
			x = robo.x + (p.x-3);
			y = robo.y + (6-p.y)+1;
		break;
		case FACE_LEFT:
			x = robo.x - (6-p.y)-2;
			y = robo.y + (2-p.x);
		break;	
	}
	if((x>=0)&&(y>=0)&&(x<MAP_EX)&&(y<MAP_EX))
	{//In map edges x,y goes to minus
		m_point.x = x;
		m_point.y = y;
		//return true;
	}else{
		m_point.x = MAP_INVALIED;//This value set for ditect that we are out of range
		//return false;
	}
}
//---------------------------------------------------------------------------------------------------
/*void CMyMap::WritePath(char *fname)
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
void CMyMap::WriteMap()
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


