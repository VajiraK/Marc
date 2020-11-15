// MyMapHelper.h: interface for the CMyMapHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMAPHELPER_H__73435A7B_E58B_407A_9489_C192437F5B1A__INCLUDED_)
#define AFX_MYMAPHELPER_H__73435A7B_E58B_407A_9489_C192437F5B1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPackages.h"

class CMyMapHelper : public CMyPackages 
{
public:
			CMyMapHelper();
	virtual ~CMyMapHelper();
	//bool	PictureToMap(MY_POINT p);
	//WORD	VirScanPic(MY_POINT p,BYTE d,BYTE mode);
	//int		DoMove(CRobot &robot,WORD steps);

	MY_LOCATION	m_path_ver[100];
	WORD		m_index_vp;
private:
};

#endif // !defined(AFX_MYMAPHELPER_H__73435A7B_E58B_407A_9489_C192437F5B1A__INCLUDED_)
