// MyBattery.h: interface for the CMyBattery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_)
#define AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyStock.h"

class CMyBattery : public CMyStock
{
public:
			CMyBattery();
	virtual ~CMyBattery();
	void	BatConsume(WORD unit);
	void	InitBat(CRobot &robot);
	BYTE	CheckBat(BYTE new_st,BYTE stock);
	BYTE	CheckBatEx(WORD upper,BYTE new_st,BYTE stock);
	int		GetBackSteps(WORD upper,int* turns);

	#ifdef _DEBUG
	DWORD	bug;
	#endif
	

private:
	DWORD	SendAgent(WORD from,BYTE stock);
};

#endif // !defined(AFX_MYBATTERY_H__F267D522_46C6_4C71_9418_37467A790A1A__INCLUDED_)
