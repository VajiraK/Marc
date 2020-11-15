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
	void			AddToPath(CRobot &robot,WORD psteps,BYTE tele=PA_NORMAL);
	bool			FindShortCut(MY_POINT p,WORD lower,WORD *upper);
	BYTE			CheckRet(Response res);
	BYTE			GetOposit(BYTE d);
	void			PictureToMap(MY_POINT p);
	void			PictureToMap(MY_POINT robo,MY_POINT p,BYTE d);
	void			PlotLocation(MY_POINT *p,BYTE d,WORD step);
	void			CopyPoint(MY_POINT *sp,MY_POINT *dp);
					
	MY_LOCATION		m_path[MAP_MAX_PATH];
	int				m_path_i;
	MY_TELE			m_tele[TELE_MAX];//Dirty teles
	BYTE			m_tele_count;//Recored number of teleportations(MakeVerPath/AddDirtyTele)
	MY_POINT		m_point;//Is set by PictureToMap()
	MY_POINT		m_curent;//Current location of the robo
	MY_MAP			m_map[MAP_EX][MAP_EX];
	BYTE			m_facing;//Current facing og the robo
	int				m_rem_bat, m_full_bat; //m_rem_bat is set by BatConsume() SaveLoc()
	int				m_bat_passblack;
private:
	void			DirtyArea(MY_POINT loc);
};

#endif // !defined(AFX_MYGLOBAL_H__91E49957_21EB_47F9_87FD_535FE6C1411C__INCLUDED_)
