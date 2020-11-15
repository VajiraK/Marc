// MyTug.cpp: implementation of the CMyTug class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTug.h"

CMyTug::CMyTug(){}
CMyTug::~CMyTug(){}
//-----------------------------------------------------------------------------------------------
WORD CMyTug::Move(WORD st)
{
	if(st!=0)
	{
		//Save location
		SaveLoc(st);
		//Move
		m_pmap->PlotLocation(&m_current,m_facing,st);
	}

return st;
}
//-----------------------------------------------------------------------------------------------
void CMyTug::SetFacing(BYTE d)
{
	m_facing = d;
}
//-----------------------------------------------------------------------------------------------
void CMyTug::Initialize(CMyMap *pmap,MY_POINT p,BYTE d)
{
	m_pmap	= pmap;
	m_pmap->CopyPoint(&m_current,&p);
	m_facing = d;
	m_index = 0;
}
//------------------------------------------------------------------
bool CMyTug::IsBeenThere()
{
	return m_pmap->IsBeenThere(m_current,m_facing);
}
//------------------------------------------------------------------
void CMyTug::SaveLoc(WORD st,BYTE tele)
{
		m_pmap->CopyPoint(&m_path[m_index].loc,&m_current);

		m_path[m_index].att	   = tele;
		m_path[m_index].came_d = m_facing;
		m_path[m_index].steps  = st;
		m_index++;
}
//------------------------------------------------------------------------------------------------
bool CMyTug::TryDirections(BYTE tele)
{
BYTE d;
WORD steps;

	d = m_facing;

	for(int i=0;i<=3;i++)
	{
		if(m_pmap->GetFresh(m_current,&d))
		{//Face set to underty area
			steps = m_pmap->GetFreeStepsEx(m_current,d,true);

			if(steps!=0)
			{
				SetFacing(d);
				SaveLoc(0,tele);
				return true;
			}
		}
		if(m_pmap->GetUnknown(m_current,&d))
		{//Face set to unknown(no pic) area
			SetFacing(d);
			SaveLoc(0,tele);
			return true;
		}

		d = m_pmap->GetNextDirection(d);
	}

	return false;
}
//---------------------------------------------------------------------------------------------
bool CMyTug::FindShortCut(MY_POINT p,WORD *s)
{
	for(int i=m_index;i>*s;i--)
	{
		if((p.x==m_path[i].loc.x) && (p.y==m_path[i].loc.y))
		{
			*s = i;//Shortcut pos of the path arr (m_path)
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------------------
void CMyTug::Teleport(WORD *index)
{
	//Save tele source
	m_path[m_index].att		= m_pmap->m_path[*index].att;
	m_path[m_index].came_d	= m_pmap->m_path[*index].came_d;
	m_path[m_index].steps	= m_pmap->m_path[*index].steps;
	m_pmap->CopyPoint(&m_path[m_index].loc,&m_pmap->m_path[*index].loc);
	m_index++;

	//Teleport tug
	m_pmap->CopyPoint(&m_current,&m_pmap->m_path[*index-1].loc);
}
