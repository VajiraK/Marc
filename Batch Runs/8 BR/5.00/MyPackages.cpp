// MyPackages.cpp: implementation of the CMyPackages class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyPackages.h"

CMyPackages::~CMyPackages(){}
CMyPackages::CMyPackages()
{
	m_helth = 100;
}
//------------------------------------------------------------------------------------------------
BYTE CMyPackages::PassBlacks(CRobot &robot,BYTE dropd,BYTE mode)
{//Pick and drop bihind Black packages
MY_POINT	p;
WORD		bst;
int			x,y;
BYTE		oldcol,ret = RET_ERROR;

	//Check helth
	if(m_helth<HELTH_MARGIN)
	{
		if(mode==MODE_CHECK_HELTH)
			return RET_STOP_MIS;
	}

	oldcol = GetColorCount();

	//Get the amount of steps can be cleared by removing blacks
	bst = 5-IsBlackFeasible();

	if(bst==5)
	{//Can't advance by removing blacks
		return RET_ERROR;
	}else{

		for (y=4;y>=bst;y--)
		{
			for (x=0;x<=4;x++)
			{	
				p.x=x;p.y=y;
				ret = PickBlack(p,robot,dropd,mode);
				if(ret==RET_STOP_MIS)
					return RET_STOP_MIS;
			}
		}

 		if(IsBlackOver()==false)
		{
			if(DropBlackPacks(robot,oldcol,dropd)==RET_STOP_MIS)
				return RET_STOP_MIS;
			return ret;
		}
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
MY_POINT p;
BYTE	 ret;
	for (p.x=0;p.x<=4;p.x++)
	{for (p.y=0;p.y<=4;p.y++)
	{//BATTERY
		//Set m_point
		PictureToMap(p);
		//In map eages
		if(m_point.x!=MAP_INVALIED)
		{
			if(m_map[m_point.x][m_point.y].type==CL_BATTERY)
			{
				//Battory pick
				ret = DoPickUp(robot,p);

				if(ret==RET_SUCCESS)
				{
					//Recharge
					m_rem_bat += m_map[m_point.x][m_point.y].h*100;
					//Clear it from vertual map
					m_map[m_point.x][m_point.y].type = CL_FREE; 
					m_map[m_point.x][m_point.y].h	 = 0;
					if(m_rem_bat>m_full_bat)
						m_rem_bat = m_full_bat;
				}else{
					return ret;
				}
			}
		}
	}}
	//===================================================
	for (p.x=0;p.x<=4;p.x++)
	{for (p.y=0;p.y<=4;p.y++)
	{//YELLOW
			if(PickColor(robot,p,CL_YELLOW)==RET_STOP_MIS)
				return RET_STOP_MIS;
	}}
	//===================================================
	for (p.x=0;p.x<=4;p.x++)
	{for (p.y=0;p.y<=4;p.y++)
	{//PINK
			if(PickColor(robot,p,CL_PINK)==RET_STOP_MIS)
				return RET_STOP_MIS;
	}}
	//===================================================
	for (p.x=0;p.x<=4;p.x++)
	{for (p.y=0;p.y<=4;p.y++)
	{//BROWN
			if(PickColor(robot,p,CL_BROWN)==RET_STOP_MIS)
				return RET_STOP_MIS;
	}}
	//===================================================

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
//-------------------------------------------------------------------------------------------------
/*BYTE CMyPackages::SmartPickEx(CRobot &robot)
{
MY_PICK_ORD pic_ord[25];
MY_POINT	p;
BYTE		ret,count,type;

	for (p.x=0;p.x<=4;p.x++)
	{for (p.y=0;p.y<=4;p.y++)
	{//BATTERY
		//Set m_point
		PictureToMap(p);
		//In map eages
		if(m_point.x!=MAP_INVALIED)
		{
			if(m_map[m_point.x][m_point.y].type==CL_BATTERY)
			{
				//Battory pick
				ret = DoPickUp(robot,p);
				if(ret==RET_SUCCESS)
				{
					//Recharge
					m_rem_bat += m_map[m_point.x][m_point.y].h*100;
					//Clear it from vertual map
					m_map[m_point.x][m_point.y].type = CL_FREE; 
					m_map[m_point.x][m_point.y].h	 = 0;
					if(m_rem_bat>m_full_bat)
						m_rem_bat = m_full_bat;
				}else{
					return ret;
				}
			}
		}
	}}

	count = 0;
	//Create picking points arr
	for (p.x=0;p.x<=4;p.x++)
	{
		for (p.y=0;p.y<=4;p.y++)
		{
			PictureToMap(p);
			if(m_point.x!=MAP_INVALIED)
			{
				type = m_map[m_point.x][m_point.y].type;
				if((type==CL_YELLOW)||(type==CL_PINK)||(type==CL_BROWN))
				{
					pic_ord[count].p_map.x=m_point.x; pic_ord[count].p_map.y=m_point.y;
					pic_ord[count].p_pic.x=p.x; pic_ord[count].p_pic.y=p.y;
					pic_ord[count].h = m_map[m_point.x][m_point.y].h;
					count++;
				}
			}
		}
	}

	if(count==0)
		return RET_SUCCESS;

	SortDescending(pic_ord,count);

	//Let's pick them
	for(BYTE i=0;i<count;i++)
	{
		if(PickColorEx(robot,pic_ord[i])==RET_STOP_MIS)
			return RET_STOP_MIS;
	}

return RET_SUCCESS;
}
//--------------------------------------------------------------------------------------------
BYTE CMyPackages::PickColorEx(CRobot &robot,MY_PICK_ORD po)
{
WORD	ntag;
char	stag[4];
int		ret,oh;
BYTE	c;

		oh = m_map[po.p_map.x][po.p_map.y].h;
		c  = m_map[po.p_map.x][po.p_map.y].type;
		//Check with current stoc 
		if((GetActualStock()+oh)>120)
			return RET_STOP_MIS;//Actualy this should not RET_STOP_MIS there might be oh low pakages to pick 
		//Make a new tag
		MakeTag(stag,&ntag);
		//Lest,s pick
		ret = DoPickUp(robot,po.p_pic,stag);

		switch (ret) 
		{ 
		case RET_SUCCESS:
			//Add to store
			AddToStock(MODE_COLOR,oh,c,ntag,stag);
			//Clear it from vertual map
			m_map[po.p_map.x][po.p_map.y].type = CL_FREE; 
			m_map[po.p_map.x][po.p_map.y].h	   = 0;
			//Check stock
			if(GetActualStock()==120)
				return RET_STOP_MIS;
			//Can we go home with this luggage(Don't check befor collect poit's lost may occur)
			if(CheckBat(0,GetStock(ST_ACTUAL))==RET_STOP_MIS)
				return RET_STOP_MIS;
			//Check remainig packs in the map
			if(IsAllCollected())
				return RET_STOP_MIS;
			return RET_SUCCESS;
		}
	
	return RET_SUCCESS;
}
//--------------------------------------------------------------------------------------------
void CMyPackages::SortDescending(MY_PICK_ORD *arr,BYTE count)
{
MY_PICK_ORD	*arr_save;
BYTE		max,i,s,sort[25];
BYTE		j;

	//Save array
	arr_save = new MY_PICK_ORD[count];
	for(i=0;i<count;i++)
	{
		arr_save[i].p_map.x = arr[i].p_map.x;
		arr_save[i].p_map.y = arr[i].p_map.y;
		arr_save[i].p_pic.x = arr[i].p_pic.x;
		arr_save[i].p_pic.y = arr[i].p_pic.y;
		arr_save[i].h  = arr[i].h;
	}

	//Sort 
	s = 0;
	for(j=0;j<count;j++)
	{
		max = arr[0].h;
		sort[s] = 0;
		for(i=1;i<count;i++)
		{
			if(max>arr[i].h)
			{
				max = arr[i].h;
				sort[s] = i;
			}
		}
		arr[sort[s]].h = 6;
		s++;
	}

	//Copy result
	for(i=0;i<count;i++)
	{
		arr[i].p_map.x = arr_save[sort[i]].p_map.x;
		arr[i].p_map.y = arr_save[sort[i]].p_map.y;
		arr[i].p_pic.x = arr_save[sort[i]].p_pic.x;
		arr[i].p_pic.y = arr_save[sort[i]].p_pic.y;
		arr[i].h  = arr_save[sort[i]].h;
	}

	delete[] arr_save;
}*/

