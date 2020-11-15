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
					CMyGlobal();
	virtual			~CMyGlobal();
	BYTE			GetOposit(BYTE d);
	BYTE			CheckRet(Response res);
	BYTE			GetFreeSteps(MY_POINT robo,BYTE d);
	bool			PictureToMapEx(MY_POINT *pic,MY_POINT *map);
	bool			PictureToMap(MY_POINT robo,MY_POINT *pic,MY_POINT *map,BYTE d);
	void			CopyPoint(MY_POINT *sp,MY_POINT *dp);
	void			PlotLocation(MY_POINT *p,BYTE d,WORD step);
	void			AddToPath(BYTE psteps,BYTE tele=PA_NORMAL,bool dirty=true);
	void			GetMapPoint(MY_POINT *p,MY_ITEM *item);

	MY_SC_LOCATIONS	m_sec_pic_loc;
	MY_LOCATION		m_path[MAP_MAX_PATH];
	MY_POINT		m_curent;//Current location of the robo
	MY_PIC_ORD		m_row_ord;
	MY_TELE			m_tele[TELE_MAX];//Dirty teles
	MY_MAP			m_map[MAP_EX][MAP_EX];
	BYTE			m_rem_hel;
	BYTE			m_facing;//Current facing og the robo
	DWORD			m_rem_bat, m_full_bat,m_last_bat; //m_rem_bat is set by BatConsume() SaveLoc()
	int				m_path_i;
private:
	//void			DirtyArea(MY_POINT loc);
};

#endif // !defined(AFX_MYGLOBAL_H__91E49957_21EB_47F9_87FD_535FE6C1411C__INCLUDED_)
