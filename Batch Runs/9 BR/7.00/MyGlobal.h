// MyGlobal.h: interface for the CMyGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYGLOBAL_H__91E49957_21EB_47F9_87FD_535FE6C1411C__INCLUDED_)
#define AFX_MYGLOBAL_H__91E49957_21EB_47F9_87FD_535FE6C1411C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CMyGlobal
{
public:
	void			GetMapPoint(MY_POINT *p,MY_ITEM *item);
					CMyGlobal();
	virtual			~CMyGlobal();
	BYTE			GetOposit(BYTE d);
	BYTE			CheckRet(Response res);
	void			PictureToMap(MY_POINT p);
	void			CopyPoint(MY_POINT *sp,MY_POINT *dp);
	void			PlotLocation(MY_POINT *p,BYTE d,WORD step);
	void			PictureToMap(MY_POINT robo,MY_POINT p,BYTE d);
	void			AddToPath(BYTE psteps,BYTE tele=PA_NORMAL,bool dirty=true);
					
	MY_LOCATION		m_path[MAP_MAX_PATH];
	MY_POINT		m_point;//Is set by PictureToMap()
	MY_POINT		m_curent;//Current location of the robo
	MY_POINT		m_row_ord[25];
	MY_TELE			m_tele[TELE_MAX];//Dirty teles
	MY_MAP			m_map[MAP_EX][MAP_EX];
	BYTE			m_facing;//Current facing og the robo
	DWORD			m_rem_bat, m_full_bat; //m_rem_bat is set by BatConsume() SaveLoc()
	int				m_path_i;
	int				m_bat_passblack;
private:
	void			DirtyArea(MY_POINT loc);
};

#endif // !defined(AFX_MYGLOBAL_H__91E49957_21EB_47F9_87FD_535FE6C1411C__INCLUDED_)
