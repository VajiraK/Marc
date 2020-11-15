// MyBattery.h: interface for the CMyBattery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_)
#define AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGlobal.h"

class CMyBattery  : public CMyGlobal
{
public:
			CMyBattery();
	virtual ~CMyBattery();
	void	BatConsume(WORD unit);
	int		CheckBat(CRobot &robot,WORD new_st,WORD mew_stock);
	void	BatInit(CRobot &robot);
private:
	int		GetBackSteps(int* turns);
};

#endif // !defined(AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_)
