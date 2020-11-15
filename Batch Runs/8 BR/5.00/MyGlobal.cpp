// MyGlobal.cpp: implementation of the CMyGlobal class.

#include "stdafx.h"
#include "MyGlobal.h"

CMyGlobal::~CMyGlobal(){}
//---------------------------------------------------------------------------------------------
CMyGlobal::CMyGlobal()
{
	m_path_i = 0;
}
//----------------------------------------------------------------------------------------
void CMyGlobal::AddToPath(CRobot &robot,WORD steps,BYTE tele)
{
	//Update current
	PlotLocation(&m_curent,m_facing,steps);
	//Add to path
	CopyPoint(&m_path[m_path_i].loc,&m_curent);
	m_path[m_path_i].came_d = m_facing;
	m_path[m_path_i].steps  = steps;
	m_path[m_path_i].att    = tele;
	m_path_i++;
	//Dirty area
	DirtyArea(m_curent);
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
		MessageBox(NULL,msg,"Marc",MB_OK|MB_ICONEXCLAMATION);
		#endif
		return RET_ERROR;
	}
}
//---------------------------------------------------------------------------------------------
bool CMyGlobal::FindShortCut(MY_POINT p,WORD lower,WORD *upper)
{
	for(int i=lower;i<=*upper-1;i++)
	{
		if((p.x==m_path[i].loc.x) && (p.y==m_path[i].loc.y))
		{
			*upper = i;//Shortcut pos of the path arr (m_path)
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------
void CMyGlobal::PictureToMap(MY_POINT p)
{//Current location to
int x,y;

	switch (m_facing) 
	{ 
		case FACE_UP:
			x = m_curent.x + (p.x-2);
			y = m_curent.y - (5-p.y)-2;
		break;
		case FACE_RIGHT:
			x = m_curent.x + (5-p.y)+2;
			y = m_curent.y + (p.x-2);
		break;
		case FACE_DOWN:
			x = m_curent.x + (2-p.x);
			y = m_curent.y + (5-p.y)+2;
		break;
		case FACE_LEFT:
			x = m_curent.x - (5-p.y)-2;
			y = m_curent.y + (2-p.x);
		break;	
	}

	if((x>=0)&&(y>=0)&&(x<MAP_EX)&&(y<MAP_EX))
	{//In map edges x,y goes to minus
		m_point.x = x;
		m_point.y = y;
		//return true;
	}else{
		//Beep(2000,200);
		m_point.x = MAP_INVALIED;//This value set for ditect that we are out of range
		//return false;
	}
}
//-------------------------------------------------------------------------------------------------
void CMyGlobal::PictureToMap(MY_POINT robo,MY_POINT p,BYTE d)
{
int x,y;

	switch (d) 
	{ 
		case FACE_UP:
			x = robo.x + (p.x-2);
			y = robo.y - (5-p.y)-2;
		break;
		case FACE_RIGHT:
			x = robo.x + (5-p.y)+2;
			y = robo.y + (p.x-2);
		break;
		case FACE_DOWN:
			x = robo.x + (2-p.x);
			y = robo.y + (5-p.y)+2;
		break;
		case FACE_LEFT:
			x = robo.x - (5-p.y)-2;
			y = robo.y + (2-p.x);
		break;	
	}

	if((x>=0)&&(y>=0)&&(x<MAP_EX)&&(y<MAP_EX))
	{//In map edges x,y goes to minus
		m_point.x = x;
		m_point.y = y;
		//return true;
	}else{
		//Beep(2000,200);
		m_point.x = MAP_INVALIED;//This value set for ditect that we are out of range
		//return false;
	}
}
//-------------------------------------------------------------------------------------------------
void CMyGlobal::DirtyArea(MY_POINT loc)
{
WORD x,y,xs,ys;

	x=loc.x-2; y=loc.y-2;

	xs=x+4; ys=y+4;

	for (x=x;x<=xs;x++)
	{
		for (y=loc.y-2;y<=ys;y++)
		{
			m_map[x][y].dirty = CL_DIRTY;
		}
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
//----------------------------------------------------------------------------------------
/*bool CMyGlobal::IsDurtyLoc(WORD steps)
{
	//Predict next location
MY_POINT new_loc;
	//steps--;
	switch (m_facing) 
	{ 
		case FACE_UP:
 			new_loc.y = m_path[m_path_i-1].loc.y - steps;
 			new_loc.x = m_path[m_path_i-1].loc.x;
 		break;
 		case FACE_DOWN:
 			new_loc.y = m_path[m_path_i-1].loc.y + steps;
 			new_loc.x = m_path[m_path_i-1].loc.x;
 		break;
 		case FACE_LEFT:
			new_loc.x = m_path[m_path_i-1].loc.x - steps;
			new_loc.y = m_path[m_path_i-1].loc.y;
		break;
		case FACE_RIGHT:
			new_loc.x = m_path[m_path_i-1].loc.x + steps;
			new_loc.y = m_path[m_path_i-1].loc.y;
		break;
	}

	//Search it in history (Back word)
	for(int i=m_path_i;i>=0;i--)
	{
		if((new_loc.x == m_path[i].loc.x) && (new_loc.y == m_path[i].loc.y))
			return true;
	}
	return false;
}*/