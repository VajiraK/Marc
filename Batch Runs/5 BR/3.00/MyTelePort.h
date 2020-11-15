// MyTelePort.h: interface for the CMyTelePort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTELEPORT_H__525A8AE6_A0E0_4A18_937E_B1D3CDDC4219__INCLUDED_)
#define AFX_MYTELEPORT_H__525A8AE6_A0E0_4A18_937E_B1D3CDDC4219__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGutter.h"

class CMyTelePort : public CMyGutter
{
public:
			CMyTelePort();
	virtual ~CMyTelePort();
	BYTE	TryTeleport(CRobot &robot);

private:
	BYTE	TeleNow(CRobot &robot);
	BYTE	FromLeftRight(CRobot &robot,MY_POINT p,MY_POINT vp,WORD xstp,WORD xfrstp);
	BYTE	FromUpDown(CRobot &robot,MY_POINT p,MY_POINT vp,WORD ystp,WORD yfrstp);
	BYTE	CentreTeleport(CRobot &robot,MY_POINT p);
	void	DirtyTeleCouple(MY_POINT s,MY_POINT d);
};

#endif // !defined(AFX_MYTELEPORT_H__525A8AE6_A0E0_4A18_937E_B1D3CDDC4219__INCLUDED_)
