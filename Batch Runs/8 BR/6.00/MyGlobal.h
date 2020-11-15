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
	void			PictureToMap(MY_POINT p);
	void			CopyPoint(MY_POINT *sp,MY_POINT *dp);
	void			PlotLocation(MY_POINT *p,BYTE d,WORD step);
	void			PictureToMap(MY_POINT robo,MY_POINT p,BYTE d);
	//bool			FindShortCut(MY_POINT p,WORD lower,WORD *upper);
	void			AddToPath(CRobot &robot,BYTE psteps,BYTE tele=PA_NORMAL);
					
	MY_LOCATION		m_path[MAP_MAX_PATH];
	MY_POINT		m_point;//Is set by PictureToMap()
	MY_POINT		m_curent;//Current location of the robo
	MY_POINT		m_row_ord[25];
	//MY_POINT		m_boder_ord[20];
	MY_TELE			m_tele[TELE_MAX];//Dirty teles
	MY_MAP			m_map[MAP_EX][MAP_EX];
	//BYTE			m_tele_count;//Recored number of teleportations(AddDirtyTele)
	BYTE			m_facing;//Current facing og the robo
	int				m_rem_bat, m_full_bat; //m_rem_bat is set by BatConsume() SaveLoc()
	int				m_path_i;
	int				m_bat_passblack;
private:
	void			DirtyArea(MY_POINT loc);
};

#endif // !defined(AFX_MYGLOBAL_H__91E49957_21EB_47F9_87FD_535FE6C1411C__INCLUDED_)
