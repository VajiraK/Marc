// MyTug.h: interface for the CMyTug class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTUG_H__D36F00D8_2157_416A_995B_AC824CB38F94__INCLUDED_)
#define AFX_MYTUG_H__D36F00D8_2157_416A_995B_AC824CB38F94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyMap.h"

class CMyTug  
{
public:
			CMyTug();
	virtual ~CMyTug();
	void	Initialize(CMyMap *pmap,MY_POINT p,BYTE d);
	void	Teleport(WORD *index);
	void	SetFacing(BYTE d);
	WORD	Move(WORD st);
	bool	TryDirections(BYTE tele=PA_NORMAL);
	bool	FindShortCut(MY_POINT p,WORD *s);
	void	SaveLoc(WORD st,BYTE tele=PA_NORMAL);


	MY_LOCATION	m_path[MAP_MAX_PATH];
	BYTE		m_facing;
	WORD		m_index;

private:
	bool	IsBeenThere();

	MY_POINT	m_current;
	MY_POINT	*m_p;
	CMyMap		*m_pmap;
};

#endif // !defined(AFX_MYTUG_H__D36F00D8_2157_416A_995B_AC824CB38F94__INCLUDED_)
