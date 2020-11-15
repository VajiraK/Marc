// MyBattery.h: interface for the CMyBattery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_)
#define AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyStock.h"

class CMyBattery  : public CMyStock
{
public:
			CMyBattery();
	virtual ~CMyBattery();
	void	BatConsume(WORD unit);
	int		CheckBat(WORD new_st,WORD mew_stock);
	int		CheckBatEx(WORD upper,WORD new_st);
	void	InitBat(CRobot &robot);
	int		GetBackSteps(WORD upper,int* turns);
private:
};

#endif // !defined(AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_)
