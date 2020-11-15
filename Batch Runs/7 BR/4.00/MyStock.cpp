// MyStock.cpp: implementation of the CMyStock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyStock.h"

CMyStock::~CMyStock(){}
//-------------------------------------------------------------------------------------------------
CMyStock::CMyStock(){}
//-------------------------------------------------------------------------------------------------
void CMyStock::AddToStock(BYTE mode,BYTE h,BYTE type,WORD ntag,char* stag)
{
MY_LUGGAGE *pstk;
BYTE		i;

	if(mode==MODE_COLOR)
	{
		pstk = &m_stock_cl;
		m_rem_pack--;
	}else{
		pstk = &m_stock_bk;
	}

	//Add to total height
	pstk->totalh += h;

	//Select a free item
	for(i=0;i<=ST_MAX;i++)
	{
		if(pstk->item[i].exist==ST_NO)
			break;
	}

	pstk->item[i].exist	= ST_YES;
	pstk->item[i].type	= type;
	pstk->item[i].h		= h;
	pstk->item[i].ntag	= ntag;
	memcpy(pstk->item[i].stag,stag,4);
	
	//Point's all I need
	switch (type) 
	{ 
		case CL_YELLOW:
			pstk->item[i].points = h*20;
		break;
		case CL_PINK:
			pstk->item[i].points = h*15;
		break;
		case CL_BROWN:
			pstk->item[i].points = h*10;
		break;
		default:
			pstk->item[i].points = 0;
	}	
}
//---------------------------------------------------------------------------
void CMyStock::RemoveFromStock(BYTE mode,WORD ntag)
{
MY_LUGGAGE *pstk;
BYTE		i;

	if(mode==MODE_COLOR)
	{
		pstk = &m_stock_cl;
		m_rem_pack++;
	}else{
		pstk = &m_stock_bk; 
	}

	//Search for the specified tag
	for(i=0;i<=ST_MAX;i++)
	{
		if(pstk->item[i].ntag==ntag)
			break;
	}

	//Remove it
	pstk->item[i].exist	= ST_NO;
	pstk->totalh -= pstk->item[i].h;
}
//---------------------------------------------------------------------------
BYTE CMyStock::GetCollected()
{
	return m_stock_cl.totalh + m_stock_bk.totalh;
}
//------------------------------------------------------------------------------------------------
WORD CMyStock::InitStore(CRobot &robot)
{
	//Packs collection
CSpaceShipInfo	sinf;
	CMarc::GetSpaceShipInfo(sinf);
	m_rem_pack  = 0;
	m_rem_pack  = sinf.GetBrownPackageCount();
	m_rem_pack += sinf.GetYellowPackageCount();
	m_rem_pack += sinf.GetPinkPackageCount();
	//Prepaire arrays
	for(BYTE i=0;i<=ST_MAX;i++)
	{
		m_stock_bk.item[i].exist = ST_NO;
		m_stock_cl.item[i].exist = ST_NO;
	}

	m_next_tag = m_stock_bk.totalh = m_stock_cl.totalh = 0;

	return m_rem_pack;
}
//-----------------------------------------------------------------------------------------------
void CMyStock::StockRelease(MY_ITEM* ditem,BYTE mode)
{//Find the lowest poit pakage in color mode
	switch (mode) 
	{ 
	case ST_BLACK:
		ReleaseBlack(ditem);
		break;
	case ST_LOW_P:
		ReleaseLowestPoit(ditem);
		break;
	case ST_MAX_H:
		ReleaseMaxHighest(ditem);
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
//--------------------------------------------------------------------------------------------
bool CMyStock::IsBlackOver()
{
	if(m_stock_bk.totalh==0)
		return true;
	else
		return false;
}
//--------------------------------------------------------------------------------------------
bool CMyStock::IsColorOver()
{
	if(m_stock_cl.totalh==0)
		return true;
	else
		return false;
}
//--------------------------------------------------------------------------------------------
BYTE CMyStock::GetColorCount()
{
	return m_stock_cl.totalh;
}
//-----------------------------------------------------------------------------------------------
bool CMyStock::IsAllCollected()
{
	if(m_rem_pack==0)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------------------------
WORD CMyStock::GetScore()
{
WORD p = 0;
	for(BYTE i=0;i<=ST_MAX;i++)
	{
		if(m_stock_cl.item[i].exist==ST_YES)
			p += m_stock_cl.item[i].points;
	}

	return p;
}
//------------------------------------------------------------------------------------------------
void CMyStock::ReleaseMaxHighest(MY_ITEM* ditem)
{
BYTE	maxh,highest,i;
BYTE	valied[ST_MAX];
BYTE	count = 0;

	for(i=0;i<=ST_MAX;i++)
	{
		if(m_stock_cl.item[i].exist==ST_YES)
		{//Collect idexes that has valied item
			valied[count] = i;
			count++;
		}
	}

	//Finding the highest item      
	maxh = m_stock_cl.item[valied[0]].h;
	highest = valied[0];

	for(i=1;i<count;i++)
	{
		if(m_stock_cl.item[valied[i]].h>maxh)
		{
			maxh = m_stock_cl.item[valied[i]].h;
			highest = valied[i];
		}
	}

	//Return pakage details
	ditem->type	= m_stock_cl.item[highest].type;
	ditem->h	= m_stock_cl.item[highest].h;
	ditem->ntag	= m_stock_cl.item[highest].ntag;
	memcpy(&ditem->stag,&m_stock_cl.item[highest].stag,4);
}
//------------------------------------------------------------------------------------------------
void CMyStock::ReleaseLowestPoit(MY_ITEM *ditem)
{
BYTE	p,least,i,h;
BYTE	valied[ST_MAX],lowest[ST_MAX];
BYTE	c1,c2;

	c1 = c2 = 0;

	for(i=0;i<=ST_MAX;i++)
	{
		if(m_stock_cl.item[i].exist==ST_YES)
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
	ditem->type	= m_stock_cl.item[least].type;
	ditem->h	= m_stock_cl.item[least].h;
	ditem->ntag	= m_stock_cl.item[least].ntag;
	memcpy(&ditem->stag,&m_stock_cl.item[least].stag,4);
}
//------------------------------------------------------------------------------------------------
void CMyStock::ReleaseBlack(MY_ITEM *ditem)
{
	for(BYTE i=0;i<=ST_MAX;i++)
	{
		if(m_stock_bk.item[i].exist==ST_YES)
		{//Valied item
			//Return pakage details
			ditem->type	= m_stock_bk.item[i].type;
			ditem->h	= m_stock_bk.item[i].h;
			ditem->ntag	= m_stock_bk.item[i].ntag;
			memcpy(&ditem->stag,&m_stock_bk.item[i].stag,4);
			return;
		}
	}
}
