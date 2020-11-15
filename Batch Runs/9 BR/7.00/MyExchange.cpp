// MyExchange.cpp: implementation of the CMyExchange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyExchange.h"

CMyExchange::CMyExchange(){}
CMyExchange::~CMyExchange(){}
//--------------------------------------------------------------------------------------------
BYTE CMyExchange::PickSecured(CRobot &robot,WORD pathi)
{
MY_ITEM	 st_item;
BYTE bid;

	for(BYTE i=0;i<25;i++)
	{
		PictureToMap(m_row_ord[i]);
		if(m_point.x!=MAP_INVALIED)
		{
			st_item.type = m_map[m_point.x][m_point.y].type;
			st_item.h = m_map[m_point.x][m_point.y].h;
			if((st_item.type==CL_YELLOW)||(st_item.type==CL_PINK)||(st_item.type==CL_BROWN))
			{
				if(IsSecured(m_point,st_item.h,&bid,true))
				{//Only pick secured one **************************
					//Make a new tag
					MakeTag(st_item.stag,&st_item.ntag);
					//Pick
					if(DoPickUp(robot,m_row_ord[i],st_item.stag)==RET_SUCCESS)
					{
						//Add to store
						st_item.realbk = INVALIED;
						AddToStock(MODE_COLOR,st_item);
						m_map[m_point.x][m_point.y].type = CL_FREE;
						m_map[m_point.x][m_point.y].h    = 0;
					}
				}//Only pick secured one **************************
			}
		}
	}

	//****************** OPTIMIZE LUGGAGE ******************

	//Optimize Luggage for safe journy
	/*DWORD nb;
	for(int t=0;t<=25;t++)
	{//Only try 25 times
		nb = SendAgent(pathi,GetColorCount());
		if(m_rem_bat<nb)
			DropColorPacks(robot,GetOposit(m_facing),1,MODE_DROP_BAT);
		else
			break;
	}*/
//****************** OPTIMIZE LUGGAGE ******************
	return RET_SUCCESS;
}
//--------------------------------------------------------------------------------------------
BYTE CMyExchange::PickColor(CRobot &robot,MY_PICK_ORD *po,BYTE count)
{//SmartPick helper
bool	st_mis,paint_black;
MY_ITEM st_item;

	paint_black = st_mis = false;

	for(BYTE i=0;i<=count;i++)
	{
		if((GetStock(ST_ALL)+po[i].h)>120)
		{//Height not fit
			paint_black = true;
			#ifdef _DEBUG
			Beep(18000,5);
			#endif
		}else{
			//Can we go home with this luggage
			if(CheckBat(0,GetColorCount()+po[i].h)!=RET_STOP_MIS)
			{
				//Make a new tag
				MakeTag(st_item.stag,&st_item.ntag);
				//Lest,s pick
				if(DoPickUp(robot,po[i].p_pic,st_item.stag)==RET_SUCCESS) 
				{ 
					//Add to store
					st_item.type   = po[i].type;
					st_item.h	   = po[i].h;
					st_item.realbk = INVALIED;
					AddToStock(MODE_COLOR,st_item);
					//Clear it from vertual map
					m_map[po[i].p_map.x][po[i].p_map.y].type = CL_FREE; 
					m_map[po[i].p_map.x][po[i].p_map.y].h	 = 0;
					//************************************
					//Check stock
					if(GetStock(ST_ALL)==120)
					{
						#ifdef _DEBUG
						MessageBox(NULL,"Belly Full(120) !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
						#endif
						return RET_STOP_MIS;
					}
					//Check remainig packs in the map
					if(IsAllCollected())
						return RET_STOP_MIS;
					//************************************
				}
			}else{
				st_mis = true;
				#ifdef _DEBUG
				MessageBox(NULL,"Skiping pick(LOW BAT) !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
				#endif
			}
		}
	}

	if(st_mis)
		return RET_STOP_MIS;

	if(paint_black)//Belly not fill (PaintBlack height not fitting colors)
		PaintBlack();

	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::PickColor(CRobot &robot)
{//Pic dump colors deu to lack store space
MY_ITEM	st_item;

	for(BYTE i=0;i<25;i++)
	{
		PictureToMap(m_row_ord[i]);
		if(m_point.x!=MAP_INVALIED)
		{
			st_item.type = m_map[m_point.x][m_point.y].type;
			st_item.h = m_map[m_point.x][m_point.y].h;
			if((st_item.type==CL_YELLOW)||(st_item.type==CL_PINK)||(st_item.type==CL_BROWN))
			{//************************************************
				//Check with current stock
				if((GetStock(ST_ACTUAL)+st_item.h)>120)
					return RET_ERROR;//No storage space
				//Make a new tag
				MakeTag(st_item.stag,&st_item.ntag);
				//Lest,s pick

				if(DoPickUp(robot,m_row_ord[i],st_item.stag)==RET_SUCCESS)
				{ 
					//Add to store
					st_item.realbk = INVALIED;
					AddToStock(MODE_COLOR,st_item);
					//Clear it from vertual map
					m_map[m_point.x][m_point.y].type = CL_FREE; 
					m_map[m_point.x][m_point.y].h	 = 0;
				}
			}//************************************************
		}
	}

	return RET_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyExchange::IsBlackFeasible()
{
MY_POINT	p;
BYTE		bst,st = 0;;
BYTE		i  = 0;
bool		bfound = false;

	for (int y=4;y>=0;y--)
	{
		for (int x=0;x<=4;x++)
		{	
			p.x=x;p.y=y;
			PictureToMap(m_curent,p,m_facing);
			if(m_point.x!=MAP_INVALIED)
			{
				i++;
				if(m_map[m_point.x][m_point.y].type==CL_BLACK)
					bfound = true;
				if((m_map[m_point.x][m_point.y].type==CL_WALL)||(m_map[m_point.x][m_point.y].type==CL_UNKNOWN)||(m_map[m_point.x][m_point.y].type==CL_DOOR))
				{
					bst = st;
					goto LABLE_ONE;
				}
			}
		}
		st++;
	}

	if(i==25)
		bst = 5;
	else
		bst = 0;

LABLE_ONE:
	if(bfound)
		return 5-bst;
	else
		return 5;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::PickBlack(MY_POINT p,CRobot &robot,BYTE dropd,BYTE mode)
{
MY_ITEM st_item;
BYTE	ret;

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

	//Set m_point
	PictureToMap(p);
	//In map eages  
	if(m_point.x==MAP_INVALIED)
		return RET_SUCCESS;

	if(m_map[m_point.x][m_point.y].type==CL_BLACK)
	{
		if(GetStock(ST_ACTUAL)>=115)
		{//No room for blackies so drop some color
			DropColorPacks(robot,dropd,10,MODE_DROP_SPACE);
			//Remap m_point because DropColorPacks changed it
			PictureToMap(p);
		}
		//Make a new tag
		MakeTag(st_item.stag,&st_item.ntag);

		//Lest,s pick
		ret = DoPickUp(robot,p,st_item.stag);
		switch (ret) 
		{ 
		case RET_SUCCESS:
	 		m_bat_passblack += 10;
			//Add to store
			st_item.h = m_map[m_point.x][m_point.y].h;
			st_item.realbk = m_map[m_point.x][m_point.y].realbk;
			st_item.type = CL_BLACK;
			AddToStock(MODE_BLACK,st_item);
			if(mode==MODE_CHECK_HELTH)
			{
				if(m_hel_marg!=0)
				{
					if(m_map[m_point.x][m_point.y].realbk==VALIED)//Not a painted one
						m_hel_marg--;//Drop helth
				}
			}
			//Clear it from vertual map
			m_map[m_point.x][m_point.y].type = CL_FREE;
			m_map[m_point.x][m_point.y].realbk = INVALIED;
			m_map[m_point.x][m_point.y].h = 0;
			return RET_SUCCESS;
		}
	}
	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::DropBlackPacks(CRobot &robot,int oldcol,BYTE dropd)
{
BYTE d,ret;

	d = m_facing;

	if(IsBlackOver())
	{
		//Is some colors drop in the proccess
		if(oldcol!=GetColorCount())
		{//Drop some deu to lack store but didn't pick black deu to low helth
			SetDirection(robot,dropd);
			PickColor(robot);
			SetDirection(robot,d);
		}
	return RET_SUCCESS;
	}

	//Drop them in oposit direction
	SetDirection(robot,dropd);

	for(BYTE i=0;i<25;i++)
	{
		if(IsBlackOver()==false)
		{
			PictureToMap(m_row_ord[i]);
			if(m_point.x!=MAP_INVALIED)
			{
				if(m_map[m_point.x][m_point.y].type==CL_FREE)
				{
				MY_ITEM  ditem;
					
					StockRelease(&ditem,ST_BLACK);
					ret = DoDrop(robot,m_row_ord[i],ditem.stag);
					
					switch (ret) 
					{
					case RET_SUCCESS:
						//Reduce storage
						RemoveFromStock(MODE_BLACK,ditem.ntag);
						//Place it in vertual map
						m_map[m_point.x][m_point.y].type = ditem.type;
						m_map[m_point.x][m_point.y].realbk = ditem.realbk;
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
	
	//Is some colors drop in the proccess
	if(oldcol!=GetColorCount())
		PickColor(robot);

  	SetDirection(robot,d);

return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::DropColorPacks(CRobot &robot,BYTE dropd,BYTE amt,BYTE mode)
{//Drop color for variour needs
BYTE	 d,bid = 0;
int		 dh = 0;//Drop height
MY_POINT p;
	if(IsColorOver())
		return RET_SUCCESS;

	d   = m_facing;
	//bid = GetBoomID();

	//Drop them in oposit direction
	SetDirection(robot,dropd);

	for(BYTE i=0;i<25;i++)
	{
		if(IsColorOver()==false)
		{
			//Only some height of packs
			if(dh>=amt)
			{
				SetDirection(robot,d);
				return RET_SUCCESS;
			}
			PictureToMap(m_row_ord[i]);
			p.x=m_point.x;p.y=m_point.y;
			if(p.x!=MAP_INVALIED)
			{
				if(m_map[p.x][p.y].type==CL_FREE)
				{
				MY_ITEM  ditem;
					switch(mode)
					{
					case MODE_DROP_SECURE:
						/*if(CheckBat(0,GetColorCount())==RET_STOP_MIS)
						{
							SetDirection(robot,d);
							return RET_STOP_MIS;
						}*/
						StockRelease(&ditem,ST_MAX_H);
						break;
					case MODE_DROP_SPACE:
						StockRelease(&ditem,ST_MAX_H);
						break;
					case MODE_DROP_BAT:
						StockRelease(&ditem,ST_LOW_P);
					}
					
					if(DoDrop(robot,m_row_ord[i],ditem.stag)==RET_SUCCESS)
					{
						RemoveFromStock(MODE_COLOR,ditem.ntag);
						//Is it secured
						if(mode==MODE_DROP_SECURE)
							AddToSecureList(p,ditem.h,bid);
						//Cal droped color height
						dh += ditem.h;
						//Place it in vertual map
						if(mode==MODE_DROP_BAT)
						{//Paint black
							m_map[p.x][p.y].type = CL_BLACK;
							m_map[p.x][p.y].h	 = ditem.h;
							m_map[p.x][p.y].realbk = INVALIED;//To prevent helth decay
						}else{
							m_map[p.x][p.y].type = ditem.type;
							m_map[p.x][p.y].realbk = ditem.realbk;
							m_map[p.x][p.y].h	 = ditem.h;
						}
					}
				}
			}
		}
	}

	SetDirection(robot,d);

	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
void CMyExchange::PaintBlack()
{//Mark useless color packs as black (So in future they will handle as black packs) hee.... heeee....

	for(BYTE i=0;i<25;i++)
	{
		//Set m_point
		PictureToMap(m_row_ord[i]);
		//In map eages  
		if(m_point.x==MAP_INVALIED)
			return;
	
		if(IsSecured(m_point,0,0,false)==false)
		{//Never to secured one
			if((m_map[m_point.x][m_point.y].type==CL_YELLOW)||(m_map[m_point.x][m_point.y].type==CL_BROWN)||(m_map[m_point.x][m_point.y].type==CL_PINK))
			{
				m_map[m_point.x][m_point.y].type = CL_BLACK;
				m_map[m_point.x][m_point.y].realbk = INVALIED;//To prevent helth decay
			}
		}
	}
}