// MyMission.cpp: implementation of the CMyMission class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMission.h"

CMyMission::CMyMission(){}
CMyMission::~CMyMission(){}
//---------------------------------------------------------------------------------------------------
void CMyMission::Start(CRobot &robot)
{
CPicture picture;
	
	//One and only TackPic (The origin of every thing - Why... Mr Anderson why you persist!)
	while(DoTakePic(robot,picture))
	{
		//Update map
		FillArea(picture);

		if(SmartPick(robot)==RET_STOP_MIS)
			break;
		if(MakeDecision(robot)==RET_STOP_MIS)
			break;
		//Examin next area to move (Is robo has gone there befor...)
		if(QuarryArea(robot)==RET_STOP_MIS)
			break;
	}

	//Compliting
	Finish(robot);
}
//---------------------------------------------------------------------------------------------------
void CMyMission::GetReady(CRobot &robot)
{
	if(CheckRet(robot.StartMission())==RET_SUCCESS)
	{
		m_path_i = 0;
		BatInit(robot);
		PacInit(robot);
		SaveLoc(robot,0);
		Start(robot);
	}
}
//--------------------------------------------------------------------------------------------------
BYTE CMyMission::MakeDecision(CRobot &robot)
{
BYTE	ret;
WORD	steps = 0;
	
	//Drop any remaining packs(Unable to drop last time because of lack of space)
	DropBlackPacks(robot,m_cti,MODE_MINING_PACKS);

	//Robot front pic in Vitual Map so get free move steps
	steps = GetFreeSteps();
	
	if(steps!=0)
	{//There is room to go
		//Play with telies
		ret = TryTeleport(robot);
		switch(ret)
		{
		case RET_SUCCESS:
			return RET_SUCCESS;
		case RET_TELE_BLOCK://Shuold re evaluate space so return now
			return RET_SUCCESS;
		case RET_STOP_MIS:
			return RET_STOP_MIS;
		}

		//Move forward
		ret = DoMove(robot,steps);
		switch(ret)
		{
		case RET_SUCCESS:
			if(steps<5)
			{//Not hole picture epmty
				if(FindNewWay(robot)==RET_STOP_MIS)
					return RET_STOP_MIS;
			}
		break;
		case RET_STOP_MIS:
			return RET_STOP_MIS;
		}

	}else{
		//Facing to some obstacle
		if(FindNewWay(robot)==RET_STOP_MIS)
			return RET_STOP_MIS;
	}
	return RET_SUCCESS;
}
//-----------------------------------------------------------------------------------------------
BYTE CMyMission::FindNewWay(CRobot &robot)
{
	if(PassBlacks(robot,MODE_MINING_PACKS)==RET_STOP_MIS)
		return RET_STOP_MIS;
	if(CrackDoor(robot)==RET_STOP_MIS)
		return RET_STOP_MIS;
	if(FindGutter(robot)==RET_STOP_MIS)
		return RET_STOP_MIS;

	return RET_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
void CMyMission::Finish(CRobot &robot)
{
BYTE d;
WORD steps,s;
	for(int i=(m_path_i-1);i>=1;i--)
	{
		//Check short cut
		s=i;
		if(FindShortCut(m_path[i].loc,0,&s))
			i = s;
		//Teleport
		if(m_path[i].tele==ID_TELE)
		{
			robot.Teleport();
		}else{
	
			d = GetOposit(m_path[i].came_d);

			//Check space
			steps = 0;
			PictureToMap(m_path[i].loc);
			if(m_point.x!=MAP_INVALIED)
				steps = VirScanPic(m_path[i].loc,d,MODE_FREE_ST);

			//Set face
			if(d!=m_facing)
				SetDirection(robot,d);

			if(steps<m_path[i].steps)
			{//No space to go so PassBlacks
				m_curent.x=m_path[i].loc.x;m_curent.y=m_path[i].loc.y;
				PassBlacks(robot,MODE_GOING_HOME,i);
			}
			
			//Lets move to next location
			if(m_path[i].steps!=0)
				robot.Move(m_path[i].steps);
	
		}
	}

	//WriteMap();

CRobotInfo rinf;
	CheckRet(robot.EndMission(rinf));
}

