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
BYTE CMyExchange::PickSecured(CRobot &robot,bool chkloc,BYTE depth)
{
MY_ITEM	 item;
MY_POINT map;

	if(chkloc)
	{//Only pick from secured location (Prevent bat usage unevenness)
		//this is also check in IsSecured() for each pack
		if((m_sec_pic_loc.loc[m_sec_pic_loc.count-1].p.x==m_curent.x)&&(m_sec_pic_loc.loc[m_sec_pic_loc.count-1].p.y==m_curent.y)&&(m_sec_pic_loc.loc[m_sec_pic_loc.count-1].d==m_facing))
			m_sec_pic_loc.count--;
		else
			return RET_SUCCESS;
	}

	for(BYTE i=0;i<25;i++)
	{
		if(PictureToMapEx(&m_row_ord.p[i],&map))
		{
			item.type = m_map[map.x][map.y].type;
			item.h	  = m_map[map.x][map.y].h;
			if((item.type==CL_YELLOW)||(item.type==CL_PINK)||(item.type==CL_BROWN))
			{
				if((depth*5)-1<i)//Pick only to dirty depth(When call by GoForword())
					return RET_SUCCESS;

				if(IsSecured(map,&item,chkloc,&m_curent))//Old tag reuse
				{//Only pick secured one **************************
					if(chkloc==false)
					{//When call by GoForword()
						if(CheckBat(0,GetColorHeight()+item.h)==RET_STOP_MIS)
							return RET_STOP_MIS;
					}
					//Remove From Secure
					RemoveFromSecure(item.ntag);
					//Pick
					if(DoPickUp(robot,m_row_ord.p[i],item.stag)==RET_SUCCESS)
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
BYTE	level;


	for(BYTE i=0;i<=count;i++)
	{
		level = GetStock(ST_COL_SEC);
		if(level==120)
		{//No space to pic let's exchange
			if(ExchangeUnBalance(robot,po[i].p_pic,po[i].p_map)==RET_STOP_MIS)
				return RET_STOP_MIS;//Low Bat
		}else{
			if((level+po[i].h)>120)
			{//Height not fit
			BYTE ret;
				ret = FillBelly(robot,po[i].p_pic,po[i].p_map,level);
				if(ret==RET_ERROR)
				{
					if(ExchangeUnBalance(robot,po[i].p_pic,po[i].p_map)==RET_STOP_MIS)
						return RET_STOP_MIS;//Low Bat
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
						//Clear it from vertual map
						m_map[po[i].p_map.x][po[i].p_map.y].type = CL_FREE; 
						m_map[po[i].p_map.x][po[i].p_map.y].h	 = 0;
						//Check score
						if(GetScore()==2400)
							return RET_STOP_MIS;
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
				if(DoPickUp(robot,m_row_ord.p[i],item.stag)==RET_SUCCESS)
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
		item.realbk = m_map[map.x][map.y].realbk;
		AddToStock(MODE_BLACK,&item);
		//Cal rem helth
		if(m_rem_hel!=0)
		{
			if(m_map[map.x][map.y].realbk==VALIED)//Not a painted one
				m_rem_hel--;//Drop helth
		}
		//Clear it from vertual map
		m_map[map.x][map.y].type   = CL_FREE;
		m_map[map.x][map.y].realbk = INVALIED;
		m_map[map.x][map.y].h	   = 0;
		return RET_SUCCESS;
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
			if(PictureToMapEx(&m_row_ord.p[i],&map))
			{
				if((m_map[map.x][map.y].type==CL_FREE)&&(m_map[map.x][map.y].dirty!=CL_BLOCK))
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

			if(PictureToMapEx(&m_row_ord.p[i],&map))
			{
				if((m_map[map.x][map.y].type==CL_FREE)&&(m_map[map.x][map.y].dirty!=CL_BLOCK))
				{
				MY_ITEM  ditem;

					if(mode==MODE_DROP_SECURE)
					{
						if(CheckBat(0,GetColorHeight())==RET_STOP_MIS)
						{
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
						m_map[map.x][map.y].realbk = ditem.realbk;
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
				if(CheckBat(0,GetColorHeight()+m_map[map.x][map.y].h-dh)!=RET_STOP_MIS)
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
							m_map[droplist[j].p_map.x][droplist[j].p_map.y].type   = CL_BLACK;
							m_map[droplist[j].p_map.x][droplist[j].p_map.y].realbk = INVALIED;//To prevent helth decay
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
				m_map[map.x][map.y].h	   = 0;
				m_map[map.x][map.y].type   = CL_FREE;
				#ifdef _DEBUG
				MessageBox(NULL,"Fill done !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
				#endif
				//Check score
				if(GetScore()==2400)
					return RET_STOP_MIS;
				else
					return RET_SUCCESS;//return true;
			}//********** PICK **********
		}
	}//Fill possible

	if(k!=0)
	{//Fill fail
		drop = k;
		//Re-store blacked secure map item
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
BYTE		drop,eqcount,count,i,loss,eqloss;

	drop = eqcount = count = k = j = loss = eqloss = 0;

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
	if((eqloss<loss)&&(eqloss!=0))
	{//true method choosen
		for(i=1;i<eqcount;i++)
			memcpy(cat_item,equal_item,sizeof(MY_CAT_ITEM));
		count = eqcount;
		loss  = eqloss;
	}
	
	if(count!=0)
	{//Exchange possible
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

		if(loss<m_map[map.x][map.y].h*m_map[map.x][map.y].type)
		{//Exchang has some gain
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
					if(CheckBat(0,GetColorHeight()+m_map[map.x][map.y].h-dh)!=RET_STOP_MIS)
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
								m_map[droplist[j].p_map.x][droplist[j].p_map.y].type   = CL_BLACK;
								m_map[droplist[j].p_map.x][droplist[j].p_map.y].realbk = INVALIED;//To prevent helth decay
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
					m_map[map.x][map.y].h	   = 0;
					m_map[map.x][map.y].type   = CL_FREE;
					#ifdef _DEBUG
					//MessageBox(NULL,"Exchange !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
					//Beep(12000,10);
					#endif
					//Check score
					if(GetScore()==2400)
						return RET_STOP_MIS;
					else
						return RET_SUCCESS;
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

	return RET_ERROR;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyExchange::ExchangeUnBalance(CRobot &robot,MY_POINT pic,MY_POINT map)
{
MY_CAT_ITEM cat_item[6];
BYTE		count,i,drop;
BYTE		si[5],k;//secured
BYTE		bi[5],j;//belly

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
				if(CheckBat(0,GetColorHeight()+m_map[map.x][map.y].h-dh)!=RET_STOP_MIS)
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
							m_map[droplist[j].p_map.x][droplist[j].p_map.y].type   = CL_BLACK;
							m_map[droplist[j].p_map.x][droplist[j].p_map.y].realbk = INVALIED;//To prevent helth decay
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
				m_map[map.x][map.y].h	   = 0;
				m_map[map.x][map.y].type   = CL_FREE;
				#ifdef _DEBUG
				MessageBox(NULL,"Un !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
				//Beep(12000,10);
				#endif
				//Check score
				if(GetScore()==2400)
					return RET_STOP_MIS;
				else
					return RET_SUCCESS;
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
		//Black exchange fail pack
		m_map[map.x][map.y].type   = CL_BLACK;
		m_map[map.x][map.y].realbk = INVALIED;//To prevent helth decay
		if(ret==RET_STOP_MIS)
			return RET_STOP_MIS;
	}

	return RET_ERROR;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyExchange::IsBlackFeasible(bool chkdirty)
{//Secure items not consider - so always not correct
MY_POINT	pic,map;
BYTE		bst,st,i,dc;
bool		bfound = false;
	
	st = i = dc = 0;

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
				//Count dirty cells 
				if((m_map[map.x][map.y].dirty==CL_DIRTY)||(m_map[map.x][map.y].dirty==CL_BLOCK))
					dc++;
				if((m_map[map.x][map.y].type==CL_WALL)||(m_map[map.x][map.y].type==CL_UNKNOWN)||(m_map[map.x][map.y].type==CL_DOOR))
				{//Blocking object
					bst = st;
					goto LABLE_ONE;
				}
			}
		}

		if(chkdirty)
		{
			if(dc==5)//Full raw is dirty
				goto LABLE_ONE;
			dc = 0;
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


