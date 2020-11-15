// MyBattery.h: interface for the CMyBattery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_)
#define AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGlobal.h"

class CMyBattery : public CMyGlobal
{
public:
			CMyBattery();
	virtual ~CMyBattery();
	BYTE	GetBoomID();
	void	UpdateBatBoom(BYTE id,BYTE reduceh);
	void	CreateBatBoom(BYTE dh,BYTE id);
	void	BatConsume(WORD unit);
	void	InitBat(CRobot &robot);
	DWORD	CheckBat(WORD new_st,WORD stock);
	DWORD	CheckBatEx(WORD upper,WORD new_st,WORD stock);
	int		GetBackSteps(WORD upper,int* turns);

private:
	DWORD		GetBoomLost(WORD stock);
	MY_BAT_BOOM	m_bat_boom;
};

#endif // !defined(AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_)
