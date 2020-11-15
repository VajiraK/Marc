// MyStock.cpp: implementation of the CMyStock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyStock.h"

CMyStock::~CMyStock(){}
//-------------------------------------------------------------------------------------------------
CMyStock::CMyStock(){}
//-------------------------------------------------------------------------------------------------
void CMyStock::AddToStock(BYTE mode,MY_ITEM *item)
{
BYTE in;

	switch(mode)
	{
	case MODE_COLOR://************************
		//Manage collected
		switch(item->type)
		{
		case CL_YELLOW:
			m_stock.map.yellow.collected++;
		break;
		case CL_PINK:
			m_stock.map.pink.collected++;
		break;
		case CL_BROWN:
			m_stock.map.brown.collected++;
		}

		if(m_stock.color.count==0)
		{//First item
			in = 0;
		}else{
			//Find the inserting point
			for(in=0;in<m_stock.color.count;in++)
			{
				if(m_stock.color.packs[in].type>=item->type)
					break;
			}
			//Make room for the new item (Move one index further)
			BYTE size = sizeof(MY_ST_ITEM);
			for(BYTE i=m_stock.color.count;i>in;i--)
				memcpy(&m_stock.color.packs[i],&m_stock.color.packs[i-1],size);
		}

		//Insert new item
		m_stock.color.packs[in].realbk = INVALIED;
		m_stock.color.packs[in].type   = item->type;
		m_stock.color.packs[in].h	   = item->h;
		m_stock.color.packs[in].ntag   = item->ntag;
		m_stock.color.totalh += item->h;
		memcpy(m_stock.color.packs[in].stag,item->stag,4);
		m_stock.color.count++;
		//Add to catalog 
		AddToCatalog(STORE_BEL,item);
		break;

	case MODE_BLACK://************************

		//Select a free item
		in = m_stock.black.count;
		//Add
		m_stock.black.totalh += item->h;
		m_stock.black.packs[in].realbk = item->realbk;
		m_stock.black.packs[in].type   = item->type;
		m_stock.black.packs[in].h	   = item->h;
		m_stock.black.packs[in].ntag   = item->ntag;
		memcpy(m_stock.black.packs[in].stag,item->stag,4);
		m_stock.black.count++;
	}

	#ifdef _DEBUG
	CheckStock();
	#endif
}
//---------------------------------------------------------------------------
void CMyStock::RemoveFromStock(BYTE mode,WORD ntag,BYTE type)
{
BYTE i;

	switch(mode)
	{
	case MODE_COLOR://*************************
		//Manage collected
		switch(type)
		{
		case CL_YELLOW:
			m_stock.map.yellow.collected--;
		break;
		case CL_PINK:
			m_stock.map.pink.collected--;
		break;
		case CL_BROWN:
			m_stock.map.brown.collected--;
		}
		
		for(i=0;i<m_stock.color.count;i++)
		{//Search to spec ntag
			if(m_stock.color.packs[i].ntag==ntag)
			{
				//Remove it
				m_stock.color.totalh -= m_stock.color.packs[i].h;
				//Remove From Catalog
				RemoveFromCatalog(ntag);

				//Close any gap
				BYTE size = sizeof(MY_ST_ITEM);
				for(BYTE j=i;j<m_stock.color.count;j++)
					memcpy(&m_stock.color.packs[j],&m_stock.color.packs[j+1],size);

				m_stock.color.count--;
				//Invaliedate last item
				return;
			}
		}
		break;

	case MODE_BLACK://*************************
		//Search for the specified tag
		for(i=0;i<m_stock.black.count;i++)
		{
			if(m_stock.black.packs[i].ntag==ntag)
			{
				//Remove it
				m_stock.black.totalh -= m_stock.black.packs[i].h;
				//Close any gap
				for(BYTE j=i;j<m_stock.black.count;j++)
				{
					m_stock.black.packs[j].realbk = m_stock.black.packs[j+1].realbk;
					m_stock.black.packs[j].type   = m_stock.black.packs[j+1].type;
					m_stock.black.packs[j].h	  = m_stock.black.packs[j+1].h;
					m_stock.black.packs[j].ntag   = m_stock.black.packs[j+1].ntag;
					memcpy(m_stock.black.packs[j].stag,m_stock.black.packs[j+1].stag,4);
				}
				m_stock.black.count--;
				break;
			}
		}
	}

	#ifdef _DEBUG
	CheckStock();
	#endif
}
//------------------------------------------------------------------------------------------------
WORD CMyStock::InitStore(CRobot &robot)
{
//Packs collection
CSpaceShipInfo	sinf;
	CMarc::GetSpaceShipInfo(sinf);
	m_stock.map.yellow.count= sinf.GetYellowPackageCount();
	m_stock.map.pink.count	= sinf.GetPinkPackageCount();
	m_stock.map.brown.count	= sinf.GetBrownPackageCount();
	m_stock.map.yellow.collected = m_stock.map.pink.collected = m_stock.map.brown.collected = 0;

	m_stock.color.totalh = m_stock.black.totalh = m_stock.secure.totalh= 0;
	m_stock.color.count  = m_stock.black.count  = m_stock.secure.count = 0;

	m_next_tag	= 0;
	//Int catalog
	for(BYTE i=0;i<ST_MAX;i++)
	{
		m_cat.item[i].dirty = INVALIED;
		m_cat.item[i].type  = m_cat.item[i].h = 100;
	}
	m_cat.count = 0;

	//Fill exchange patterns
	m_five.count = 6;
	m_five.pat[0].icount = 5;//1 This number Pattern Map name
	m_five.pat[0].h[0]   = 1;
	m_five.pat[0].h[1]   = 1;
	m_five.pat[0].h[2]   = 1;
	m_five.pat[0].h[3]   = 1;
	m_five.pat[0].h[4]   = 1;
	m_five.pat[1].icount = 4;//2
	m_five.pat[1].h[0]   = 1;
	m_five.pat[1].h[1]   = 1;
	m_five.pat[1].h[2]   = 1;
	m_five.pat[1].h[3]   = 2;
	m_five.pat[2].icount = 3;//3
	m_five.pat[2].h[0]   = 1;
	m_five.pat[2].h[1]   = 1;
	m_five.pat[2].h[2]   = 3;
	m_five.pat[3].icount = 2;//4
	m_five.pat[3].h[0]   = 1;
	m_five.pat[3].h[1]   = 4;
	m_five.pat[4].icount = 2;//5
	m_five.pat[4].h[0]   = 2;
	m_five.pat[4].h[1]   = 3;
	m_five.pat[5].icount = 1;//6
	m_five.pat[5].h[0]   = 5;

	m_four.count = 5;
	m_four.pat[0].icount = 4;//7
	m_four.pat[0].h[0]   = 1;
	m_four.pat[0].h[1]   = 1;
	m_four.pat[0].h[2]   = 1;
	m_four.pat[0].h[3]   = 1;
	m_four.pat[1].icount = 3;//8
	m_four.pat[1].h[0]   = 1;
	m_four.pat[1].h[1]   = 1;
	m_four.pat[1].h[2]   = 2;
	m_four.pat[2].icount = 2;//9
	m_four.pat[2].h[0]   = 1;
	m_four.pat[2].h[1]   = 3;
	m_four.pat[3].icount = 2;//10
	m_four.pat[3].h[0]   = 2;
	m_four.pat[3].h[1]   = 2;
	m_four.pat[4].icount = 1;//11
	m_four.pat[4].h[0]   = 4;

	m_three.count = 3;
	m_three.pat[0].icount = 3;//12
	m_three.pat[0].h[0]   = 1;
	m_three.pat[0].h[1]   = 1;
	m_three.pat[0].h[2]   = 1;
	m_three.pat[1].icount = 2;//13
	m_three.pat[1].h[0]   = 1;
	m_three.pat[1].h[1]   = 2;
	m_three.pat[2].icount = 1;//14
	m_three.pat[2].h[0]   = 3;

	m_two.count = 2;
	m_two.pat[0].icount = 2;//15
	m_two.pat[0].h[0]   = 1;
	m_two.pat[0].h[1]   = 1;
	m_two.pat[1].icount = 1;//16
	m_two.pat[1].h[0]   = 2;

	m_one.count = 1;
	m_one.pat[0].icount = 1;//17
	m_one.pat[0].h[0]   = 1;

	return m_stock.map.yellow.count+m_stock.map.pink.count+m_stock.map.brown.count;
}
//--------------------------------------------------------------------------------------------
bool CMyStock::IsBlackOver()
{
	if(m_stock.black.totalh==0)
		return true;
	else
		return false;
}
//--------------------------------------------------------------------------------------------
bool CMyStock::IsColorOver()
{
BYTE h = m_stock.color.totalh;

	if(h==0)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------------------------
bool CMyStock::IsAllCollected()
{
WORD p = m_stock.map.yellow.count+m_stock.map.pink.count+m_stock.map.brown.count;
WORD c = m_stock.map.brown.collected+m_stock.map.pink.collected+m_stock.map.yellow.collected;

	if(p==(m_stock.secure.count+c))
	{
		#ifdef _DEBUG
		MessageBox(NULL,"All collected !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
		#endif
		return true;
	}else{
		return false;
	}
}
//-----------------------------------------------------------------------------------------------
WORD CMyStock::GetScore()
{
BYTE i;
WORD p = 0;

	for(i=0;i<m_stock.color.count;i++)
		p += m_stock.color.packs[i].h * m_stock.color.packs[i].type;

	for(i=0;i<m_stock.secure.count;i++)
		p += m_stock.secure.packs[i].h * m_stock.secure.packs[i].type;

	#ifdef _DEBUG
	if(p==2400)
		MessageBox(NULL,"Max score !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
	#endif

	return p;
}
//------------------------------------------------------------------------------------------------
void CMyStock::ReleaseMaxHeight(MY_ITEM* ditem,BYTE mode)
{
BYTE		maxh,highest,i;
MY_PACKS	*st;
	switch (mode) 
	{ 
	case ST_COLOR:
		st = &m_stock.color;
		break;
	case ST_BLACK:
		st = &m_stock.black;
	}

	highest = maxh = 0;

	if(st->count!=0)
	{//Has some colors
		for(i=0;i<st->count;i++)
		{//Finding the highest item  
			if(st->packs[i].h>maxh)
			{
				maxh	= st->packs[i].h;
				highest = i;
				if(maxh==5)
					break;
			}
		}

		//Return pakage details
		ditem->realbk = st->packs[highest].realbk;
		ditem->type	  = st->packs[highest].type;
		ditem->h	  = st->packs[highest].h;
		ditem->ntag	  = st->packs[highest].ntag;
		memcpy(ditem->stag,st->packs[highest].stag,4);
	}
}
//------------------------------------------------------------------------------------------------
bool CMyStock::ReleaseExchange(MY_CAT_ITEM *item,BYTE *count,bool equal)
{
MY_EP	*ep;
BYTE	p_i,ph_i,i;
BYTE	p_count,ph_count,di[7];
bool	b;

	//Select exchange pattern
	switch (item[0].h) 
	{ 
	case 1:
		ep = &m_one;
	break;
	case 2:
		ep = &m_two;
	break;
	case 3:
		ep = &m_three;
	break;
	case 4:
		ep = &m_four;
	break;
	case 5:
		ep = &m_five;
	break;
	default://If somethig gone teribly wrong(Like item[0].h==6)
		#ifdef _DEBUG
		MessageBox(NULL,"Invaliied Height !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
		#endif
		return false;
	}

	ZeroMemory(di,7);
	p_count = ep->count;
	ph_count = 0;

	for(p_i=0;p_i<p_count;p_i++)
	{//Go through patterns ***
		
		//Reset dirty flag(This is realy tricky - a few palces must tap
		for(i=0;i<7;i++)
			m_cat.item[di[i]].dirty = INVALIED;
		ZeroMemory(di,6);

		#ifdef _DEBUG
		for(i=0;i<m_cat.count;i++)
		{
			if(m_cat.item[i].dirty==VALIED)
				MessageBox(NULL,"Dirty cat !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
		}
		#endif
		ph_count = ep->pat[p_i].icount;

		for(ph_i=0;ph_i<ph_count;ph_i++)
		{//Go through heights ####################
		b = false;
			//Less than check ********************
			for(i=0;i<m_cat.count;i++)
			{
				if(m_cat.item[i].type<item[0].type)//First item has the map details
				{//Less than
					if(m_cat.item[i].dirty==INVALIED)
					{//Fresh one
						if(m_cat.item[i].h==ep->pat[p_i].h[ph_i])
						{//Found match
							//Get pack details
							item[ph_i+1].store = m_cat.item[i].store;
							item[ph_i+1].type  = m_cat.item[i].type;
							item[ph_i+1].ntag  = m_cat.item[i].ntag;
							item[ph_i+1].h	   = m_cat.item[i].h;
							//Dirty this pack
							m_cat.item[i].dirty = VALIED;
							di[ph_i] = i;
							b = true;
							break;
						}
					}
				}
			}//Less than check *******************

			if(equal)
			{
				//Equal than check  ******************
				if(b==false)
				{//Less than loop not success
					for(i=0;i<m_cat.count;i++)
					{
						if(m_cat.item[i].type==item[0].type)//First item has the map details
						{//Equal
							if(m_cat.item[i].dirty==INVALIED)
							{//Fresh one
								if(m_cat.item[i].h==ep->pat[p_i].h[ph_i])
								{//Found match
									//Get pack details
									item[ph_i+1].store = m_cat.item[i].store;
									item[ph_i+1].type  = m_cat.item[i].type;
									item[ph_i+1].ntag  = m_cat.item[i].ntag;
									item[ph_i+1].h	   = m_cat.item[i].h;
									//Dirty this pack
									m_cat.item[i].dirty = VALIED;
									di[ph_i] = i;
									b = true;
									break;
								}
							}
						}
					}
				}//Equal than check  ******************
			}
			if(b==false)
				break;//One height not found(This pat fail)
		}//Go through heights ####################

		if(b)
		{//Patter succeed
			*count = ph_count+1;
			//Reset dirty flag(This is realy tricky - a few palces must tap
			for(i=0;i<7;i++)
				m_cat.item[di[i]].dirty = INVALIED;
			return true;
		}
	}//Go through patterns ***

	return false;
}
//------------------------------------------------------------------------------------------------
/*bool CMyStock::ReleaseExchange(MY_CAT_ITEM *item,BYTE *count)
{
MY_EP	*ep;
BYTE	p_i,ph_i,index,fild[5],f;
BYTE	p_count,ph_count,n;
bool	b;

	//Select exchange pattern
	switch (item[0].h) 
	{ 
	case 1:
		ep = &m_one;
	break;
	case 2:
		ep = &m_two;
	break;
	case 3:
		ep = &m_three;
	break;
	case 4:
		ep = &m_four;
	break;
	case 5:
		ep = &m_five;
	break;
	default://If somethig gone teribly wrong(Like item[0].h==6)
		#ifdef _DEBUG
		MessageBox(NULL,"Invaliied Height !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
		#endif
		return false;
	}

	p_count = ep->count;

	for(p_i=0;p_i<p_count;p_i++)
	{//GO THROUGH PATTERNS *********
	index = 1;
	f	  = 0;
	for(n=0;n<5;n++)//Initialize
		fild[n] = 100;
		for(BYTE i=0;i<m_cat.count;i++)
		{//GO THROUGH PACKAGES *********
			if(m_cat.item[i].type<item[0].type)//First item has the map details
			{//Exchangde potencials

				ph_count = ep->pat[p_i].icount;

				for(ph_i=0;ph_i<ph_count;ph_i++)
				{
					if(m_cat.item[i].h==ep->pat[p_i].h[ph_i])
					{
						b = true;
						for(n=0;n<f;n++)
						{
							if(fild[n]==ph_i)
							{//Fulfield pattern item
								b = false;
								break;
							}
						}
						if(b)
						{
							//Get fulfield pattern item
							fild[f] = ph_i;
							f++;
							//Get pack details
							item[index].store = m_cat.item[i].store;
							item[index].type  = m_cat.item[i].type;
							item[index].ntag  = m_cat.item[i].ntag;
							item[index].h	  = m_cat.item[i].h;
							index++;
							//All items found in this pattern
							if(index>ph_count)
							{
								*count = index;
								return true;
							}
							break;
						}
					}
				}	
			}
		}//GO THROUGH PACKAGES *********
	}//GO THROUGH PATTERNS *********

	return false;
}*/
//----------------------------------------------------------------------------------------------------
bool CMyStock::ReleaseTail(MY_CAT_ITEM *item,BYTE *count)
{//Release specific height from the lowest value pack(Top to bottom from m_cat)
BYTE index,rh,loss;

	index = 1;
	rh = loss = 0;

	for(BYTE i=0;i<m_cat.count;i++)
	{
		//Get pack details
		item[index].store = m_cat.item[i].store;
		item[index].type  = m_cat.item[i].type;
		item[index].ntag  = m_cat.item[i].ntag;
		item[index].h	  = m_cat.item[i].h;
		loss += m_cat.item[i].h*m_cat.item[i].type;
		rh   += m_cat.item[i].h;
		index++;
		//Check loss
		if(loss>=item[0].h*item[0].type)
			return false;
		//Check released height
		if(item[0].h<=rh)
		{
			*count = index;
			return true;
			break;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------------
BYTE CMyStock::GetStock(BYTE mode)
{
	switch(mode)
	{
	case ST_COL_BK:
		return m_stock.color.totalh + m_stock.black.totalh;
		break;
	case ST_SECURE:
		return m_stock.secure.totalh;
		break;
	case ST_COL_SEC:
		return m_stock.color.totalh + m_stock.secure.totalh;
	}

	return 0;		
}
//--------------------------------------------------------------------------------------------
BYTE CMyStock::GetColorHeight()
{
	return m_stock.color.totalh;
}
//--------------------------------------------------------------------------------------------
bool CMyStock::IsSecured(MY_POINT p,MY_ITEM *item,bool chkloc,MY_POINT *loc)
{
	for(BYTE i=0;i<m_stock.secure.count;i++)
	{//Search secured list	
		if((m_stock.secure.packs[i].p.x==p.x)&&(m_stock.secure.packs[i].p.y==p.y))
		{//That's a secured one 
			if(chkloc)
			{//Picking loc shuold check
				if((m_stock.secure.packs[i].loc.x==loc->x)&&(m_stock.secure.packs[i].loc.y==loc->y))
				{//Picking location ok (Location also check in PickSecure() by using m_sec_pic_loc
					if(item!=0)//When call by CMyBatAgent::PickSecured() item==0
					{
						item->ntag = m_stock.secure.packs[i].ntag;
						memcpy(item->stag,m_stock.secure.packs[i].stag,4);
					}
					return true;
				}
			}else{
			//No location checking(Call by GoForward())
				if(item!=0)//When call by SmartPick() item==0
				{
					item->ntag = m_stock.secure.packs[i].ntag;
					memcpy(item->stag,m_stock.secure.packs[i].stag,4);
				}
				return true;
			}
		} 
	}
	return false;
}
//--------------------------------------------------------------------------------------------
void CMyStock::AddToSecureList(MY_POINT p,MY_ITEM *item)
{//Maintain in type sorted manner
BYTE in;

	if(m_stock.secure.count==0)
	{//First item
		in = 0;
	}else{
		//Find the inserting point
		for(in=0;in<m_stock.secure.count;in++)
		{
			if(m_stock.secure.packs[in].type>=item->type)
				break;
		}

		//Make room for the new item (Move one index further)
		BYTE size = sizeof(MY_SC_POINT);
		for(BYTE i=m_stock.secure.count;i>in;i--)
			memcpy(&m_stock.secure.packs[i],&m_stock.secure.packs[i-1],size);
	}
	//Insert new item
	m_stock.secure.packs[in].p.x   = p.x;
	m_stock.secure.packs[in].p.y   = p.y;
	m_stock.secure.packs[in].loc.x = m_curent.x;
	m_stock.secure.packs[in].loc.y = m_curent.y;
	m_stock.secure.packs[in].h	   = item->h;
	m_stock.secure.packs[in].type  = item->type;
	m_stock.secure.packs[in].ntag  = item->ntag;
	memcpy(m_stock.secure.packs[in].stag,item->stag,4);
	m_stock.secure.totalh   += item->h;
	m_stock.secure.count++;
	//Add to Catalog
	AddToCatalog(STORE_SEC,item);

	#ifdef _DEBUG
	CheckStock();
	#endif
}
//-------------------------------------------------------------------------------------------------------
void CMyStock::RemoveFromSecure(WORD ntag,bool remove)
{//Only call by  CMyExchange::ExchangePacks()
	for(BYTE i=0;i<m_stock.secure.count;i++)
	{
		if(m_stock.secure.packs[i].ntag==ntag)
		{
			//Make this pack black in the map (That's elacution practice)
			m_map[m_stock.secure.packs[i].p.x][m_stock.secure.packs[i].p.y].type = CL_BLACK;
			m_map[m_stock.secure.packs[i].p.x][m_stock.secure.packs[i].p.y].realbk = INVALIED;//To prevent helth decay

			if(remove)
			{//Actualy remove it from stores
				//Remove it
				m_stock.secure.totalh -= m_stock.secure.packs[i].h;
				//Remove From Catalog
				RemoveFromCatalog(ntag);

				//Close any gap
				BYTE size = sizeof(MY_SC_POINT);
				for(BYTE j=i;j<m_stock.secure.count;j++)
					memcpy(&m_stock.secure.packs[j],&m_stock.secure.packs[j+1],size);

				m_stock.secure.count--;
			}
			break;
		}
	}
}
//------------------------------------------------------------------------------------------------
void CMyStock::RestoreSecurePacks(WORD ntag)
{
	for(BYTE i=0;i<m_stock.secure.count;i++)
	{
		if(m_stock.secure.packs[i].ntag==ntag)
		{//Re-store type
			m_map[m_stock.secure.packs[i].p.x][m_stock.secure.packs[i].p.y].type = m_stock.secure.packs[i].type;
			break;
		}
	}
}
//------------------------------------------------------------------------------------------------
void CMyStock::AddToCatalog(BYTE store,MY_ITEM *item)
{
BYTE in;

	if(m_cat.count==0)
	{//First item
		in = 0;
	}else{
		
		for(in=0;in<m_cat.count;in++)
		{
			if(((m_cat.item[in].type==item->type)&&(m_cat.item[in].h>=item->h))||(m_cat.item[in].type>item->type))
			//if(((m_cat.item[in].type>=item->type)&&(m_cat.item[in].h>=item->h)))
				break;
		}

		//Make room for the new item (Move one index further)
		BYTE size = sizeof(MY_CAT_ITEM);
		for(BYTE i=m_cat.count;i>in;i--)
			memcpy(&m_cat.item[i],&m_cat.item[i-1],size);
	}

	//Insert new item
	m_cat.item[in].type  = item->type;
	m_cat.item[in].ntag  = item->ntag;
	m_cat.item[in].h	 = item->h;
	m_cat.item[in].store = store;
	m_cat.count++;

	/*ifdef _DEBUG
BYTE h = 100;
		for(int t=0;t<ST_MAX;t++)
		{
			if(m_cat.item[in].type<h)
				MessageBox(NULL,"Catalog mix!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
			if(h<m_cat.item[in].type)
				h = m_cat.item[in].type;
		}

WORD s[200];
	for(t=0;t<ST_MAX;t++)
		s[t] = m_cat.item[t].type;
	#endif*/
	
}
//------------------------------------------------------------------------------------------------
void CMyStock::RemoveFromCatalog(WORD ntag)
{
	for(BYTE i=0;i<m_cat.count;i++)
	{
		if(m_cat.item[i].ntag==ntag)
		{
			//Close gap
			BYTE size = sizeof(MY_CAT_ITEM);
			for(BYTE j=i;j<m_cat.count;j++)
				memcpy(&m_cat.item[j],&m_cat.item[j+1],size);
		
			m_cat.count--;
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------
void CMyStock::GetBelStringTag(char *stag, WORD ntag,BYTE type)
{
	for(BYTE i=0;i<m_stock.color.count;i++)
	{//Search to spec ntag
		if(m_stock.color.packs[i].ntag==ntag)
		{
			memcpy(stag,m_stock.color.packs[i].stag,4);
			return;
		}
	}
}
//------------------------------------------------------------------------------------------------
void CMyStock::GetSecStringTag(char *stag, WORD ntag)
{
	for(BYTE i=0;i<m_stock.secure.count;i++)
	{
		if(m_stock.secure.packs[i].ntag==ntag)
		{
			memcpy(stag,m_stock.secure.packs[i].stag,4);
			return;
		}
	}
}
//------------------------------------------------------------------------------------------------
void CMyStock::MakeTag(char* stag,WORD* ntag)
{//Capacity <9999
int		pos[4],stop;
DWORD	n;

	m_next_tag++;
	*ntag = m_next_tag;
	n     = m_next_tag;

	#ifdef _DEBUG
	if(m_next_tag==100)
		MessageBox(NULL,"100 Tags !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
	#endif

	ZeroMemory(stag,4);

	//Set loop*********************************************
	if(n>999)
		{//1000
			stop = 3;
		}else{

			if(n>99)
			{//100
				stop = 2;
			}else{
				if(n>9)
				{//10
					stop = 1;
				}else{//1
					stop = 0;
				}
			}
		}

	//split intiger******************************************
	pos[3] =  n/1000;
	n = n - pos[3]*1000;
	pos[2] =  n/100;
	n = n - pos[2]*100;
	pos[1] =  n/10;
	n = n - pos[1]*10;
	pos[0] =  n;
	
	//convert to char****************************************
	for(int i=0;i<=stop;i++)
	{
		switch (pos[stop-i])
		{
		case 0:
			stag[i] = '0';break;
		case 1:
			stag[i] = '1';break;
		case 2:
			stag[i] = '2';break;
		case 3:
			stag[i] = '3';break;
		case 4:
			stag[i] = '4';break;
		case 5:
			stag[i] = '5';break;
		case 6:
			stag[i] = '6';break;
		case 7:
			stag[i] = '7';break;
		case 8:
			stag[i] = '8';break;
		case 9:
			stag[i] = '9';break;
		}
	}
}
//------------------------------------------------------------------------------------------------
#ifdef _DEBUG
void CMyStock::CheckStock()
{
BYTE level;

	level = GetStock(ST_COL_SEC);

	//Overflow
	if(level>120)
		MessageBox(NULL,"Stock overflow !  ","Marc",MB_OK|MB_ICONEXCLAMATION);
}
#endif


