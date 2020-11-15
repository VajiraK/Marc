// MyGlobal.h: interface for the CMyGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYGLOBAL_H__91E49957_21EB_47F9_87FD_535FE6C1411C__INCLUDED_)
#define AFX_MYGLOBAL_H__91E49957_21EB_47F9_87FD_535FE6C1411C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include <marccppapi.h>
using namespace marc;

struct MY_POINT
{
	int	x;
	int	y;
};
struct MY_OBJECT
{
	ObjectType	type;
	MY_POINT	loc;
};
struct MY_ROW
{
	bool		empty;
	MY_OBJECT	ojc;
};
struct MY_LOCATION
{
	MY_POINT loc;
	int		 came_d;
	int		 steps;
};

class CMyGlobal
{
public:
	int GetBackSteps(int* turns);
			CMyGlobal();
	virtual ~CMyGlobal();
	void	CollectInfo(CRobot &robot);
	bool	IsDurtyLoc(int psteps);
	void	SaveLoc(CRobot &robot,int psteps);
	int		CheckRet(Response res,CRobot &robot);
	bool	FindSimilar(MY_POINT p,int *from);
	int		GetOposit(int d);

	MY_LOCATION	m_path[2000];
	int			path_i;
	MY_ROW		m_rows[5];
	bool		m_ad_check;
	int			m_facing;
	int			m_rem_bat,m_full_bat;
	int			m_rem_pack,m_storage;
private:
	
};

#endif // !defined(AFX_MYGLOBAL_H__91E49957_21EB_47F9_87FD_535FE6C1411C__INCLUDED_)
