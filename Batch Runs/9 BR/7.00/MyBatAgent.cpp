// MyBatAgent.cpp: implementation of the CMyBatAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBatAgent.h"

CMyBatAgent::CMyBatAgent(){}
CMyBatAgent::~CMyBatAgent(){}
//------------------------------------------------------------------------------------------------
void CMyBatAgent::Initialize(CMyStock *pstcls, MY_POINT p, BYTE d,BYTE stock)
{
	m_facing = d;
	m_current.x = p.x;
	m_current.y = p.y;

	m_stock = pstcls;
	m_packs = stock;

	m_bat = m_count = 0;
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
	m_nst++;
	nbat = 1 + ((10 + (m_packs * 2))  *  st);
	m_stbat += nbat;
	m_bat += nbat;
	//Return average bat needed to one st
	return (WORD)(m_stbat/m_nst);
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
MY_POINT *p_ord = m_stock->m_row_ord;
MY_POINT *p_point = &m_stock->m_point;
MY_ITEM	  item;
bool	  fresh = true;

	for(BYTE i=0;i<25;i++)
	{
	m_stock->PictureToMap(m_current,p_ord[i],m_facing);
	if(p_point->x!=MAP_INVALIED)
	{
		m_stock->GetMapPoint(p_point,&item);
		if((item.type==CL_YELLOW)||(item.type==CL_PINK)||(item.type==CL_BROWN))
		{
			if(m_stock->IsSecured(*p_point,0,0,false))
			{//Only pick secured one **************************
				for(BYTE c=0;c<m_count;c++)
				{
					if((m_picked[c].x==p_point->x)&&(m_picked[c].y==p_point->y))
					{
						fresh = false;
						break;
					}
				}
				if(fresh)
				{
					m_picked[m_count].x = p_point->x;
					m_picked[m_count].y = p_point->y;
					m_count++;
					m_packs += item.h;
					m_bat += 4;
				}
			}//Only pick secured one **************************
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
