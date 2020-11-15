// MyGutter.h: interface for the CMyGutter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYGUTTER_H__F272AEFA_3872_46C7_A76E_0395C267C288__INCLUDED_)
#define AFX_MYGUTTER_H__F272AEFA_3872_46C7_A76E_0395C267C288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyDoor.h"

#define		LEFT_EMPTY		1 
#define		RIGHT_EMPTY		2

#define		PLUS_Y			1
#define		MINUS_Y			2
#define		PLUS_X			3
#define		MINUS_X			4

class CMyGutter : public CMyDoor
{
public:
				CMyGutter();
	virtual		~CMyGutter();
	BYTE		FindGutter(CRobot &robot);

private:
	BYTE		CodSearch(BYTE mode);
	BYTE		QuaryGutter(CRobot &robot,MY_POINT p,BYTE stat);

	MY_POINT	m_gut_p; 
};

#endif // !defined(AFX_MYGUTTER_H__F272AEFA_3872_46C7_A76E_0395C267C288__INCLUDED_)
