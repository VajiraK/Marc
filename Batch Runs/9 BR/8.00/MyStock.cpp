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
			//memcpy(&m_stock.color.packs[in+1],&temp,m_my_st_item*(m_stock.color.count));
			for(BYTE i=m_stock.color.count;i>in;i--)
			{
				//m_stock.color.packs[i].exist  = m_stock.color.packs[i-1].exist;
				m_stock.color.packs[i].realbk = m_stock.color.packs[i-1].realbk;
				m_stock.color.packs[i].type   = m_stock.color.packs[i-1].type;
				m_stock.color.packs[i].h	  = m_stock.color.packs[i-1].h;
				m_stock.color.packs[i].ntag   = m_stock.color.packs[i-1].ntag;
				memcpy(m_stock.color.packs[i].stag,m_stock.color.packs[i-1].stag,4);
			}
		}

		//Insert new item
		//m_stock.color.packs[in].exist  = VALIED;
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
		//m_stock.black.packs[in].exist  = VALIED;
		m_stock.black.packs[in].realbk = item->realbk;
		m_stock.black.packs[in].type   = item->type;
		m_stock.black.packs[in].h	   = item->h;
		m_stock.black.packs[in].ntag   = item->ntag;
		memcpy(m_stock.black.packs[in].stag,item->stag,4);
		m_stock.black.count++;
	}
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
				//m_stock.color.packs[i].exist = INVALIED;
				m_stock.color.totalh -= m_stock.color.packs[i].h;
				//Remove From Catalog
				RemoveFromCatalog(ntag);
				//Close any gap
				//memcpy(&m_stock.color.packs[i],&m_stock.color.packs[i+1],m_my_st_item*(m_stock.color.count-i));

				for(BYTE j=i;j<m_stock.color.count;j++)
				{
					//m_stock.color.packs[j].exist  = m_stock.color.packs[j+1].exist;
					m_stock.color.packs[j].realbk = m_stock.color.packs[j+1].realbk;
					m_stock.color.packs[j].type   = m_stock.color.packs[j+1].type;
					m_stock.color.packs[j].h	  = m_stock.color.packs[j+1].h;
					m_stock.color.packs[j].ntag   = m_stock.color.packs[j+1].ntag;
					memcpy(m_stock.color.packs[j].stag,m_stock.color.packs[j+1].stag,4);
				}

				m_stock.color.count--;
				//Invaliedate last item
				//m_stock.color.packs[m_stock.color.count].exist = INVALIED;
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
				//m_stock.black.packs[i].exist  = INVALIED;
				//Close any gap
				//memcpy(&m_stock.color.packs[i],&m_stock.color.packs[i+1],m_my_st_item*(m_stock.color.count-i));
				for(BYTE j=i;j<m_stock.black.count;j++)
				{
					//m_stock.black.packs[j].exist  = m_stock.black.packs[j+1].exist;
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
}
//------------------------------------------------------------------------------------------------
WORD CMyStock::InitStore(CRobot &robot)
{
	m_my_st_item  = sizeof(MY_ST_ITEM);
	m_my_sc_point = sizeof(MY_SC_POINT);
	m_my_cat_item = sizeof(MY_CAT_ITEM);
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
	m_cat.count = 0;

	//Fill exchange patterns
	m_five.count = 6;
	m_five.pat[0].icount = 5;//
	m_five.pat[0].h[0]   = 1;
	m_five.pat[0].h[1]   = 1;
	m_five.pat[0].h[2]   = 1;
	m_five.pat[0].h[3]   = 1;
	m_five.pat[0].h[4]   = 1;
	m_five.pat[1].icount = 4;//
	m_five.pat[1].h[0]   = 1;
	m_five.pat[1].h[1]   = 1;
	m_five.pat[1].h[2]   = 1;
	m_five.pat[1].h[3]   = 2;
	m_five.pat[2].icount = 3;//
	m_five.pat[2].h[0]   = 1;
	m_five.pat[2].h[1]   = 1;
	m_five.pat[2].h[2]   = 3;
	m_five.pat[3].icount = 2;//
	m_five.pat[3].h[0]   = 1;
	m_five.pat[3].h[1]   = 4;
	m_five.pat[4].icount = 2;//
	m_five.pat[4].h[0]   = 2;
	m_five.pat[4].h[1]   = 3;
	m_five.pat[5].icount = 1;//
	m_five.pat[5].h[0]   = 5;

	m_four.count = 5;
	m_four.pat[0].icount = 4;//
	m_four.pat[0].h[0]   = 1;
	m_four.pat[0].h[1]   = 1;
	m_four.pat[0].h[2]   = 1;
	m_four.pat[0].h[3]   = 1;
	m_four.pat[1].icount = 3;//
	m_four.pat[1].h[0]   = 1;
	m_four.pat[1].h[1]   = 1;
	m_four.pat[1].h[2]   = 2;
	m_four.pat[2].icount = 2;//
	m_four.pat[2].h[0]   = 1;
	m_four.pat[2].h[1]   = 3;
	m_four.pat[3].icount = 2;//
	m_four.pat[3].h[0]   = 2;
	m_four.pat[3].h[1]   = 2;
	m_four.pat[4].icount = 1;//
	m_four.pat[4].h[0]   = 4;

	m_three.count = 3;
	m_three.pat[0].icount = 3;//
	m_three.pat[0].h[0]   = 1;
	m_three.pat[0].h[1]   = 1;
	m_three.pat[0].h[2]   = 1;
	m_three.pat[1].icount = 2;//
	m_three.pat[1].h[0]   = 1;
	m_three.pat[1].h[1]   = 2;
	m_three.pat[2].icount = 1;//
	m_three.pat[2].h[0]   = 3;

	m_two.count = 2;
	m_two.pat[0].icount = 2;//
	m_two.pat[0].h[0]   = 1;
	m_two.pat[0].h[1]   = 1;
	m_two.pat[1].icount = 1;//
	m_two.pat[1].h[0]   = 2;

	m_one.count = 1;
	m_one.pat[0].icount = 1;//
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
#ifdef _DEBUG
WORD CMyStock::GetScore()
{
BYTE i;
WORD p = 0;

	for(i=0;i<m_stock.color.count;i++)
	{
		p += m_stock.color.packs[i].h * m_stock.color.packs[i].type;
	}

	return p;
}
#endif
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
				//MessageBox(NULL,m_stock.color.packs[i].stag,"Marc",MB_OK|MB_ICONEXCLAMATION);
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
bool CMyStock::ReleaseExchange(MY_CAT_ITEM *item,BYTE *count)
{
MY_EP	*ep;
BYTE	pat_i,item_i;

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
	}

	for(pat_i=0;pat_i<ep->count;pat_i++)
	{//GO THROUGH PATTERNS *********
	item_i = 1;
		for(BYTE i=0;i<m_cat.count;i++)
		{//GO THROUGH PACKAGES *********
			if(m_cat.item[i].type<item[0].type)//First item has the map details
			{//Exchangde potencials
				if(m_cat.item[i].h==ep->pat[pat_i].h[item_i-1])
				{
					item[item_i].store = m_cat.item[i].store;
					item[item_i].type  = m_cat.item[i].type;
					item[item_i].ntag  = m_cat.item[i].ntag;
					item[item_i].h	   = m_cat.item[i].h;
					item_i++;
					if(item_i>ep->pat[pat_i].icount)
					{//All items found in this pattern
						*count = item_i;
						return true;
					}
				}
			}
		}//GO THROUGH PACKAGES *********
	}//GO THROUGH PATTERNS *********

	return false;
}
//------------------------------------------------------------------------------------------------
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
bool CMyStock::IsSecured(MY_POINT p,MY_ITEM *item,bool update)
{
	for(BYTE i=0;i<m_stock.secure.count;i++)
	{//Search secured list	
		if((m_stock.secure.packs[i].p.x==p.x)&&(m_stock.secure.packs[i].p.y==p.y))
		{//That's a secured one 
			if(update)
			{//Remove this item from list
				item->ntag = m_stock.secure.packs[i].ntag;
				memcpy(item->stag,m_stock.secure.packs[i].stag,4);
				m_stock.secure.totalh -= item->h;
				//Remove From Catalog
				RemoveFromCatalog(item->ntag);
				//Close any gap
				//memcpy(&m_stock.secure.packs[i],&m_stock.secure.packs[i+1],m_my_sc_point*(m_stock.secure.count-i));

				for(BYTE j=i;j<m_stock.secure.count;j++)
				{
					m_stock.secure.packs[j].p.x   = m_stock.secure.packs[j+1].p.x;
					m_stock.secure.packs[j].p.y   = m_stock.secure.packs[j+1].p.y;
					m_stock.secure.packs[j].h	   = m_stock.secure.packs[j+1].h;
					m_stock.secure.packs[j].type  = m_stock.secure.packs[j+1].type;
					m_stock.secure.packs[j].ntag  = m_stock.secure.packs[j+1].ntag;
					memcpy(m_stock.secure.packs[j].stag,m_stock.secure.packs[j+1].stag,4);
				}
				m_stock.secure.count--;
				//Invaliedate last item
				//m_stock.secure.packs[m_stock.secure.count].exist = INVALIED;
			}
			return true;
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
		//memcpy(&m_stock.secure.packs[in+1],&m_stock.secure.packs[in],m_my_sc_point*m_stock.secure.count);
		for(BYTE i=m_stock.secure.count;i>in;i--)
		{
			m_stock.secure.packs[i].p.x   = m_stock.secure.packs[i-1].p.x;
			m_stock.secure.packs[i].p.y   = m_stock.secure.packs[i-1].p.y;
			m_stock.secure.packs[i].h	   = m_stock.secure.packs[i-1].h;
			m_stock.secure.packs[i].type  = m_stock.secure.packs[i-1].type;
			m_stock.secure.packs[i].ntag  = m_stock.secure.packs[i-1].ntag;
			memcpy(m_stock.secure.packs[i].stag,m_stock.secure.packs[i-1].stag,4);
		}
	}
	//Insert new item
	//m_stock.secure.packs[in].exist = VALIED; 
	m_stock.secure.packs[in].p.x   = p.x;
	m_stock.secure.packs[in].p.y   = p.y;
	m_stock.secure.packs[in].h	   = item->h;
	m_stock.secure.packs[in].type  = item->type;
	m_stock.secure.packs[in].ntag  = item->ntag;
	memcpy(m_stock.secure.packs[in].stag,item->stag,4);
	m_stock.secure.totalh   += item->h;
	m_stock.secure.count++;
	//Add to Catalog
	AddToCatalog(STORE_SEC,item);
}
//-------------------------------------------------------------------------------------------------------
void CMyStock::RemoveFromSecure(WORD ntag)
{//Only call by  CMyExchange::ExchangePacks()
	for(BYTE i=0;i<m_stock.secure.count;i++)
	{
		if(m_stock.secure.packs[i].ntag==ntag)
		{
			//Make this pack black in the map (That's elacution practice)
			m_map[m_stock.secure.packs[i].p.x][m_stock.secure.packs[i].p.y].type = CL_BLACK;
			m_map[m_stock.secure.packs[i].p.x][m_stock.secure.packs[i].p.y].realbk = INVALIED;//To prevent helth decay
			//Remove it
			//m_stock.secure.packs[i].exist = INVALIED;
			m_stock.secure.totalh -= m_stock.secure.packs[i].h;
			//Remove From Catalog
			RemoveFromCatalog(ntag);
			//Close any gap
			//memcpy(&m_stock.secure.packs[i],&m_stock.secure.packs[i+1],m_my_sc_point*(m_stock.secure.count-i));

			for(BYTE j=i;j<m_stock.secure.count;j++)
			{
				m_stock.secure.packs[j].p.x   = m_stock.secure.packs[j+1].p.x;
				m_stock.secure.packs[j].p.y   = m_stock.secure.packs[j+1].p.y;
				m_stock.secure.packs[j].h	   = m_stock.secure.packs[j+1].h;
				m_stock.secure.packs[j].type  = m_stock.secure.packs[j+1].type;
				m_stock.secure.packs[j].ntag  = m_stock.secure.packs[j+1].ntag;
				memcpy(m_stock.secure.packs[j].stag,m_stock.secure.packs[j+1].stag,4);
			}

			m_stock.secure.count--;
			//Invaliedate last item
			//m_stock.secure.packs[m_stock.secure.count].exist = INVALIED;
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
		//Find the inserting point
		for(in=0;in<m_cat.count;in++)
		{
			if(m_cat.item[in].type>=item->type)
				break;
		}
		//Make room for the new item (Move one index further)
		//memcpy(&m_cat.item[in+1],&m_cat.item[in],m_my_cat_item*m_cat.count);
		for(BYTE i=m_cat.count;i>in;i--)
		{
			m_cat.item[i].type  = m_cat.item[i-1].type;
			m_cat.item[i].ntag  = m_cat.item[i-1].ntag;
			m_cat.item[i].h		= m_cat.item[i-1].h;
			m_cat.item[i].store = m_cat.item[i-1].store;
		}
	}
	//Insert new item
	m_cat.item[in].type  = item->type;
	m_cat.item[in].ntag  = item->ntag;
	m_cat.item[in].h	 = item->h;
	m_cat.item[in].store = store;
	m_cat.count++;
}
//------------------------------------------------------------------------------------------------
void CMyStock::RemoveFromCatalog(WORD ntag)
{
	for(BYTE i=0;i<m_cat.count;i++)
	{
		if(m_cat.item[i].ntag==ntag)
		{
			//Close gap
			//memcpy(&m_cat.item[i],&m_cat.item[i+1],m_my_cat_item*(m_cat.count-i));
			for(BYTE j=i;j<m_cat.count;j++)
			{
				m_cat.item[j].type  = m_cat.item[j+1].type;
				m_cat.item[j].ntag  = m_cat.item[j+1].ntag;
				m_cat.item[j].h		= m_cat.item[j+1].h;
				m_cat.item[j].store = m_cat.item[j+1].store;
			}
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
/*void CMyStock::ReleaseLowestPoit(MY_ITEM *ditem)
{
BYTE	p,least,i,h;
BYTE	valied[ST_MAX],lowest[ST_MAX];
BYTE	c1,c2;

	c1 = c2 = 0;

	for(i=0;i<ST_MAX;i++)
	{
		if(m_stock_cl.item[i].exist==VALIED)
		{//Collect idexes that has valied item
			valied[c1] = i;
			c1++;
		}
	}
	//Finding the lowest point value
	p = m_stock_cl.item[valied[0]].points;
	for(i=1;i<c1;i++)
	{
		if(m_stock_cl.item[valied[i]].points<p)
		{
			p = m_stock_cl.item[valied[i]].points;
		}
	}
	//Extract lowest poit item(s)
	for(i=0;i<c1;i++)
	{
		if(m_stock_cl.item[valied[i]].points==p)
		{
			lowest[c2] = valied[i];
			c2++;
		}
	}
	//Select the heighest item from lowest poits
	h = m_stock_cl.item[lowest[0]].h;
	least = lowest[0];
	for(i=1;i<c2;i++)
	{
		if(m_stock_cl.item[lowest[i]].h>h)
		{
			h = m_stock_cl.item[lowest[i]].h;
			least = lowest[i];
		}
	}

	//Return pakage details
	ditem->realbk = m_stock_cl.item[least].realbk;
	ditem->type	= m_stock_cl.item[least].type;
	ditem->h	= m_stock_cl.item[least].h;
	ditem->ntag	= m_stock_cl.item[least].ntag;
	memcpy(ditem->stag,m_stock_cl.item[least].stag,4);
}*/


