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
		{
			//Is some colors drop in the proccess
			if(oldcol!=GetColorCount())
				DropBlackPacks(robot,oldcol,dropd);
			return RET_STOP_MIS;
		}
		if((m_row_ord[i].y==bst)&&(m_row_ord[i].x==4))
			break;
	}

	if(DropBlackPacks(robot,oldcol,dropd)==RET_STOP_MIS)
		return RET_STOP_MIS;
	return ret;

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
BYTE		ret,col_count[3],c,index=0;

	col_count[0] = col_count[1] = col_count[2] = 0;

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
							if(m_rem_bat>=m_full_bat)
								m_rem_bat = m_full_bat;
						}
					}
				}//************** BATTERY *****************
			}
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

		/*BYTE f[25];
		for(BYTE t=0;t<index;t++)
			f[t] = po[t].h;*/

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
		arr[i].type = arr_save[sort[i]].type;
		//arr[i].ord  = arr_save[sort[i]].ord;
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
	memcpy(arr_save,arr,12*count);

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
		//arr[i].ord  = arr_save[sort[i]].ord;
	}

	delete[] arr_save;
}