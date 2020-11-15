// MyDirection.cpp: implementation of the CMyDirection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyDirection.h"

CMyDirection::~CMyDirection(){}
//----------------------------------------------------------------------------------------
CMyDirection::CMyDirection()
{
	m_facing = FACE_DOWN;
}
//----------------------------------------------------------------------------------------
void CMyDirection::SetFacing(Angle a)
{
	switch (a) 
	{ 
	case DEGREES_90:
		switch (m_facing) 
		{ 
			case FACE_UP:
				m_facing = FACE_RIGHT;
			break;
			case FACE_DOWN:
				m_facing = FACE_LEFT;
			break;
			case FACE_LEFT:
				m_facing = FACE_UP;
			break;
			case FACE_RIGHT:
				m_facing = FACE_DOWN;
			break;
		}
	break;
	case DEGREES_180:
		switch (m_facing) 
		{ 
			case FACE_UP:
				m_facing = FACE_DOWN;
			break;
			case FACE_DOWN:
				m_facing = FACE_UP;
			break;
			case FACE_LEFT:
				m_facing = FACE_RIGHT;
			break;
			case FACE_RIGHT:
				m_facing = FACE_LEFT;
			break;
		}
	break;
	case DEGREES_MINUS_90:
		switch (m_facing) 
		{ 
			case FACE_UP:
				m_facing = FACE_LEFT;
			break;
			case FACE_DOWN:
				m_facing = FACE_RIGHT;
			break;
			case FACE_LEFT:
				m_facing = FACE_DOWN;
			break;
			case FACE_RIGHT:
				m_facing = FACE_UP;
			break;
		}
	break;
	}
}
//----------------------------------------------------------------------------------------
BYTE CMyDirection::GetNextDirection(BYTE d)
{
	switch (d) 
	{ 
		case FACE_DOWN:
			return FACE_LEFT;		
		break;
		case FACE_LEFT:
			return FACE_UP;		
		break;
		case FACE_RIGHT:
			return FACE_DOWN;		
		break;
		case FACE_UP:
			return FACE_RIGHT;	
		break;
	}	

	return 0;
}
//----------------------------------------------------------------------------------------
void CMyDirection::SetDirection(CRobot &robot,BYTE d)
{
	switch (d) 
	{ 
	case FACE_UP:
		switch (m_facing) 
		{ 
		case FACE_DOWN:
			DoTurn(robot,DEGREES_180);
		break;
		case FACE_LEFT:
			DoTurn(robot,DEGREES_90);
		break;
		case FACE_RIGHT:
			DoTurn(robot,DEGREES_MINUS_90);
		break;
		}	
	m_facing = FACE_UP;
	break;

	case FACE_DOWN:
		switch (m_facing) 
		{ 
		case FACE_UP:
			DoTurn(robot,DEGREES_180);
		break;
		case FACE_LEFT:
			DoTurn(robot,DEGREES_MINUS_90);
		break;
		case FACE_RIGHT:
			DoTurn(robot,DEGREES_90);
		break;
		}
	m_facing = FACE_DOWN;
	break;

	case FACE_LEFT:
		switch (m_facing) 
		{ 
		case FACE_UP:
			DoTurn(robot,DEGREES_MINUS_90);
		break;
		case FACE_DOWN:
			DoTurn(robot,DEGREES_90);
		break;
		case FACE_RIGHT:
			DoTurn(robot,DEGREES_180);
		break;
		}
	m_facing = FACE_LEFT;
	break;

	case FACE_RIGHT:
		switch (m_facing) 
		{ 
		case FACE_UP:
			DoTurn(robot,DEGREES_90);
		break;
		case FACE_DOWN:
			DoTurn(robot,DEGREES_MINUS_90);
		break;
		case FACE_LEFT:
			DoTurn(robot,DEGREES_180);
		break;
		}
	m_facing = FACE_RIGHT;
	break;
	}
}