// MyExchange.cpp: implementation of the CMyExchange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBatAgent.h"
#include "MyExchange.h"

CMyExchange::~CMyExchange(){}
//--------------------------------------------------------------------------------------------
CMyExchange::CMyExchange()
{//Initialize reuse tag pool
	for(BYTE i=0;i<25;i++)
		m_tags[i].ntag = 0;
}
//--------------------------------------------------------------------------------------------
BYTE CMyExchange::PickSecured(CRobot &robot,bool chkloc,BYTE depth)
{
MY_PICK_ORD po[25];;
MY_ITEM		item[25];
MY_POINT	map;
BYTE		i,th,count;

	th = count = 0;

	if(m_max_score==false)
	{
		for(i=0;i<25;i++)
		{
			if(PictureToMapEx(&m_row_ord.p[i],&map))
			{
				//Try to exchange free color
				if(m_map[map.x][map.y].free==VALIED)
				{
					if(ExchangeUnBalance(robot,m_row_ord.p[i],map)==RET_STOP_MIS)
						break;
				}
			}
		}
	}

	if(chkloc)
	{//Only pick from secured location (Prevent bat usage unevenness)
		//this is also check in IsSecured() for each pack
		if((m_sec_pic_loc.loc[m_sec_pic_loc.count-1].p.x==m_curent.x)&&(m_sec_pic_loc.loc[m_sec_pic_loc.count-1].p.y==m_curent.y)&&(m_sec_pic_loc.loc[m_sec_pic_loc.count-1].d==m_facing))
		{
			m_sec_pic_loc.count--;
		}else{
			return RET_SUCCESS;
		}
	}

	for(i=0;i<25;i++)
	{
		if(PictureToMapEx(&m_row_ord.p[i],&map))
		{
			if(m_row_ord.st[i]==depth)
				break;//Pick only to dirty depth(When call by GoForword())

			item[count].type = m_map[map.x][map.y].type;
			item[count].h	 = m_map[map.x][map.y].h;

			if((item[count].type==CL_YELLOW)||(item[count].type==CL_PINK)||(item[count].type==CL_BROWN))
			{
				if(IsSecured(map,&item[count],chkloc,&m_curent))//Old tag reuse
				{//Only pick secured one **************************
					if(chkloc==false)
					{//When call by GoForword() - BR11 MAP4 BAT 125000
						for(BYTE b=0;b<=m_blacklist[0].steps;b++)
						{//Go through black list
							if(!((m_blacklist[b].loc.x==m_curent.x)&&(m_blacklist[b].loc.y==m_curent.y)&&(m_blacklist[b].came_d==m_facing)))
							{//Not in black list
								if(CheckBat(0,GetColorHeight()+th+item[count].h)==RET_STOP_MIS)
								{//Add to black list (Check CMyMap::GetFeasible())
									m_blacklist[m_blacklist[0].steps].loc.x = m_curent.x;
									m_blacklist[m_blacklist[0].steps].loc.y = m_curent.y;
									m_blacklist[m_blacklist[0].steps].came_d= m_facing;
									m_blacklist[0].steps++;//First item holds the count
									return RET_ERROR;
								}
							}else{
								return RET_ERROR;
							}
						}
					}
					//Fill picking order
					po[count].h		  = m_map[map.x][map.y].h;
					th				 += po[count].h;
					po[count].p_map.x = map.x;
					po[count].p_map.y = map.y;
					po[count].p_pic.x = m_row_ord.p[i].x;
					po[count].p_pic.y = m_row_ord.p[i].y;
					count++;
				}//Only pick secured one **************************
			}
		}
	}

	//Actual Pick using picking order
	for(i=0;i<count;i++)
	{
		//Remove From Secure
		RemoveFromSecure(item[i].ntag);
		//Pick
		if(DoPickUp(robot,po[i].p_pic,item[i].stag)==RET_SUCCESS)
		{
			//Add to store
			AddToStock(MODE_COLOR,&item[i]);
			//Free location
			FreeLocation(&po[i].p_map);
		}
	}

	return RET_SUCCESS;
}
//--------------------------------------------------------------------------------------------
BYTE CMyExchange::PickColor(CRobot &robot,MY_PICK_ORD *po,BYTE count)
{//SmartPick Helper
MY_ITEM item;
BYTE	level;

	for(BYTE i=0;i<=count;i++)
	{
		level = GetStock(ST_COL_SEC);
		if(level==120)
		{//No space to pic let's exchange
			if(ExchangeUnBalance(robot,po[i].p_pic,po[i].p_map)==RET_STOP_MIS)
				return RET_STOP_MIS;
		}else{
			if((level+po[i].h)>120)
			{//Height not fit
			BYTE ret;
				ret = FillBelly(robot,po[i].p_pic,po[i].p_map,level);
				if(ret==RET_ERROR)
				{
					if(ExchangeUnBalance(robot,po[i].p_pic,po[i].p_map)==RET_STOP_MIS)
						return RET_STOP_MIS;
				}else{
						if(ret==RET_STOP_MIS)
							return RET_STOP_MIS;//Max score
				}
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
						//Erase from map
						FreeLocation(&po[i].p_map);
						//Check score
						if(GetScore()==2400)
						{
							m_max_score = true;
							return RET_STOP_MIS;
						}
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
		if(PictureToMapEx(&m_row_ord.p[i],&p))
		{
			item.type = m_map[p.x][p.y].type;
			item.h	  = m_map[p.x][p.y].h;
			if((item.type==CL_YELLOW)||(item.type==CL_PINK)||(item.type==CL_BROWN))
			{//************************************************
				if(m_map[p.x][p.y].free==INVALIED)
				{//Not a free pack
				if(IsSecured(p,0,false,0)==false)
				{//Not secure
					//Check with current stock
					if((GetStock(ST_COL_BK)+item.h)>120)
					{//No storage space
						DropBlackPacks(robot,GetColorHeight(),MODE_CHECK_HELTH);
					}

					if(GetStock(ST_COL_BK)+item.h>120)
						return RET_SUCCESS;

					if(GetOldTag(&item)==false)
						MakeTag(item.stag,&item.ntag);//Make a new tag

					//Lest,s pick
					if(DoPickUp(robot,m_row_ord.p[i],item.stag)==RET_SUCCESS)
					{
						//Add to store
						AddToStock(MODE_COLOR,&item);
						//Erase from map
						FreeLocation(&p);
					}
				}
				}
			}//************************************************
		}
	}

	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::PickColor(MY_POINT pic,MY_POINT map,CRobot &robot,BYTE dropd)
{//Pass free color
MY_ITEM  item;

	item.type = m_map[map.x][map.y].type;
	item.h	  = m_map[map.x][map.y].h;

	if(GetStock(ST_COL_BK)+item.h>120)
	{//No room so drop some color
		DropColorPacks(robot,dropd,10,MODE_DROP_SPACE);
	}

	if(GetStock(ST_COL_BK)+item.h>120)
		return RET_SUCCESS;

	if(GetOldTag(&item)==false)
		MakeTag(item.stag,&item.ntag);//Make a new tag

	//Lest,s pick
	if(DoPickUp(robot,pic,item.stag)==RET_SUCCESS)
	{
		//Add to store
		AddToStock(MODE_FREE_COLOR,&item);
		//Erase from map
		FreeLocation(&map);
		return RET_SUCCESS;
	}

	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::PickBlack(MY_POINT pic,MY_POINT map,CRobot &robot,BYTE dropd,BYTE mode)
{
MY_ITEM  item;

	item.type = m_map[map.x][map.y].type;
	item.h	  = m_map[map.x][map.y].h;

	if(GetStock(ST_COL_BK)+m_map[map.x][map.y].h>120)
	{//No room for blackies so drop some color
		DropColorPacks(robot,dropd,10,MODE_DROP_SPACE);
	}

	if(GetStock(ST_COL_BK)+item.h>120)
		return RET_SUCCESS;

	if(GetOldTag(&item)==false)
		MakeTag(item.stag,&item.ntag);//Make a new tag

	//Lest,s pick
	if(DoPickUp(robot,pic,item.stag)==RET_SUCCESS)
	{
		//Add to store
		AddToStock(MODE_BLACK,&item);
		//Cal rem helth
		if(m_rem_hel!=0)
				m_rem_hel--;//Drop helth

		//Erase from map
		FreeLocation(&map);
		return RET_SUCCESS;
	}

	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::DropBlackPacks(CRobot &robot,int oldcol,BYTE mode)
{
MY_POINT map;
MY_ITEM  item;
bool	 b = false;

	for(BYTE i=0;i<25;i++)
	{
		if(IsBlackOver()==false)
		{
			if(PictureToMapEx(&m_row_ord.p[i],&map))
			{
				if((m_map[map.x][map.y].type==CL_FREE)&&(m_map[map.x][map.y].dirty!=CL_BLOCK))
				{
					if(mode==MODE_CHECK_HELTH)
					{
						if(m_rem_hel==SendAgent()){
							#ifdef _DEBUG
							MessageBox(NULL,"Drop Nohelth !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
							#endif
							return RET_SUCCESS;//No helth
						}else{
							b = true;}
					}else{
						b = true;
					}

					if(b)
					{
						ReleaseMaxHeight(&item,ST_BLACK);
						//Drop
						if(DoDrop(robot,m_row_ord.p[i],item.stag)==RET_SUCCESS)
						{
							//Save this tag for later use
							SaveOldTag(&item);
							//Reduce storage
							RemoveFromStock(MODE_BLACK,item.ntag);
							//Place it in vertual map
							m_map[map.x][map.y].type   = item.type;
							m_map[map.x][map.y].free   = INVALIED;
							m_map[map.x][map.y].h	   = item.h;
						}
					}
				}
			}
		}else{
			//All collected blacks proped
			break;
		}
	}

return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::DropFreeColor(CRobot &robot, int oldcol)
{
MY_POINT map;
MY_ITEM  item;

	for(BYTE i=0;i<25;i++)
	{
		if(IsFreeColOver()==false)
		{
			if(PictureToMapEx(&m_row_ord.p[i],&map))
			{
				if((m_map[map.x][map.y].type==CL_FREE)&&(m_map[map.x][map.y].dirty!=CL_BLOCK))
				{
					ReleaseMaxHeight(&item,ST_FREE_COLOR);
					//Drop
					if(DoDrop(robot,m_row_ord.p[i],item.stag)==RET_SUCCESS)
					{
						//Save this tag for later use
						SaveOldTag(&item);
						//Reduce storage
						RemoveFromStock(MODE_FREE_COLOR,item.ntag);
						//Place it in vertual map
						m_map[map.x][map.y].type   = item.type;
						m_map[map.x][map.y].free   = VALIED;
						m_map[map.x][map.y].h	   = item.h;
					}
				}
			}
		}else{
			//All collected free color droped
			break;
		}
	}

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

			if(PictureToMapEx(&m_row_ord.p[i],&map))
			{
				if((m_map[map.x][map.y].type==CL_FREE)&&(m_map[map.x][map.y].dirty!=CL_BLOCK))
				{
				MY_ITEM  ditem;

					if(mode==MODE_DROP_SECURE)
					{
						if(CheckBat(0,GetColorHeight())==RET_STOP_MIS)
						{//No battory
							if(dh!=0)
							{
								//Save sec location
								m_sec_pic_loc.loc[m_sec_pic_loc.count].p.x = m_curent.x;
								m_sec_pic_loc.loc[m_sec_pic_loc.count].p.y = m_curent.y;
								m_sec_pic_loc.loc[m_sec_pic_loc.count].d   = m_facing;
								m_sec_pic_loc.count++;
								PickSecured(robot,true);//Map BR11-1 bat-125000
							}
							SetDirection(robot,d);
							return RET_STOP_MIS;
						}
					}

					ReleaseMaxHeight(&ditem,ST_COLOR);

					if(DoDrop(robot,m_row_ord.p[i],ditem.stag)==RET_SUCCESS)
					{
						RemoveFromStock(MODE_COLOR,ditem.ntag,ditem.type);
						//Is it secured
						if(mode==MODE_DROP_SECURE)
						{
							AddToSecureList(map,&ditem);
						}else{
							SaveOldTag(&ditem);//Secure tags are reusing
						}

						//Cal droped color height
						dh += ditem.h;
						m_map[map.x][map.y].type   = ditem.type;
						m_map[map.x][map.y].free   = INVALIED;
						m_map[map.x][map.y].h	   = ditem.h;
					}
				}
			}
		}
	}

	if(mode==MODE_DROP_SECURE)
	{
		if(dh!=0)
		{
			//Save sec location
			m_sec_pic_loc.loc[m_sec_pic_loc.count].p.x = m_curent.x;
			m_sec_pic_loc.loc[m_sec_pic_loc.count].p.y = m_curent.y;
			m_sec_pic_loc.loc[m_sec_pic_loc.count].d   = m_facing;
			m_sec_pic_loc.count++;
		}
	}
	SetDirection(robot,d);

	return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::FillBelly(CRobot &robot,MY_POINT pic,MY_POINT map,BYTE level)
{
MY_CAT_ITEM cat_item[6];
BYTE		si[5],k,drop,count;

	k = drop = count = 0;

	cat_item[0].type = m_map[map.x][map.y].type;
	cat_item[0].h	 = m_map[map.x][map.y].h-(120-level);

	if(ReleaseExchange(cat_item,&count,true))
	{//Fill possible
	MY_PICK_ORD droplist[5];
	MY_ITEM		aitem;
	BYTE		bi[5],i,j,loss;
	char		stag[4];
	bool		ddone=false;

		j = loss = 0;
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
			loss += cat_item[i].h*cat_item[i].type;
		}
		//Check whether this is profitable
		//BYTE g = m_map[map.x][map.y].h*m_map[map.x][map.y].type;
		if(loss>=m_map[map.x][map.y].h*m_map[map.x][map.y].type)
		{
			#ifdef _DEBUG
				MessageBox(NULL,"Point less drop !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
			#endif
			return RET_ERROR;//return false;
		}

		if(k!=0)
		{//*** SECURE ******************
			drop = k;
			//For reuse sec item tag
			aitem.ntag = cat_item[si[0]].ntag;
			GetSecStringTag(aitem.stag,aitem.ntag);
			//Only mark this pack as free
			for(k=0;k<drop;k++)
				RemoveFromSecure(cat_item[si[k]].ntag,false);
			k = drop;
			if(j==0)
			{//No Belly
				if(CheckBat(0,GetColorHeight()+m_map[map.x][map.y].h)!=RET_STOP_MIS)
				{
					ddone = true;
				}else{
					#ifdef _DEBUG
					MessageBox(NULL,"FillBelly LowBat!","Marc",MB_OK|MB_ICONEXCLAMATION);
					#endif
				}
			}
		}//*** SECURE ******************

		if(j!=0)
		{//*** BELLY ******************
		MY_POINT m;
		drop = 0;
			for(i=0;i<25;i++)
			{
				if(PictureToMap(m_curent,&m_row_ord.p[i],&m,GetOposit(m_facing)))
				{
					if((m_map[m.x][m.y].type==CL_FREE)&&(m_map[map.x][map.y].dirty!=CL_BLOCK))
					{//Prepaire droping points
						CopyPoint(&droplist[drop].p_map,&m);
						CopyPoint(&droplist[drop].p_pic,&m_row_ord.p[i]);
						drop++;
						if(j==drop)
							break;//Possible to drop all
					}
				}
			}

			if(j==drop)
			{//Possible to drop all
			BYTE dh = 0;
				//Cal drop height
				for(j=0;j<drop;j++)
					dh += cat_item[bi[j]].h;
				//CheckBat
				if(CheckBat(0,GetColorHeight()+cat_item[0].h-dh)!=RET_STOP_MIS)
				{
					//********** DROP **********
					SetDirection(robot,GetOposit(m_facing));
					for(j=0;j<drop;j++)
					{
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
							m_map[droplist[j].p_map.x][droplist[j].p_map.y].type   = cat_item[bi[j]].type;
							m_map[droplist[j].p_map.x][droplist[j].p_map.y].free   = VALIED;
						}
					}
					ddone = true;
					SetDirection(robot,GetOposit(m_facing));
					//********** DROP **********
				}else{
					#ifdef _DEBUG
					MessageBox(NULL,"FillBelly LowBat!","Marc",MB_OK|MB_ICONEXCLAMATION);
					#endif
				}
			}else{
				#ifdef _DEBUG
				MessageBox(NULL,"Drop fail !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
				#endif
			}
		}//*** BELLY ******************

		if(ddone)//Successfuly handle stores so pick heigh value pack
		{
			if(k!=0)
			{
				drop = k;
				//Remove from secure store
				for(k=0;k<drop;k++)
					RemoveFromSecure(cat_item[si[k]].ntag,true);
			}
			//********** PICK *********
			aitem.h		 = m_map[map.x][map.y].h;
			aitem.type   = m_map[map.x][map.y].type;
			if(DoPickUp(robot,pic,aitem.stag))//Tag is reusing
			{
				AddToStock(MODE_COLOR,&aitem);
				//Erase from map
				FreeLocation(&map);
				/*#ifdef _DEBUG
				MessageBox(NULL,"Fill done !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
				#endif*/
				//Check score
				if(GetScore()==2400)
				{
					m_max_score = true;
					return RET_STOP_MIS;
				}else{
					return RET_SUCCESS;//return true;
				}
			}//********** PICK **********
		}
	}//Fill possible

	if(k!=0)
	{//Fill fail
		drop = k;
		//Re-store free secure map item(s)
		for(k=0;k<drop;k++)
			RestoreSecurePacks(cat_item[si[k]].ntag);
	}

	return RET_ERROR;//return false;
}
//------------------------------------------------------------------------------------------------
BYTE CMyExchange::ExchangeBalance(CRobot &robot,MY_POINT pic,MY_POINT map)
{
MY_CAT_ITEM cat_item[6],equal_item[6];
BYTE		si[5],k;//secured
BYTE		bi[5],j;//belly
BYTE		drop,eqcount,count,i,loss,eqloss,ret = RET_ERROR;

	drop = loss = eqcount = count = k = j = eqloss = 0;

	equal_item[0].type = cat_item[0].type = m_map[map.x][map.y].type;
	equal_item[0].h	   = cat_item[0].h	  = m_map[map.x][map.y].h;

	//Try two exchange methods
	if(ReleaseExchange(equal_item,&eqcount,true))
	{
		for(i=1;i<eqcount;i++)
			eqloss += equal_item[i].h*equal_item[i].type;
	}
	if(ReleaseExchange(cat_item,&count,false))
	{
		for(i=1;i<count;i++)
			loss += cat_item[i].h*cat_item[i].type;
	}

	//Choose best one
	if((eqloss<loss)&&(eqloss!=0)&&(loss!=0))
	{//true method choosen
		for(i=1;i<eqcount;i++)
			memcpy(&cat_item[i],&equal_item[i],sizeof(MY_CAT_ITEM));
		count = eqcount;
		loss  = eqloss;
	}/*else{
		if((loss==0)&&(eqloss!=0))
		{
			for(i=1;i<eqcount;i++)
				memcpy(&cat_item[i],&equal_item[i],sizeof(MY_CAT_ITEM));
			count = eqcount;
			loss  = eqloss;
		}
	}*/

	if(count!=0)
	{//Exchange possible
	MY_PICK_ORD droplist[5];
	MY_ITEM		aitem;
	char		stag[4];
	bool		ddone = false;

		if(loss<m_map[map.x][map.y].h*m_map[map.x][map.y].type)
		{//Exchang has some gain
			//Separate belly and secure items
			for(i=1;i<count;i++)
			{//(BR10-4 Bat 125000 Do both Bet and Sec Exchange)
				if(cat_item[i].store==STORE_SEC)
				{
					si[k] = i;
					k++;
				}else{
					bi[j] = i;
					j++;
				}
			}

			if(k!=0)
			{//*** SECURE ******************
				drop = k;
				//For reuse sec item tag
				aitem.ntag = cat_item[si[0]].ntag;
				GetSecStringTag(aitem.stag,aitem.ntag);
				//Only black secured packs(Not removing from stock - remove after every thing completed)
				for(k=0;k<drop;k++)
					RemoveFromSecure(cat_item[si[k]].ntag,false);
				k = drop;
				if(j==0)
				{//No Belly
					ret = CheckBat(0,GetColorHeight()+m_map[map.x][map.y].h);
					if(ret!=RET_STOP_MIS)
					{
						ddone = true;
					}else{
						#ifdef _DEBUG
						MessageBox(NULL,"Bal LowBat!","Marc",MB_OK|MB_ICONEXCLAMATION);
						#endif
					}
				}
			}//*** SECURE ******************

			if(j!=0)
			{//*** BELLY ******************
			MY_POINT m;
			drop = 0;
				for(i=0;i<25;i++)
				{
					if(PictureToMap(m_curent,&m_row_ord.p[i],&m,GetOposit(m_facing)))
					{
						if((m_map[m.x][m.y].type==CL_FREE)&&(m_map[map.x][map.y].dirty!=CL_BLOCK))
						{//Prepaire droping points
							CopyPoint(&droplist[drop].p_map,&m);
							CopyPoint(&droplist[drop].p_pic,&m_row_ord.p[i]);
							drop++;
							if(j==drop)
								break;//Possible to drop all
						}
					}
				}

				if(j==drop)
				{//Possible to drop all
				BYTE dh = 0;
					//Cal drop height
					for(j=0;j<drop;j++)
						dh += cat_item[bi[j]].h;
					//CheckBat
					ret = CheckBat(0,GetColorHeight()+m_map[map.x][map.y].h-dh);
					if(ret!=RET_STOP_MIS)
					{
						//********** DROP **********
						SetDirection(robot,GetOposit(m_facing));
						for(j=0;j<drop;j++)
						{
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
								m_map[droplist[j].p_map.x][droplist[j].p_map.y].type   = cat_item[bi[j]].type;
								m_map[droplist[j].p_map.x][droplist[j].p_map.y].free   = VALIED;
							}
						}
						ddone = true;
						SetDirection(robot,GetOposit(m_facing));
						//********** DROP **********
					}else{
						#ifdef _DEBUG
						MessageBox(NULL,"Bal LowBat!","Marc",MB_OK|MB_ICONEXCLAMATION);
						#endif
					}
				}else{
					#ifdef _DEBUG
					MessageBox(NULL,"Bal Drop fail !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
					#endif
				}
			}//*** BELLY ******************

			if(ddone)//Successfuly handle stores so pick heigh value pack
			{
				if(k!=0)
				{
					drop = k;
					//Remove from secure store
					for(k=0;k<drop;k++)
						RemoveFromSecure(cat_item[si[k]].ntag,true);
				}

			 //********** PICK *********
				aitem.h		 = m_map[map.x][map.y].h;
				aitem.type   = m_map[map.x][map.y].type;
				if(DoPickUp(robot,pic,aitem.stag))//Tag is reusing
				{
					AddToStock(MODE_COLOR,&aitem);
					//Erase from map
					FreeLocation(&map);
					/*#ifdef _DEBUG
					MessageBox(NULL,"Exchange !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
					#endif*/
					//Check score
					if(GetScore()==2400)
					{
						m_max_score = true;
						return RET_STOP_MIS;
					}else{
						return RET_SUCCESS;
					}
				}
			}//********** PICK **********
		}
	}//Exchange possible

	if(k!=0)
	{//Exchange fail
		drop = k;
		//Re-store blacked secure map item
		for(k=0;k<drop;k++)
			RestoreSecurePacks(cat_item[si[k]].ntag);
	}
	if(ret==RET_STOP_MIS)
		return RET_STOP_MIS;
	else
		return RET_ERROR;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyExchange::ExchangeUnBalance(CRobot &robot,MY_POINT pic,MY_POINT map)
{
MY_CAT_ITEM cat_item[6];
BYTE		count,i,drop;
BYTE		si[5],k;//secured
BYTE		bi[5],j;//belly

	if(m_max_score)
	{//Nothing to do with those packs
		m_map[map.x][map.y].free = VALIED;//When call ExchangeUnBalance() by PicSecure() RET_STOP_MIS return dosen't ditect 
		return RET_STOP_MIS;
	}

	count = k = j = 0;
	cat_item[0].type = m_map[map.x][map.y].type;
	cat_item[0].h	 = m_map[map.x][map.y].h;

	if(ReleaseTail(cat_item,&count))
	{
	MY_PICK_ORD droplist[5];
	MY_ITEM		aitem;
	char		stag[4];
	bool		ddone = false;

		//Separate belly and secure items
		for(i=1;i<count;i++)
		{//(BR10-4 Bat 125000 Do both Bet and Sec Exchange)
			if(cat_item[i].store==STORE_SEC)
			{
				si[k] = i;
				k++;
			}else{
				bi[j] = i;
				j++;
			}
		}

		if(k!=0)
		{//*** SECURE ******************
			drop = k;
			//For reuse sec item tag
			aitem.ntag = cat_item[si[0]].ntag;
			GetSecStringTag(aitem.stag,aitem.ntag);
			//Only black secured packs(Not removing from stock - remove after every thing completed)
			for(k=0;k<drop;k++)
				RemoveFromSecure(cat_item[si[k]].ntag,false);
			k = drop;
			if(j==0)
			{//No Belly
				if(CheckBat(0,GetColorHeight()+m_map[map.x][map.y].h)!=RET_STOP_MIS)
				{
					ddone = true;
				}else{
					#ifdef _DEBUG
					MessageBox(NULL,"Un LowBat!","Marc",MB_OK|MB_ICONEXCLAMATION);
					#endif
				}
			}
		}//*** SECURE ******************

		if(j!=0)
		{//*** BELLY ******************
		MY_POINT m;
		drop = 0;
			for(i=0;i<25;i++)
			{
				if(PictureToMap(m_curent,&m_row_ord.p[i],&m,GetOposit(m_facing)))
				{
					if((m_map[m.x][m.y].type==CL_FREE)&&(m_map[map.x][map.y].dirty!=CL_BLOCK))
					{//Prepaire droping points
						CopyPoint(&droplist[drop].p_map,&m);
						CopyPoint(&droplist[drop].p_pic,&m_row_ord.p[i]);
						drop++;
						if(j==drop)
							break;//Possible to drop all
					}
				}
			}

			if(j==drop)
			{//Possible to drop all
			BYTE dh = 0;
				//Cal drop height
				for(j=0;j<drop;j++)
					dh += cat_item[bi[j]].h;
				//CheckBat
				if(CheckBat(0,GetColorHeight()+m_map[map.x][map.y].h)!=RET_STOP_MIS)
				{
					//********** DROP **********
					SetDirection(robot,GetOposit(m_facing));
					for(j=0;j<drop;j++)
					{
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
							m_map[droplist[j].p_map.x][droplist[j].p_map.y].type   = cat_item[bi[j]].type;
							m_map[droplist[j].p_map.x][droplist[j].p_map.y].free   = VALIED;
						}
					}
					ddone = true;
					SetDirection(robot,GetOposit(m_facing));
					//********** DROP **********
				}else{
					#ifdef _DEBUG
					MessageBox(NULL,"Un LowBat!","Marc",MB_OK|MB_ICONEXCLAMATION);
					#endif
				}
			}else{
				#ifdef _DEBUG
				MessageBox(NULL,"Un Drop fail !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
				#endif
			}
		}//*** BELLY ******************

		if(ddone)//Successfuly handle stores so pick heigh value pack
		{
			if(k!=0)
			{
				drop = k;
				//Remove from secure store
				for(k=0;k<drop;k++)
					RemoveFromSecure(cat_item[si[k]].ntag,true);
			}

		 //********** PICK *********
			aitem.h		 = m_map[map.x][map.y].h;
			aitem.type   = m_map[map.x][map.y].type;
			if(DoPickUp(robot,pic,aitem.stag))//Tag is reusing
			{
				AddToStock(MODE_COLOR,&aitem);
				//Erase from map
				FreeLocation(&map);
				/*#ifdef _DEBUG
				//MessageBox(NULL,"Un !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
				#endif*/
				//Check score
				if(GetScore()==2400)
				{
					m_max_score = true;
					return RET_STOP_MIS;
				}else{
					return RET_SUCCESS;
				}
			}
		}//********** PICK **********
	}//Exchange possible

	if(k!=0)
	{//Exchange fail
		drop = k;
		//Re-store blacked secure map item
		for(k=0;k<drop;k++)
			RestoreSecurePacks(cat_item[si[k]].ntag);
	}

BYTE ret;
	ret = ExchangeBalance(robot,pic,map);
	if(ret!=RET_SUCCESS)
	{
		//Origion of free color packs
		m_map[map.x][map.y].free = VALIED;
		if(ret==RET_STOP_MIS)
			return RET_STOP_MIS;
	}

	return RET_ERROR;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyExchange::IsPassFeasible(bool chkdirty)
{//Secure items not consider - so always not correct
MY_POINT	map;
BYTE		bst,st,dc;
BYTE		type,dirty;
bool		pfound = false;

	bst = st = dc = 0;

	for(BYTE i=0;i<25;i++)
	{
		if(PictureToMap(m_curent,&m_row_ord.p[i],&map,m_facing))
		{
			type  = m_map[map.x][map.y].type;
			dirty = m_map[map.x][map.y].dirty;

			//New raw
			if(m_row_ord.nr[i]==VALIED)
				dc = 0;
			//Count dirty cells
			if((dirty==CL_DIRTY)||(dirty==CL_BLOCK))
				dc++;
			//Full raw is dirty
			if(chkdirty)
			{
				if(dc==5)
				{
					if(pfound)
						return m_row_ord.st[i];
					else
						return 0;
				}
			}

			//Found something to pass
			if((m_map[map.x][map.y].free==VALIED)||(type==CL_BLACK))
			{
				//if(chkdirty)
				//{
					//if(dirty!=CL_DIRTY)
						//pfound = true;
				//}else{
					pfound = true;
				//}
			}

			if((type==CL_WALL)||(type==CL_UNKNOWN)||(type==CL_DOOR))
			{//Blocking object
				if(pfound)
				    return m_row_ord.st[i];
				else
					return 0;
			}
		}
	}

	if(pfound)
		return m_row_ord.st[i];
	else
		return 0;
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
//-------------------------------------------------------------------------------------------------
BYTE CMyExchange::SendAgent()
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



