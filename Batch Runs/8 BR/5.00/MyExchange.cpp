// MyExchange.cpp: implementation of the CMyExchange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyExchange.h"

CMyExchange::CMyExchange(){}
CMyExchange::~CMyExchange(){}
//--------------------------------------------------------------------------------------------
BYTE CMyExchange::PickSecured(CRobot &robot)
{
MY_POINT p;
BYTE	 c,h;
WORD	 ntag;
char	 stag[4];

	for (p.x=0;p.x<=4;p.x++)
	{
		for (p.y=0;p.y<=4;p.y++)
		{
			PictureToMap(p);
			if(m_point.x!=MAP_INVALIED)
			{
				c = m_map[m_point.x][m_point.y].type;
				h = m_map[m_point.x][m_point.y].h;
				if((c==CL_YELLOW)||(c==CL_PINK)||(c==CL_BROWN))
				{
				BYTE bid;
					if(IsSecured(m_point,h,&bid,true))
					{//Only pick secured one **************************
						//Make a new tag
						MakeTag(stag,&ntag);
						//Pick
						if(DoPickUp(robot,p,stag)==RET_SUCCESS)
						{
							//Add to store
							AddToStock(MODE_COLOR,h,c,ntag,stag);
							//UpdateBatBoom
							UpdateBatBoom(bid,h);
							m_map[m_point.x][m_point.y].type = CL_FREE;
							m_map[m_point.x][m_point.y].h    = 0;
						}
					}//Only pick secured one **************************
				}
			}
		}
	}

//****************** OPTIMIZE LUGGAGE ******************
/*WORD bs,nb;
int  i,nt;
	//Optimize Luggage for safe journy
	//DropBlackPacks(robot,GetColorCount(),m_path[i+1].came_d);
	bs = GetBackSteps(m_path_i,&nt);
	for(int t=0;t<=25;t++)
	{//Only try 25 times
		nb = 1 + ((10+((GetStock(ST_ACTUAL))*2)) * (bs)) + (nt*6) + bs + nt + m_bat_passblack;

		/*if(nb>=m_rem_bat)
			DropColorPacks(robot,m_path[i+1].came_d,1,MODE_DROP_BAT);
		else
			break;
	}*/
//****************** OPTIMIZE LUGGAGE ******************

	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::PickColor(CRobot &robot,MY_POINT p,BYTE c)
{//SmartPick helper
WORD	ntag;
BYTE	ret,oh;
char	stag[4];

	//Set m_point
	PictureToMap(p);
	//In map eages
	if(m_point.x==MAP_INVALIED)
		return RET_SUCCESS;

	//Never pick secured one (VERY RARE CASE)
	if(IsSecured(m_point,0,0,false))
		return RET_SUCCESS;//Certanly secured one

	if(m_map[m_point.x][m_point.y].type==c)
	{
		oh = m_map[m_point.x][m_point.y].h;
		//Check with current stoc 

		if((GetStock(ST_ALL)+oh)>120)
		{
			#ifdef _DEBUG
			MessageBox(NULL,"Belly Full !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
			#endif
			return RET_STOP_MIS;//Actualy this should not RET_STOP_MIS there might be oh low pakages to pick 
		}
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
			//************************************
			//Check stock
			if(GetStock(ST_ALL)==120)
			{
				#ifdef _DEBUG
				MessageBox(NULL,"Belly Full(120) !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
				#endif
				return RET_STOP_MIS;
			}
			//Can we go home with this luggage(Don't check befor collect poit's lost may occur)
			if(CheckBat(0,GetStock(ST_ACTUAL))==RET_STOP_MIS)
				return RET_STOP_MIS;
			//Check remainig packs in the map
			if(IsAllCollected())
				return RET_STOP_MIS;
			//************************************
			return RET_SUCCESS;
		//case RET_ERROR:
			//Here can enter code to discard last tag if needed
			//break;
		}
	}
	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::PickColor(CRobot &robot)
{//Pic dump colors deu to lack store space
MY_POINT	p;
WORD		ntag;
BYTE		c,h,ret,oh;
char		stag[4];

	for (p.x=0;p.x<=4;p.x++)
	{
		for (p.y=0;p.y<=4;p.y++)
		{
			PictureToMap(p);
			if(m_point.x!=MAP_INVALIED)
			{
				c = m_map[m_point.x][m_point.y].type;
				h = m_map[m_point.x][m_point.y].h;
				if((c==CL_YELLOW)||(c==CL_PINK)||(c==CL_BROWN))
				{//************************************************
					oh = m_map[m_point.x][m_point.y].h;
					//Check with current stock

					if((GetStock(ST_ACTUAL)+oh)>120)
						return RET_ERROR;//No storage space
				
					//Make a new tag
					MakeTag(stag,&ntag);
					//Lest,s pick
					ret = DoPickUp(robot,p,stag);

					if(ret==RET_SUCCESS)
					{ 
						//Add to store
						AddToStock(MODE_COLOR,oh,c,ntag,stag);
						//AddToStock(MODE_SECURE,oh,c,ntag,stag);//No increase secure stock(m_sec_pack)
						//Clear it from vertual map
						m_map[m_point.x][m_point.y].type = CL_FREE; 
						m_map[m_point.x][m_point.y].h	 = 0;
					}
				}//************************************************
			}
		}
	}

	return RET_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
WORD CMyExchange::IsBlackFeasible()
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
BYTE CMyExchange::PickBlack(MY_POINT p,CRobot &robot,BYTE dropd,BYTE mode)
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
		if(GetStock(ST_ACTUAL)>=115)
		{//No room for blackies so drop some color
			//DropColorPacks(robot,dropd,10,ST_MAX_H);
			DropColorPacks(robot,dropd,10,MODE_DROP_SPACE);
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
BYTE CMyExchange::DropBlackPacks(CRobot &robot,int oldcol,BYTE dropd)
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
		PickColor(robot);
	
  	SetDirection(robot,d);

return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::DropColorPacks(CRobot &robot,BYTE dropd,BYTE amt,BYTE mode)
{//Drop color for variour needs
MY_POINT p;
BYTE	 d,bid;
;
int		 x,y;
int		 dh = 0;//Drop height

	if(IsColorOver())
		return RET_SUCCESS;

	d   = m_facing;
	bid = GetBoomID();

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
					if(mode==MODE_DROP_SECURE)
						CreateBatBoom(dh,bid);
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
						switch(mode)
						{
						case MODE_DROP_SECURE:
							StockRelease(&ditem,ST_MAX_H);
							break;
						case MODE_DROP_SPACE:
							StockRelease(&ditem,ST_MAX_H);
							break;
						case MODE_DROP_BAT:
							StockRelease(&ditem,ST_LOW_P);
						}
						
						if(DoDrop(robot,tmp,ditem.stag)==RET_SUCCESS)
						{
							RemoveFromStock(MODE_COLOR,ditem.ntag);
							//Is it secured
							if(mode==MODE_DROP_SECURE)
								AddToSecureList(m_point,ditem.h,bid);
							//Cal droped color height
							dh += ditem.h;
							//Place it in vertual map
							m_map[m_point.x][m_point.y].type = ditem.type;
							m_map[m_point.x][m_point.y].h	 = ditem.h;
						}
					}
				}
			}
		}
	}

	if(mode==MODE_DROP_SECURE)
		CreateBatBoom(dh,bid);
	SetDirection(robot,d);

	return RET_SUCCESS;
}