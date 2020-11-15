// MyDirection.h: interface for the CMyDirection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDIRECTION_H__A3B22738_5A36_4242_963F_9CF1A57ED6D2__INCLUDED_)
#define AFX_MYDIRECTION_H__A3B22738_5A36_4242_963F_9CF1A57ED6D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyAction.h"

class CMyDirection : public CMyAction
{
public:
			CMyDirection();
	virtual ~CMyDirection();
	void	SetFacing(Angle a);
	BYTE	GetNextDirection(BYTE d);
	void	SetDirection(CRobot &robot,BYTE d);
};

#endif // !defined(AFX_MYDIRECTION_H__A3B22738_5A36_4242_963F_9CF1A57ED6D2__INCLUDED_)
