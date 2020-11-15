// MyExchange.cpp: implementation of the CMyExchange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyExchange.h"

CMyExchange::~CMyExchange(){}
//--------------------------------------------------------------------------------------------
CMyExchange::CMyExchange()
{//Initialize reuse tag pool
	for(BYTE i=0;i<25;i++)
		m_tags[i].ntag = 0;
}
//--------------------------------------------------------------------------------------------
BYTE CMyExchange::PickSecured(CRobot &robot,WORD pathi)
{
MY_ITEM	 item;
MY_POINT map;

	for(BYTE i=0;i<25;i++)
	{
		if(PictureToMapEx(&m_row_ord[i],&map))
		{
			item.type = m_map[map.x][map.y].type;
			item.h	  = m_map[map.x][map.y].h;
			if((item.type==CL_YELLOW)||(item.type==CL_PINK)||(item.type==CL_BROWN))
			{
				if(IsSecured(map,&item,true))//Old tag reuse
				{//Only pick secured one **************************
					//Pick
					if(DoPickUp(robot,m_row_ord[i],item.stag)==RET_SUCCESS)
					{
						//Add to store
						AddToStock(MODE_COLOR,&item);
						m_map[map.x][map.y].type = CL_FREE;
						m_map[map.x][map.y].h    = 0;
					}
				}//Only pick secured one **************************
			}
		}
	}

	return RET_SUCCESS;
}
//--------------------------------------------------------------------------------------------
BYTE CMyExchange::PickColor(CRobot &robot,MY_PICK_ORD *po,BYTE count)
{//SmartPick Helper
MY_ITEM item;

	for(BYTE i=0;i<=count;i++)
	{
		if((GetStock(ST_COL_SEC)+po[i].h)>120)
		{//Height not fit
			//ExchangePacks(robot,po[i].p_pic,po[i].p_map);
			return RET_STOP_MIS;
		}else{
			if(GetStock(ST_COL_SEC)==120)
			{//No space to pic let's exchange
				//ExchangePacks(robot,po[i].p_pic,po[i].p_map);
				return RET_STOP_MIS;
			}else{
				//Can we go home with this luggage
				if(CheckBat(0,GetColorHeight()+po[i].h)!=RET_STOP_MIS)
				{
					if(GetOldTag(&item)==false)
						MakeTag(item.stag,&item.ntag);//Make a new tag
					
					//Lest,s pick
					if(DoPickUp(robot,po[i].p_pic,item.stag)==RET_SUCCESS) 
					{ 
						//Add to store
						item.type  = po[i].type;
						item.h	   = po[i].h;
						AddToStock(MODE_COLOR,&item);
						//Clear it from vertual map
						m_map[po[i].p_map.x][po[i].p_map.y].type = CL_FREE; 
						m_map[po[i].p_map.x][po[i].p_map.y].h	 = 0;
						//Check remainig packs in the map
						if(IsAllCollected())
							return RET_STOP_MIS;
					}
				}else{
					return RET_STOP_MIS;//Low Bat
				}
			}
		}
	}

	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::PickColor(CRobot &robot)
{//Pic Dump colors deu to lack store space
MY_ITEM	 item;
MY_POINT p;
	for(BYTE i=0;i<25;i++)
	{
		if(PictureToMapEx(&m_row_ord[i],&p))
		{
			item.type = m_map[p.x][p.y].type;
			item.h	  = m_map[p.x][p.y].h;
			if((item.type==CL_YELLOW)||(item.type==CL_PINK)||(item.type==CL_BROWN))
			{//************************************************
				//Check with current stock
				if((GetStock(ST_COL_BK)+item.h)>120)
				{//No storage space
					DropBlackPacks(robot,GetColorHeight(),m_facing);
				}

				if(GetStock(ST_COL_BK)+m_map[p.x][p.y].h>120)
					return RET_SUCCESS;

				if(GetOldTag(&item)==false)
					MakeTag(item.stag,&item.ntag);//Make a new tag

				//Lest,s pick
				if(DoPickUp(robot,m_row_ord[i],item.stag)==RET_SUCCESS)
				{ 
					//Add to store
					//item.realbk = INVALIED;
					AddToStock(MODE_COLOR,&item);
					//Clear it from vertual map
					m_map[p.x][p.y].type = CL_FREE; 
					m_map[p.x][p.y].h	 = 0;
				}
			}//************************************************
		}
	}

	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::PickBlack(MY_POINT p,CRobot &robot,BYTE dropd,BYTE mode)
{
MY_ITEM  item;
MY_POINT map;

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

	if(PictureToMapEx(&p,&map)==false)
		return RET_SUCCESS;

	item.type = m_map[map.x][map.y].type;
	item.h = m_map[map.x][map.y].h;

	if(item.type==CL_BLACK)
	{
		if(GetStock(ST_COL_BK)+m_map[map.x][map.y].h>120)
		{//No room for blackies so drop some color
			DropColorPacks(robot,dropd,10,MODE_DROP_SPACE);
		}

		if(GetStock(ST_COL_BK)+item.h>120)
			return RET_SUCCESS;

		if(GetOldTag(&item)==false)
			MakeTag(item.stag,&item.ntag);//Make a new tag

		//Lest,s pick
		if(DoPickUp(robot,p,item.stag)==RET_SUCCESS)
		{	 	
			//Add to store
			item.realbk = m_map[map.x][map.y].realbk;
			AddToStock(MODE_BLACK,&item);

			if(mode==MODE_CHECK_HELTH)
			{
				if(m_hel_marg!=0)
				{
					if(m_map[map.x][map.y].realbk==VALIED)//Not a painted one
						m_hel_marg--;//Drop helth
				}
			}
			//Clear it from vertual map
			m_map[map.x][map.y].type   = CL_FREE;
			m_map[map.x][map.y].realbk = INVALIED;
			m_map[map.x][map.y].h	   = 0;
			return RET_SUCCESS;
		}
	}
	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::DropBlackPacks(CRobot &robot,int oldcol,BYTE dropd)
{
MY_POINT map;
MY_ITEM  item;
BYTE	 d;

	d = m_facing;

	if(IsBlackOver())
	{
		//Is some colors drop in the proccess
		if(oldcol!=GetColorHeight())
		{//Drop some deu to lack store but didn't pick black deu to low helth
			SetDirection(robot,dropd);
			PickColor(robot);
			SetDirection(robot,d);
		}
	return RET_SUCCESS;
	}

	//Drop them in specified direction
	SetDirection(robot,dropd);

	for(BYTE i=0;i<25;i++)
	{
		if(IsBlackOver()==false)
		{
			if(PictureToMapEx(&m_row_ord[i],&map))
			{
				if(m_map[map.x][map.y].type==CL_FREE)
				{
					ReleaseMaxHeight(&item,ST_BLACK);
					//Drop
					if(DoDrop(robot,m_row_ord[i],item.stag)==RET_SUCCESS)
					{
						//Save this tag for later use
						SaveOldTag(&item);
						//Reduce storage
						RemoveFromStock(MODE_BLACK,item.ntag);
						//Place it in vertual map
						m_map[map.x][map.y].type   = item.type;
						m_map[map.x][map.y].realbk = item.realbk;
						m_map[map.x][map.y].h	   = item.h;
					}
				}
			}
		}else{
			//All collected blacks proped
			break;
		}
	}
	
	//Is some colors drop in the proccess
	if(oldcol!=GetColorHeight())
		PickColor(robot);
		//PickSecured(robot,0);

  	SetDirection(robot,d);

return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::DropColorPacks(CRobot &robot,BYTE dropd,BYTE amt,BYTE mode)
{//Drop color for variour needs
BYTE	 d;
int		 dh = 0;//Drop height
MY_POINT map;

	if(IsColorOver())
		return RET_SUCCESS;

	d   = m_facing;

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

			if(PictureToMapEx(&m_row_ord[i],&map))
			{
				if(m_map[map.x][map.y].type==CL_FREE)
				{
				MY_ITEM  ditem;

					if(mode==MODE_DROP_SECURE)
					{
						if(CheckBat(0,GetColorHeight())==RET_STOP_MIS)
						{
							SetDirection(robot,d);
							return RET_STOP_MIS;
						}
					}

					ReleaseMaxHeight(&ditem,ST_COLOR);

					if(DoDrop(robot,m_row_ord[i],ditem.stag)==RET_SUCCESS)
					{
						RemoveFromStock(MODE_COLOR,ditem.ntag,ditem.type);
						//Is it secured
						if(mode==MODE_DROP_SECURE)
							AddToSecureList(map,&ditem);
						else
							SaveOldTag(&ditem);//Secure tags are reusing

						//Cal droped color height
						dh += ditem.h;
						m_map[map.x][map.y].type   = ditem.type;
						m_map[map.x][map.y].realbk = ditem.realbk;
						m_map[map.x][map.y].h	   = ditem.h;
					}
				}
			}
		}
	}

	SetDirection(robot,d);

	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::ExchangePacks(CRobot &robot,MY_POINT pic,MY_POINT map)
{
MY_CAT_ITEM cat_item[6];
BYTE		count;

	cat_item[0].type = m_map[map.x][map.y].type;
	cat_item[0].h	 = m_map[map.x][map.y].h;

	if(ReleaseExchange(cat_item,&count))
	{//Exchange possible
	MY_PICK_ORD droplist[5];
	MY_ITEM		aitem;
	BYTE		bi[5],si[5],i,j,k,drop;
	char		stag[4];
	bool		ddone=false;

		drop = j = k = 0;
		//Separate belly and secure items
		for(i=1;i<count;i++)
		{
			if(cat_item[i].store==STORE_SEC) 
			{
				si[k] = i;
				k++;
			}else{
				bi[j] = i;
				j++;
			}
		}

		if(j!=0)
		{//*** BELLY ******************
		MY_POINT	m;
			for(i=0;i<25;i++)
			{
				if(PictureToMap(m_curent,&m_row_ord[i],&m,GetOposit(m_facing)))
				{
					if(m_map[m.x][m.y].type==CL_FREE)
					{//Prepaire droping points
						CopyPoint(&droplist[drop].p_map,&m);
						CopyPoint(&droplist[drop].p_pic,&m_row_ord[i]);
						drop++;
					}

					if(j==drop)
					{//Possible to drop all(So let's drop)
						SetDirection(robot,GetOposit(m_facing));
						for(j=0;j<drop;j++)
						{//********** DROP **********

							GetBelStringTag(stag,cat_item[bi[j]].ntag,cat_item[bi[j]].type);
							if(DoDrop(robot,droplist[j].p_pic,stag)==RET_SUCCESS)
							{
								if(j==0)
								{//For reuse drop item tag(One tag enogh)
									memcpy(aitem.stag,stag,4);
									aitem.ntag = cat_item[bi[0]].ntag;
								}
								RemoveFromStock(MODE_COLOR,cat_item[bi[j]].ntag,cat_item[bi[j]].type);
								m_map[droplist[j].p_map.x][droplist[j].p_map.y].h	   = cat_item[bi[j]].h;
								m_map[droplist[j].p_map.x][droplist[j].p_map.y].type   = CL_BLACK;
								m_map[droplist[j].p_map.x][droplist[j].p_map.y].realbk = INVALIED;//To prevent helth decay
							}
						}//********** DROP **********
						ddone = true;
						SetDirection(robot,GetOposit(m_facing));
						break;
					}
				}
			}
		}//*** BELLY ******************

		if(k!=0)
		{//*** SECURE ******************
			drop = k;
			if(j==0)
			{//No belly
				ddone = true;
				//For reuse sec item tag
				aitem.ntag = cat_item[si[0]].ntag;
				GetSecStringTag(aitem.stag,aitem.ntag);
				//Remove from secure
				for(k=0;k<drop;k++)
					RemoveFromSecure(cat_item[si[k]].ntag);
			}else{
				if(ddone)
				{//There are bellys and they successfuly droped so continue with secure
					//Remove from secure
					for(k=0;k<drop;k++)
						RemoveFromSecure(cat_item[si[k]].ntag);
				}
			}
		}//*** SECURE ******************

		if(ddone)//Successfuly handle stores so pick heigh value pack
		{//********** PICK *********
			aitem.h		 = m_map[map.x][map.y].h;
			aitem.type   = m_map[map.x][map.y].type;
			if(DoPickUp(robot,pic,aitem.stag))//Tag is reusing
			{
				AddToStock(MODE_COLOR,&aitem);
				m_map[map.x][map.y].h	   = 0;
				m_map[map.x][map.y].type   = CL_FREE;
				return RET_SUCCESS;
			}
		}//********** PICK **********
	}

	m_map[map.x][map.y].type   = CL_BLACK;
	m_map[map.x][map.y].realbk = INVALIED;//To prevent helth decay
	return RET_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyExchange::IsBlackFeasible()
{
MY_POINT	pic,map;
BYTE		bst,st = 0;;
BYTE		i  = 0;
bool		bfound = false;

	for (int y=4;y>=0;y--)
	{
		for (int x=0;x<=4;x++)
		{	
			pic.x=x;pic.y=y;
			if(PictureToMap(m_curent,&pic,&map,m_facing))
			{
				i++;
				if(m_map[map.x][map.y].type==CL_BLACK)
					bfound = true;
				if((m_map[map.x][map.y].type==CL_WALL)||(m_map[map.x][map.y].type==CL_UNKNOWN)||(m_map[map.x][map.y].type==CL_DOOR))
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
//-------------------------------------------------------------------------------------------------
bool CMyExchange::GetOldTag(MY_ITEM *item)
{
	for(BYTE i=0;i<25;i++)
	{
		if(m_tags[i].ntag!=0)
		{
			item->ntag = m_tags[i].ntag;
			memcpy(item->stag,m_tags[i].stag,4);
			m_tags[i].ntag = 0;
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------
void CMyExchange::SaveOldTag(MY_ITEM *item)
{
	for(BYTE i=0;i<25;i++)
	{
		if(m_tags[i].ntag==0)
		{
			m_tags[i].ntag = item->ntag;
			memcpy(m_tags[i].stag,item->stag,4);
			return;
		}
	}
}