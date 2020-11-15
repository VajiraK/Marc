// MyPackages.cpp: implementation of the CMyPackages class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyPackages.h"

CMyPackages::~CMyPackages(){}
CMyPackages::CMyPackages()
{
	m_hel_marg = 100;
}
//------------------------------------------------------------------------------------------------
BYTE CMyPackages::PassBlacks(CRobot &robot,BYTE dropd,BYTE bst,BYTE mode)
{//Pick and drop bihind Black packages
BYTE	oldcol,ret = RET_ERROR;

	//Check helth
	if(m_hel_marg<HELTH_MARGIN)
	{
		if(mode==MODE_CHECK_HELTH)
		{
			#ifdef _DEBUG
			MessageBox(NULL,"No helth !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
			#endif
			return RET_STOP_MIS;
		}
	}

	oldcol = GetColorCount();

	for(BYTE i=0;i<25;i++)
	{
		ret = PickBlack(m_row_ord[i],robot,dropd,mode);
		if(ret==RET_STOP_MIS)
			return RET_STOP_MIS;
		if((m_row_ord[i].y==bst)&&(m_row_ord[i].x==4))
			break;
	}

 	if(IsBlackOver()==false)
	{
		if(DropBlackPacks(robot,oldcol,dropd)==RET_STOP_MIS)
			return RET_STOP_MIS;
		return ret;
	}

	return ret;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyPackages::CollectColor(CRobot &robot,BYTE mode)
{
	switch(mode)
	{
	case MODE_COLE_FRESH:
		return SmartPick(robot);
		break;
	case MODE_COLE_SECURE:
		return PickSecured(robot);
		break;
	}

	return RET_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyPackages::SmartPick(CRobot &robot)
{
MY_PICK_ORD po[25];
BYTE		ret,c,index=0;

	for(BYTE i=0;i<25;i++)
	{//Fill MY_PICK_ORD
		//Set m_point
		PictureToMap(m_row_ord[i]);
		//In map eages
		if(m_point.x!=MAP_INVALIED)
		{
			if(IsSecured(m_point,0,0,false)==false)
			{//Never pick secured one
				c = m_map[m_point.x][m_point.y].type;
				if((c==CL_YELLOW)||(c==CL_PINK)||(c==CL_BROWN))
				{//It's color
					switch(c)
					{
					case CL_YELLOW:
						po[index].type = CL_YELLOW;
						po[index].ord  = 20;
					break;
					case CL_PINK:
						po[index].type = CL_PINK;
						po[index].ord  = 15;
					break;
					case CL_BROWN:
						po[index].type = CL_BROWN;
						po[index].ord  = 10;
					}
					po[index].h = m_map[m_point.x][m_point.y].h;
					po[index].p_map.x = m_point.x; po[index].p_map.y = m_point.y;
					po[index].p_pic.x = m_row_ord[i].x; po[index].p_pic.y = m_row_ord[i].y;
					index++;
				}else{
				//************** BATTERY *****************
					if(c==CL_BATTERY)
					{
						ret = DoPickUp(robot,m_row_ord[i]);
						if(ret==RET_SUCCESS)
						{
							//Recharge
							m_rem_bat += m_map[m_point.x][m_point.y].h*100;
							//Clear it from vertual map
							m_map[m_point.x][m_point.y].type = CL_FREE; 
							m_map[m_point.x][m_point.y].h	 = 0;
							if(m_rem_bat>m_full_bat)
								m_rem_bat = m_full_bat;
						}
					}
				}//************** BATTERY *****************
			}
		}
	}
	if(index!=0)
	{
		SortAcending(po,index);
		if(PickColor(robot,po,index-1)==RET_STOP_MIS)
			return RET_STOP_MIS;
	}
return RET_SUCCESS;
}

//------------------------------------------------------------------------------------------------
void CMyPackages::PassColors(CRobot &robot,BYTE dropd)
{
MY_POINT	p;
MY_TAGS		tags[25];
WORD		ntag;
BYTE		d;
int			tc,x,y;

	tc = 0;
	d  = m_facing;
//*********************** PICK **********************
	for (y=4;y>=0;y--)
	{
		for (x=0;x<=4;x++)
		{	
			p.x=x;p.y=y;
			//Set m_point
			PictureToMap(p);
			//In map eages  
			if(m_point.x==MAP_INVALIED)
				return;
			
			if(m_map[m_point.x][m_point.y].dirty==CL_DIRTY)
			{
				if((m_map[m_point.x][m_point.y].type==CL_YELLOW)||(m_map[m_point.x][m_point.y].type==CL_BROWN)||(m_map[m_point.x][m_point.y].type==CL_PINK))
				{
					//Make a new tag
					MakeTag(tags[tc].tag,&ntag);
					//Lest,s pick
					if(DoPickUp(robot,p,tags[tc].tag)==RET_SUCCESS)
					{
						//Clear it from vertual map
						m_map[m_point.x][m_point.y].type = CL_FREE;
						m_map[m_point.x][m_point.y].h	 = 0;
						tc++;
					}
				}
			}
		}
	}
//************************ PICK **********************
	if(tc==0)
		return;//Nothing picked
	tc--; 
	SetDirection(robot,dropd);
//************************ DROP **********************
	for (y=4;y>=0;y--)
	{
		for (x=0;x<=4;x++)
		{	
			if(tc<0)
			{
				SetDirection(robot,d);//Reset direction
				return;
			}
			p.x=x;p.y=y;
			//Set m_point
			PictureToMap(p);
			//In map eages  
			if(m_point.x==MAP_INVALIED)
				return;
			if(m_map[m_point.x][m_point.y].type==CL_FREE)
			{
				if(DoDrop(robot,p,tags[tc].tag)==RET_SUCCESS)
					tc--;
			}
		}
	}
//************************ DROP **********************
}
//--------------------------------------------------------------------------------------------
void CMyPackages::SortAcending(MY_PICK_ORD *arr,BYTE count)
{
MY_PICK_ORD	*arr_save;
BYTE		min,i,s,sort[25];
BYTE		j;

	//Save array
	arr_save = new MY_PICK_ORD[count];
	memcpy(arr_save,arr,12*count);

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
		arr[i].type = arr_save[i].type;
		arr[i].ord  = arr_save[i].ord;
	}

	delete[] arr_save;
}

