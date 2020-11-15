// MyBatAgent.cpp: implementation of the CMyBatAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBatAgent.h"

CMyBatAgent::CMyBatAgent(){}
//------------------------------------------------------------------------------------------------
CMyBatAgent::~CMyBatAgent()
{
	//Re_store count
	m_sl->count = m_sl_count;
}
//------------------------------------------------------------------------------------------------
void CMyBatAgent::Initialize(CMyStock *pstcls, MY_POINT p, BYTE d,BYTE stock,MY_SC_LOCATIONS *sl)
{
	m_facing = d;
	m_current.x = p.x;
	m_current.y = p.y;

	m_packh = stock;
	//CMyStock class
	m_stock = pstcls;
	//Secure location list
	m_sl = sl;
	//Save count because this gay(CMyBatAgent) change it
	m_sl_count = m_sl->count;

	m_bat = 0;
	m_bat_st = m_total_st = 0;

	m_helth = 3;
}
//------------------------------------------------------------------------------------------------
WORD CMyBatAgent::Move(BYTE st)
{
WORD   nbat;

	//Move ******************************
	switch (m_facing)
	{ 
	case FACE_UP:
		m_current.y = m_current.y-st;
		break;
	case FACE_RIGHT:
		m_current.x = m_current.x+st;
		break;
	case FACE_DOWN:
		m_current.y = m_current.y+st;
		break;
	case FACE_LEFT:
		m_current.x = m_current.x-st;
		break;	
	}
	//Batory ***************************
	m_total_st += st;
	nbat = 1 + ((10 + (m_packh * 2))  *  st);
	m_bat_st += nbat;
	m_bat += nbat;
	//Return average bat needed for one st
	return (WORD)(m_bat_st/m_total_st)*10;
}
//------------------------------------------------------------------------------------------------
void CMyBatAgent::SetFacing(BYTE d)
{
	switch (d) 
	{ 
	case FACE_UP:
		switch (m_facing) 
		{ 
		case FACE_DOWN:
			m_bat += 5;
		break;
		case FACE_LEFT:
			m_bat += 3;
		break;
		case FACE_RIGHT:
			m_bat += 3;
		break;
		}	
	m_facing = FACE_UP;
	break;

	case FACE_DOWN:
		switch (m_facing) 
		{ 
		case FACE_UP:
			m_bat += 5;
		break;
		case FACE_LEFT:
			m_bat += 3;
		break;
		case FACE_RIGHT:
			m_bat += 3;
		break;
		}
	m_facing = FACE_DOWN;
	break;

	case FACE_LEFT:
		switch (m_facing) 
		{ 
		case FACE_UP:
			m_bat += 3;
		break;
		case FACE_DOWN:
			m_bat += 3;
		break;
		case FACE_RIGHT:
			m_bat += 5;
		break;
		}
	m_facing = FACE_LEFT;
	break;

	case FACE_RIGHT:
		switch (m_facing) 
		{ 
		case FACE_UP:
			m_bat += 3;
		break;
		case FACE_DOWN:
			m_bat += 3;
		break;
		case FACE_LEFT:
			m_bat += 5;
		break;
		}
	m_facing = FACE_RIGHT;
	break;
	}
}
//------------------------------------------------------------------------------------------------
void CMyBatAgent::PickSecured()
{
MY_PIC_ORD*	p_ord = &m_stock->m_row_ord;
MY_POINT	map;
MY_ITEM		item;

	//Only pick from secured location (Prevent bat usage unevenness)
	//this is also check in IsSecured() for each pack
	if((m_sl->loc[m_sl->count-1].p.x==m_current.x)&&(m_sl->loc[m_sl->count-1].p.y==m_current.y)&&(m_sl->loc[m_sl->count-1].d==m_facing))
		m_sl->count--;
	else
		return;

	for(BYTE i=0;i<25;i++)
	{
		if(m_stock->PictureToMap(m_current,&p_ord->p[i],&map,m_facing))
		{
			m_stock->GetMapPoint(&map,&item);

			if((item.type==CL_YELLOW)||(item.type==CL_PINK)||(item.type==CL_BROWN))
			{
				if(m_stock->IsSecured(map,0,true,&m_current))
				{//Only pick secured one 
					m_packh += item.h;
					m_bat += 4;
				}else{
					/*if(item.free==VALIED)
						m_bat += 25;*/
				}
			}else{
				if(item.type==CL_BLACK)
					m_bat += 25;	
			}
			
		}
	}
}
//------------------------------------------------------------------------------------------------
void CMyBatAgent::TelePort(MY_POINT *p)
{
	m_current.x = p->x;
	m_current.y = p->y;
	m_bat += 1;
}
//------------------------------------------------------------------------------------------------
void CMyBatAgent::PassBlacks()
{
MY_PIC_ORD*	p_ord = &m_stock->m_row_ord;
MY_POINT	map;
MY_ITEM		item;

	for(BYTE i=0;i<25;i++)
	{
		if(m_stock->PictureToMap(m_current,&p_ord->p[i],&map,m_facing))
		{
			m_stock->GetMapPoint(&map,&item);
			if(item.type==CL_BLACK)
				m_helth++;
		}
	}
}
