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
void CMyGlobal::SaveLoc(CRobot &robot,WORD steps,BYTE tele)
{
CLocation loc;

	robot.GetLocation(loc);
	m_path[m_path_i].loc.x  = (WORD)loc.GetX();
	m_path[m_path_i].loc.y  = (WORD)loc.GetY();
	m_curent.x			    = m_path[m_path_i].loc.x;
	m_curent.y			    = m_path[m_path_i].loc.y;
	m_path[m_path_i].came_d = m_facing;
	m_path[m_path_i].steps  = steps;
	//Teleported locations
	m_path[m_path_i].tele = tele;
	m_path_i++;
	//Battory for every thing
	m_rem_bat--;//m_rem_bat is access thruogh BatConsume() in other locations
}
//---------------------------------------------------------------------------------------------
BYTE CMyGlobal::CheckRet(Response res)
{
	if(res==ACTION_SUCCESSFUL)
	{
		return RET_SUCCESS;
	}else{
		/*char msg[200];
		CMarc::ResponseToStr(res,msg,200);
		MessageBox(NULL,msg,"Marc",MB_OK|MB_ICONEXCLAMATION);*/
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
//-------------------------------------------------------------------------------------------
WORD CMyGlobal::VirScanPic(MY_POINT p,BYTE d,BYTE mode)
{// p is robo centre
int	ys,xs,r,step,dirty,frstp,x,y;
BYTE t;
	step = dirty = frstp = 0; x = p.x; y = p.y;

	switch (d)
	{	case FACE_UP:
			x = x-2;y = y-3;r = x;xs = x+4;ys = y-4;
			for (y=y ; y>=ys ; y--)
			{for (x=x ; x<=xs ; x++){if((x>=0)&&(y>=0)){
					//****************************
					switch (mode) 
					{ 
					case MODE_DIRTY:
						if(m_map[x][y].type==CL_UNKNOWN)
							dirty++;
					break;
					case MODE_FREE_ST:
						frstp++;
						if((m_map[x][y].type==CL_WALL)||(m_map[x][y].type==CL_UNKNOWN)||(m_map[x][y].type==CL_DOOR)||(m_map[x][y].type==CL_BLACK)||(m_map[x][y].type==CL_YELLOW)||(m_map[x][y].type==CL_PINK)||(m_map[x][y].type==CL_BROWN))
							return step;
					break;
					case MODE_UN_ST:
						if(m_map[x][y].type==CL_UNKNOWN)
							return step+1;
					break;
					case MODE_DOOR:
						if(step==0)
						{
							if(m_map[x][y].type==CL_DOOR)
								return RET_DOOR_CELL;
						}else{
							return 0;
						}
					break;
					case MODE_BLACK:
						frstp++;
						if((m_map[x][y].type==CL_WALL)||(m_map[x][y].type==CL_UNKNOWN)||(m_map[x][y].type==CL_DOOR))
							return step;
					break;
					}
					//****************************
				}}
				x = r;
				step++;
			}
			break;
		case FACE_RIGHT:
			y = y-2;x = x+3;r = y;xs = x+4;ys = y+4;
			for (x=x ; x<=xs ; x++)
			{for (y=y ; y<=ys ; y++){if((x>=0)&&(y>=0)){
					//****************************
					switch (mode) 
					{ 
					case MODE_DIRTY:
						if(m_map[x][y].type==CL_UNKNOWN)
							dirty++;
					break;
					case MODE_FREE_ST:
						frstp++;
						if((m_map[x][y].type==CL_WALL)||(m_map[x][y].type==CL_UNKNOWN)||(m_map[x][y].type==CL_DOOR)||(m_map[x][y].type==CL_BLACK)||(m_map[x][y].type==CL_YELLOW)||(m_map[x][y].type==CL_PINK)||(m_map[x][y].type==CL_BROWN))
							return step;
					break;
					case MODE_UN_ST:
						if(m_map[x][y].type==CL_UNKNOWN)
							return step+1;
					break;
					case MODE_DOOR:
						if(step==0)
						{
							if(m_map[x][y].type==CL_DOOR)
								return RET_DOOR_CELL;
						}else{
							return 0;
						}
					break;
					case MODE_BLACK:
						frstp++;
						if((m_map[x][y].type==CL_WALL)||(m_map[x][y].type==CL_UNKNOWN)||(m_map[x][y].type==CL_DOOR))
							return step;
					break;
					}
					//****************************
				}}
				y=r;
				step++;
			}
			break;
		case FACE_DOWN:
			y = y+3;x = x-2;r = x;xs = x+4;ys = y+4;
			for (y=y ; y<=ys ; y++){
			for (x=x ; x<=xs ; x++){if((x>=0)&&(y>=0)){
					//I****************************
					switch (mode) 
					{ 
					case MODE_DIRTY:
						if(m_map[x][y].type==CL_UNKNOWN)
							dirty++;
					break;
					case MODE_FREE_ST:
						frstp++;
						t = m_map[x][y].type;
						if((m_map[x][y].type==CL_WALL)||(m_map[x][y].type==CL_UNKNOWN)||(m_map[x][y].type==CL_DOOR)||(m_map[x][y].type==CL_BLACK)||(m_map[x][y].type==CL_YELLOW)||(m_map[x][y].type==CL_PINK)||(m_map[x][y].type==CL_BROWN))
							return step;
					break;
					case MODE_UN_ST:
						if(m_map[x][y].type==CL_UNKNOWN)
							return step+1;
					break;
					case MODE_DOOR:
						if(step==0)
						{
							if(m_map[x][y].type==CL_DOOR)
								return RET_DOOR_CELL;
						}else{
							return 0;
						}
					break;
					case MODE_BLACK:
						frstp++;
						if((m_map[x][y].type==CL_WALL)||(m_map[x][y].type==CL_UNKNOWN)||(m_map[x][y].type==CL_DOOR))
							return step;
					break;
					}
					//****************************
				}}
				x = r;
				step++;
			}
			break;
		case FACE_LEFT:y = y+2;x = x-3;r = y;xs = x-4;ys = y-4;
			for (x=x ; x>=xs ; x--)
			{for (y=y ; y>=ys ; y--){if((x>=0)&&(y>=0)){
					//I****************************
					switch (mode) 
					{ 
					case MODE_DIRTY:
						if(m_map[x][y].type==CL_UNKNOWN)
							dirty++;
					break;
					case MODE_FREE_ST:
						frstp++;
						if((m_map[x][y].type==CL_WALL)||(m_map[x][y].type==CL_UNKNOWN)||(m_map[x][y].type==CL_DOOR)||(m_map[x][y].type==CL_BLACK)||(m_map[x][y].type==CL_YELLOW)||(m_map[x][y].type==CL_PINK)||(m_map[x][y].type==CL_BROWN))
							return step;
					break;
					case MODE_UN_ST:
						if(m_map[x][y].type==CL_UNKNOWN)
							return step+1;
					break;
					case MODE_DOOR:
						if(step==0)
						{
							if(m_map[x][y].type==CL_DOOR)
								return RET_DOOR_CELL;
						}else{
							return 0;
						}
					break;
					case MODE_BLACK:
						frstp++;
						if((m_map[x][y].type==CL_WALL)||(m_map[x][y].type==CL_UNKNOWN)||(m_map[x][y].type==CL_DOOR))
							return step;
					break;
					}
					//****************************
				}}
				y = r;
				step++;
			}
			break;	
	}

	if(mode==MODE_DIRTY)
	{
		if(dirty==0)
			return RET_DIRTY_CELL;
	}
	if((mode==MODE_FREE_ST)||(mode==MODE_BLACK))
	{
		if(frstp==25)
			return 5;
	}
	return 0;
}
//---------------------------------------------------------------------------------------
void CMyGlobal::PictureToMap(MY_POINT p)
{//Cordinate convertion (Oh... real headache)
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

	if((x>=0)&&(y>=0))
	{//In map edges x,y goes to minus
		m_point.x = x;
		m_point.y = y;
		//return true;
	}else{
		m_point.x = MAP_INVALIED;//This value set for ditect that we are out of range
		//return false;
	}
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