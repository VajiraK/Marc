// MyMapHelper.cpp: implementation of the CMyMapHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMapHelper.h"

CMyMapHelper::~CMyMapHelper(){}
//------------------------------------------------------------------------------------------------
CMyMapHelper::CMyMapHelper()
{
	m_index_vp = 0;
	//Initialiez map (Is it hard Mr.CPU)
	for (int x=0;x<MAP_EX;x++)
	{
		for (int y=0;y<MAP_EX;y++)
		{
			m_map[x][y].type = CL_UNKNOWN;
		}
	}
}
