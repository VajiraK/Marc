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
BYTE		oldcol;
BYTE		ret = RET_ERROR;

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
					break;
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
//------------------------------------------------------------------------------------------------
BYTE CMyPackages::PickBlack(MY_POINT p,CRobot &robot,BYTE dropd,BYTE mode)
{
BYTE	ret;
char	stag[4];
WORD	ntag;

	//Check helth
	if(m_helth<HELTH_MARGIN)
	{
		if(mode==MODE_CHECK_HELTH)
			return RET_STOP_MIS;
	}

	//Set m_point
	PictureToMap(p);
	//In map eages  
	if(m_point.x==MAP_INVALIED)
		return RET_SUCCESS;

	if(m_map[m_point.x][m_point.y].type==CL_BLACK)
	{
		if(GetCollected()>=115)
		{//No room for blackies so drop some color
			DropColorPacks(robot,dropd,10,ST_MAX_H);
			//Remap m_point because DropColorPacks changed it
			PictureToMap(p);
		}

		//Make a new tag
		MakeTag(stag,&ntag);

		//Lest,s pick
		ret = DoPickUp(robot,p,stag);

		switch (ret) 
		{ 
		case RET_SUCCESS:
			m_bat_passblack += 10;
			//Add to store
			AddToStock(MODE_BLACK,m_map[m_point.x][m_point.y].h,CL_BLACK,ntag,stag);

			if(m_helth!=0)
				m_helth--;//Drop helth
			//Clear it from vertual map
			m_map[m_point.x][m_point.y].type = CL_FREE;
			return RET_SUCCESS;
		}

	}
	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyPackages::DropBlackPacks(CRobot &robot,int oldcol,BYTE dropd)
{
MY_POINT	p;
BYTE		d,ret;
int			x,y;

	if(IsBlackOver())
		return RET_SUCCESS;

	d = m_facing;

	//Drop them in oposit direction
	SetDirection(robot,dropd);

	for (y=4;y>=0;y--)
	{for (x=4;x>=0;x--)
		{	
			if(IsBlackOver()==false)
			{
				p.x=x;p.y=y;
				PictureToMap(p);
				if(m_point.x!=MAP_INVALIED)
				{
					if(m_map[m_point.x][m_point.y].type==CL_FREE)
					{
					MY_POINT tmp;
					MY_ITEM  ditem;
						tmp.x=x;tmp.y=y;

						StockRelease(&ditem,ST_BLACK);
						ret = DoDrop(robot,tmp,ditem.stag);

						switch (ret) 
						{
						case RET_SUCCESS:
							//Reduce storage
							RemoveFromStock(MODE_BLACK,ditem.ntag);
							//Place it in vertual map
							m_map[m_point.x][m_point.y].type = ditem.type;
							m_map[m_point.x][m_point.y].h	 = ditem.h;
							break;
						}
					}
				}
			}else{
				//All collected blacks proped
				break;
			}
		}
	}

	//Is some colors drop in the proccess
	if(oldcol!=GetColorCount())
		SmartPick(robot);
	
	SetDirection(robot,d);

return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyPackages::DropColorPacks(CRobot &robot,BYTE dropd,BYTE amt,BYTE mode)
{//This func make room to pick blakies (Store has field)
MY_POINT p;
BYTE	 d,ret;
int		 x,y;
int		 dh = 0;//Drop height

	if(IsColorOver())
		return RET_SUCCESS;

	d = m_facing;

	//Drop them in oposit direction
	SetDirection(robot,dropd);

	for (y=4;y>=0;y--)
	{for (x=4;x>=0;x--)
		{	
			if(IsColorOver()==false)
			{
				//Only some height of packs
				if(dh>=amt)
				{
					SetDirection(robot,d);
					return RET_SUCCESS;
				}
				p.x=x;p.y=y;
				PictureToMap(p);
				if(m_point.x!=MAP_INVALIED)
				{
					if(m_map[m_point.x][m_point.y].type==CL_FREE)
					{
					MY_POINT tmp;
					MY_ITEM  ditem;
						tmp.x=x;tmp.y=y;
						StockRelease(&ditem,mode);
						ret = DoDrop(robot,tmp,ditem.stag);

						switch (ret) 
						{
						case RET_SUCCESS:
							//Update stock
							RemoveFromStock(MODE_COLOR,ditem.ntag);
							//Cal droped color height
							dh += ditem.h;
							//Place it in vertual map
							m_map[m_point.x][m_point.y].type = ditem.type;
							m_map[m_point.x][m_point.y].h	 = ditem.h;
							break;
						}
					}
				}
			}else{
				//Collected colors over
				SetDirection(robot,d);
				return RET_SUCCESS;
			}
		}
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
BYTE CMyPackages::PickColor(CRobot &robot,MY_POINT p,BYTE c)
{
WORD	ntag;
char	stag[4];
int		ret,oh;

	//Set m_point
	PictureToMap(p);
	//In map eages
	if(m_point.x==MAP_INVALIED)
		return 0;

	if(m_map[m_point.x][m_point.y].type==c)
	{
		oh = m_map[m_point.x][m_point.y].h;
		//Check with current stoc 
		if((GetCollected()+oh)>120)
			return RET_STOP_MIS;//Actualy this should not RET_STOP_MIS there might be oh low pakages to pick 
		//Make a new tag
		MakeTag(stag,&ntag);
		//Lest,s pick
		ret = DoPickUp(robot,p,stag);

		switch (ret) 
		{ 
		case RET_SUCCESS:
			//Add to store
			AddToStock(MODE_COLOR,oh,c,ntag,stag);
			//Clear it from vertual map
			m_map[m_point.x][m_point.y].type = CL_FREE; 
			m_map[m_point.x][m_point.y].h	 = 0;
			//Check stock
			if(GetCollected()==120)
				return RET_STOP_MIS;
			//Can we go home with this luggage(Don't check befor collect poit's lost may occur)
			if(CheckBat(0,0)==RET_STOP_MIS)
				return RET_STOP_MIS;
			//Check remainig packs in the map
			if(IsAllCollected())
				return RET_STOP_MIS;

			return RET_SUCCESS;
		//case RET_ERROR:
			//Here can enter code to discard last tag if needed
			//break;
		}
	}
	return RET_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
WORD CMyPackages::IsBlackFeasible()
{
MY_POINT	p;
BYTE		st = 0;
WORD		i  = 0;

	for (int y=4;y>=0;y--)
	{
		for (int x=0;x<=4;x++)
		{	
			p.x=x;p.y=y;
			PictureToMap(m_curent,p,m_facing);
			if(m_point.x!=MAP_INVALIED)
			{
				i++;
				if((m_map[m_point.x][m_point.y].type==CL_WALL)||(m_map[m_point.x][m_point.y].type==CL_UNKNOWN)||(m_map[m_point.x][m_point.y].type==CL_DOOR))
					return st;
			}
		}
		st++;
	}

	if(i==25)
		return 5;
	else
		return 0;
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
				{
					tc--;
				}
			}
		}
	}
//************************ DROP **********************
}
