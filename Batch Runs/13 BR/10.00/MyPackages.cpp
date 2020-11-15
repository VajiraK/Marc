// MyPackages.cpp: implementation of the CMyPackages class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBatAgent.h"
#include "MyPackages.h"

CMyPackages::~CMyPackages(){}
CMyPackages::CMyPackages()
{
	m_my_pick_ord = sizeof(MY_PICK_ORD);
	m_rem_hel = 100;
}
//------------------------------------------------------------------------------------------------
BYTE CMyPackages::PassBlacks(CRobot &robot,BYTE dropd,BYTE bst,BYTE mode,bool *bfound)
{//Pick and drop bihind Black packages
MY_POINT	map;
BYTE		helth,oldcol,ret = RET_ERROR;

	oldcol = GetColorHeight();
	*bfound = false;

	//Check helth
	if(mode==MODE_CHECK_HELTH)
	{
		helth = SendAgent();
		if(m_rem_hel==helth)
		{
			#ifdef _DEBUG
			MessageBox(NULL,"No helth !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
			#endif
			return RET_STOP_MIS;
		}
	}

	for(BYTE i=0;i<25;i++)
	{
		if(PictureToMapEx(&m_row_ord.p[i],&map))
		{
			if(m_map[map.x][map.y].type==CL_BLACK)
			{
				//Check Bat
				/*if(CheckBat(0,GetColorHeight())==RET_STOP_MIS)
					return RET_STOP_MIS;*/
				//Check helth
				if((mode==MODE_CHECK_HELTH)&&(m_map[map.x][map.y].realbk==VALIED))
				{
					if(m_rem_hel==helth)
					{
						#ifdef _DEBUG
						MessageBox(NULL,"No helth !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
						#endif
						return RET_STOP_MIS;
					}
					//Shuld incriment after check(Map "Asterisk_2.mmmf" Bat 125000) 
					helth++;
				}
				*bfound = true;
				ret = PickBlack(m_row_ord.p[i],map,robot,dropd,mode);
				if(ret==RET_STOP_MIS)
				{
					//Is some colors drop in the proccess
					if(oldcol!=GetColorHeight())
						DropBlackPacks(robot,oldcol,dropd);
					return RET_STOP_MIS;
				}
			}
		}

		if((m_row_ord.p[i].y==bst)&&(m_row_ord.p[i].x==4))
			break;
	}

	if(DropBlackPacks(robot,oldcol,dropd)==RET_STOP_MIS)
		return RET_STOP_MIS;

	return ret;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyPackages::SmartPick(CRobot &robot)
{
MY_PICK_ORD po[25];
MY_POINT	map;
BYTE		ret,col_count[3],c,index=0;

	col_count[0] = col_count[1] = col_count[2] = 0;

	for(BYTE i=0;i<25;i++)
	{//Fill MY_PICK_ORD
		if(PictureToMapEx(&m_row_ord.p[i],&map))
		{
			c = m_map[map.x][map.y].type;
			if((c==CL_YELLOW)||(c==CL_PINK)||(c==CL_BROWN))
			{//It's color
				if(IsSecured(map,0,false,0)==false)
				{//Never pick secured one
					switch(c)
					{
					case CL_YELLOW:
						po[index].type = CL_YELLOW;
						po[index].ord  = 20;
						col_count[0]++;
					break;
					case CL_PINK:
						po[index].type = CL_PINK;
						po[index].ord  = 15;
						col_count[1]++;
					break;
					case CL_BROWN:
						po[index].type = CL_BROWN;
						po[index].ord  = 10;
						col_count[2]++;
					}
					po[index].h = m_map[map.x][map.y].h;
					po[index].p_map.x = map.x; po[index].p_map.y = map.y;
					po[index].p_pic.x = m_row_ord.p[i].x; po[index].p_pic.y = m_row_ord.p[i].y;
					index++;
				}
			}else{
			//************** BATTERY *****************
				if(c==CL_BATTERY)
				{
					ret = DoPickUp(robot,m_row_ord.p[i]);
					if(ret==RET_SUCCESS)
					{
						//Recharge
						m_rem_bat += m_map[map.x][map.y].h*100;
						//Clear it from vertual map
						m_map[map.x][map.y].type = CL_FREE; 
						m_map[map.x][map.y].h	 = 0;
						if(m_rem_bat>=m_full_bat)
							m_rem_bat = m_full_bat;
					}
				}
			}//************** BATTERY *****************
		}
	}
	if(index!=0)
	{
		SortToColor(po,index);
		//Yellow
		if(col_count[0]!=0)
			SortToHeight(po,col_count[0]);
		//Pink
		if(col_count[1]!=0)
			SortToHeight(&po[col_count[0]],col_count[1]);
		//Brown
		if(col_count[2]!=0)
			SortToHeight(&po[col_count[0]+col_count[1]],col_count[2]);

		if(PickColor(robot,po,index-1)==RET_STOP_MIS)
			return RET_STOP_MIS;
	}
return RET_SUCCESS;
}
//--------------------------------------------------------------------------------------------
void CMyPackages::SortToColor(MY_PICK_ORD *arr,BYTE count)
{
MY_PICK_ORD	*arr_save;
BYTE		min,i,s,sort[25];
BYTE		j;

	//Save array
	arr_save = new MY_PICK_ORD[count];
	memcpy(arr_save,arr,m_my_pick_ord*count);

	//Sort 
	s = 0;
	for(j=0;j<count;j++)
	{
		min = arr[0].ord;
		sort[s] = 0;
		for(i=1;i<count;i++)
		{
			if(min<arr[i].ord)
			{
				min = arr[i].ord;
				sort[s] = i;
			}
		}
		arr[sort[s]].ord = 0;
		s++;
	}

	//Copy result
	for(i=0;i<count;i++)
	{
		arr[i].p_map.x = arr_save[sort[i]].p_map.x;
		arr[i].p_map.y = arr_save[sort[i]].p_map.y;		
		arr[i].p_pic.x = arr_save[sort[i]].p_pic.x;
		arr[i].p_pic.y = arr_save[sort[i]].p_pic.y;
		arr[i].h    = arr_save[sort[i]].h;
		arr[i].type = arr_save[sort[i]].type;
	}

	delete[] arr_save;
}
//--------------------------------------------------------------------------------------------
void CMyPackages::SortToHeight(MY_PICK_ORD *arr,BYTE count)
{
MY_PICK_ORD	*arr_save;
BYTE		maxh,i,s,sort[25];
BYTE		j;

	//Save array
	arr_save = new MY_PICK_ORD[count];
	memcpy(arr_save,arr,m_my_pick_ord*count);

	//Sort 
	s = 0;
	for(j=0;j<count;j++)
	{
		maxh = arr[0].h;
		sort[s] = 0;
		for(i=1;i<count;i++)
		{
			if(maxh<arr[i].h)
			{
				maxh = arr[i].h;
				sort[s] = i;
			}
		}
		arr[sort[s]].h = 0;
		s++;
	}
	//Copy result
	for(i=0;i<count;i++)
	{
		arr[i].p_map.x = arr_save[sort[i]].p_map.x;
		arr[i].p_map.y = arr_save[sort[i]].p_map.y;
		arr[i].p_pic.x = arr_save[sort[i]].p_pic.x;
		arr[i].p_pic.y = arr_save[sort[i]].p_pic.y;
		arr[i].h    = arr_save[sort[i]].h;
		arr[i].type = arr_save[sort[i]].type;
	}

	delete[] arr_save;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyPackages::SendAgent()
{
CMyBatAgent agent;
BYTE		d,steps;

	//Initialize
	agent.Initialize((CMyStock*)this,m_curent,m_facing,0,&m_sec_pic_loc);

	for(int i=(m_path_i-1);i>=1;i--)
	{
		//Teleport
		if(m_path[i].att==PA_TELE)
		{
			agent.TelePort(&m_path[i-1].loc);
		}else{

			d = GetOposit(m_path[i].came_d);

			steps = GetFreeSteps(m_path[i].loc,d);

			if(d!=agent.m_facing)
				agent.SetFacing(d);

			if(steps<m_path[i].steps)
				agent.PassBlacks();//No space to go so PassBlacks
			
			//Lets move to the next location
			if(m_path[i].steps!=0)
				agent.Move(m_path[i].steps);
		}
	}

	return agent.m_helth;
}