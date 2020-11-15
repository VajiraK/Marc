// MyMap.cpp: implementation of the CMyMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMap.h"

CMyMap::~CMyMap(){}
//---------------------------------------------------------------------------------------------
CMyMap::CMyMap()
{
}
//---------------------------------------------------------------------------------------------
int CMyMap::QuarryArea(CRobot &robot)
{
BYTE d,ret;
int i;
	d = m_facing;

	//Look around
	for(i=0;i<=3;i++)
	{
		//Search for unexplore area
		if(VirScanPic(m_curent,d,MODE_DIRTY)==RET_DIRTY_CELL)
		{//Dirty
			d = GetNextDirection(d);
		}else{
			if(d!=m_facing)
				SetDirection(robot,d);
			return RET_SUCCESS;
		}
	}

//Cannot find unexplore by rotating *****************************************************

	//Make path to unexplore area
	ret = MakeVerPath(robot);
	switch(ret)
	{
	case RET_STOP_MIS:
			return RET_STOP_MIS;
	case ID_TELE:
			return ID_TELE;
	}
		
	//Go there
	for(i=0;i<=m_index_vp-1;i++)
	{
		/*s=i;
		if(FindShortCut(m_path[i].loc,&s))
			i = s;*/

		if(m_path_ver[i].came_d==m_facing)
		{
			if(robot,m_path_ver[i].steps>0)
			{
				ret = DoMove(robot,m_path_ver[i].steps);
				//Beep(50,50);
				if(ret==RET_STOP_MIS)
					return RET_STOP_MIS;	
			}
		}else{
			SetDirection(robot,m_path_ver[i].came_d);
			if(robot,m_path_ver[i].steps>0)
			{
				ret = DoMove(robot,m_path_ver[i].steps);
				// Beep(50,50);
				if(ret==RET_STOP_MIS)
					return RET_STOP_MIS;
			}
		}
	}

	return RET_SUCCESS;
}
//---------------------------------------------------------------------------------------------
int CMyMap::MakeVerPath(CRobot &robot)
{
BYTE		vfacing;
WORD		vstep;
MY_POINT	vloc;
int			i;

	m_index_vp = 0;
	vfacing = m_facing;
	vloc.x	= m_curent.x;
	vloc.y	= m_curent.y;

	for(int ex=0;ex<=100;ex++)
	{
		for(i=0;i<=3;i++)
		{
			//Get free steps
			vstep = VirScanPic(vloc,vfacing,MODE_FREE_ST);
			if(vstep<=0)
			{
				//Turn
				vfacing = GetNextDirection(vfacing);
			}else{
				break;
			}
		}
		
		if(vstep<=0)
		{
			//MessageBox(NULL,"Movw No free sp!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
			return RET_STOP_MIS;
		}

		//Save location
		m_path_ver[m_index_vp].came_d	= vfacing;
		m_path_ver[m_index_vp].steps	= vstep;
		m_path_ver[m_index_vp].loc.x	= vloc.x;
		m_path_ver[m_index_vp].loc.y	= vloc.y;
		m_index_vp++;

		//Go there
		VirMove(&vloc,vfacing,vstep);

		for(i=0;i<=3;i++)
		{
			//Search for unexplore area
			if(VirScanPic(vloc,vfacing,MODE_DIRTY)!=RET_DIRTY_CELL)
			{//Got one
				//Save location
				m_path_ver[m_index_vp].came_d	= vfacing;
				m_path_ver[m_index_vp].steps	= 0;
				m_path_ver[m_index_vp].loc.x	= vloc.x;
				m_path_ver[m_index_vp].loc.y	= vloc.y;
				m_index_vp++;
				return RET_SUCCESS;
			}
			//Turn
			vfacing = GetNextDirection(vfacing);
		}
	}

	//MessageBox(NULL,"No Unexplore!   ","Marc",MB_OK|MB_ICONEXCLAMATION);

	if(m_tele_count!=0)
	{
		m_tele_count--;
		if(GotoLastTele(robot)==RET_STOP_MIS)
			return RET_STOP_MIS;
		return ID_TELE;
	}
	else
	{
		return RET_STOP_MIS;
	}
}
//-------------------------------------------------------------------------------------------
void CMyMap::VirMove(MY_POINT* p,BYTE d,WORD step)
{
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
//---------------------------------------------------------------------------------------
void CMyMap::FillArea(CPicture &picture)
{
BYTE		h;
ObjectType	obt;
MY_POINT	pic;;

	for (pic.x=0;pic.x<=4;pic.x++)
	{
		for (pic.y=0;pic.y<=4;pic.y++)
		{
			obt = picture.GetObjectTypeAt(pic.x,pic.y);
			h	= picture.GetObjectHeightAt(pic.x,pic.y);

			PictureToMap(pic);

			if(m_point.x!=MAP_INVALIED)
			{
				switch (obt) 
				{ 
					case OT_WALL:
						m_map[m_point.x][m_point.y].type = CL_WALL;
						m_map[m_point.x][m_point.y].h	 = h;
					break;
					case OT_DOOR:
						m_map[m_point.x][m_point.y].type = CL_DOOR;	
						m_map[m_point.x][m_point.y].h	 = h;
					break;
					case OT_NONE:
						m_map[m_point.x][m_point.y].type = CL_FREE;	
						m_map[m_point.x][m_point.y].h	 = h;
					break;
					case OT_TELEPORT_BEAM:
						m_map[m_point.x][m_point.y].type = CL_TELE;
						m_map[m_point.x][m_point.y].h	 = h;
					break;
					case OT_BLACK_PACKAGE:
						m_map[m_point.x][m_point.y].type = CL_BLACK;
						m_map[m_point.x][m_point.y].h	 = h;
					break;
					case OT_BATTERY_PACK:
						m_map[m_point.x][m_point.y].type = CL_BATTERY;
						m_map[m_point.x][m_point.y].h	 = h;
					break;
					case OT_YELLOW_PACKAGE:
						m_map[m_point.x][m_point.y].type = CL_YELLOW;
						m_map[m_point.x][m_point.y].h	 = h;
					break;
					case OT_PINK_PACKAGE:
						m_map[m_point.x][m_point.y].type = CL_PINK;
						m_map[m_point.x][m_point.y].h	 = h;
					break;
					case OT_BROWN_PACKAGE:
						m_map[m_point.x][m_point.y].type = CL_BROWN;
						m_map[m_point.x][m_point.y].h	 = h;
					break;	
				}
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
WORD CMyMap::GetFreeSteps()
{
	return VirScanPic(m_curent,m_facing,MODE_FREE_ST);
}
//-------------------------------------------------------------------------------------------------
BYTE CMyMap::GotoLastTele(CRobot &robot)
{
BYTE d;
WORD steps,s;
bool lb = false;
	for(int i=(m_path_i-1);i>=1;i--)
	{
		//Check short cut to last tele
		s=i;
		if(FindShortCut(m_path[i].loc,m_last_tele_des[m_tele_count],&s))
			i = s;
		//Teleport
		if(m_path[i].tele==ID_TELE)
		{
			DoTelePort(robot);
			//Re store direction
			d = GetOposit(m_facing);
			SetDirection(robot,d);
			//Clear path history to last teleported sorce(it will over write by next SaveLoc())
			m_path_i = m_path_i-(m_path_i-i);
			if(lb)
				return RET_STOP_MIS;
			else
				return RET_SUCCESS;
		}else{
	
			d = GetOposit(m_path[i].came_d);

			//Check space
			steps = 0;
			PictureToMap(m_path[i].loc);
			if(m_point.x!=MAP_INVALIED)
				steps = VirScanPic(m_path[i].loc,d,MODE_FREE_ST);

			//Set face
			if(d!=m_facing)
				SetDirection(robot,d);

			if(steps<m_path[i].steps)
			{//No space to go so PassBlacks
				m_curent.x=m_path[i].loc.x;m_curent.y=m_path[i].loc.y;
				PassBlacks(robot,MODE_GOING_HOME,i);
			}
			
			//Lets move to next location (Don't need to save these locations)
			if(DoMove(robot,m_path[i].steps,MODE_NO_SAVE_LOC)==RET_STOP_MIS)
				lb = true;
		}
	}
	return RET_SUCCESS;
}
//----------------------------------------------------------------------------------------------
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
