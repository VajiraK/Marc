// MyMission.cpp: implementation of the CMyMission class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMission.h"
#include "MyTug.h"

CMyMission::~CMyMission(){}
//---------------------------------------------------------------------------------------------------
CMyMission::CMyMission()
{	
	#ifdef _DEBUG
	if((MAP_EX!=500)||(MAP_MAX_PATH!=1000)||(HELTH_MARGIN!=55))
	{
		MessageBox(NULL,"Invalied critical constants!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
	}
	#endif
}
//---------------------------------------------------------------------------------------------------
void CMyMission::Start(CRobot &robot)
{
BYTE ret;

	while(true)
	{
		//Snap all unexplore directions
		ret = TakePictures(robot);
		if(ret==RET_STOP_MIS)
			break;
		//Try to go to a possible direction
		ret = TryDirections(robot);
		if(ret==RET_STOP_MIS)
			break;
		//No way to go let's Deploy Tug
		if(ret==RET_ERROR)
		{
			ret = DeployTug(robot);
			if((ret==RET_STOP_MIS)||(ret==RET_ERROR))
				break;
		}
	}
	//Let's go home bybe
	Finish(robot);
}
//--------------------------------------------------------------------------------------------------
BYTE CMyMission::GoForward(CRobot &robot)
{
BYTE ret;
BYTE steps = 0;
static WORD st = 0;
	//Drop any remaining packs (Unable to drop last time because of lack of space)
	DropBlackPacks(robot,GetColorCount(),GetOposit(m_path[m_path_i-1].came_d));

	if(MakeWay(robot,GetOposit(m_facing),1,MODE_CHECK_HELTH)==RET_STOP_MIS)
		return RET_STOP_MIS;

	if(FindNewWay(robot)==RET_STOP_MIS)
		return RET_STOP_MIS;

	//Robot front pic in Vitual Map so get free move steps(Full dirty raw is a blocking object)
	steps = GetFreeStepsEx(m_curent,m_facing,false);
	
	st += steps;//Secure packs***************
	if(st>20)
	{
		DropColorPacks(robot,GetOposit(m_facing),120,MODE_DROP_SECURE);
		st = 0;
	}//********** Secure packs **************

	if(steps!=0)
	{
	MY_POINT p;
		CopyPoint(&p,&m_curent);
		PlotLocation(&p,m_facing,steps);
		if(m_path_i>=2)
		{//For avoid robot looping
			if((p.x!=m_path[m_path_i-2].loc.x)||(p.y!=m_path[m_path_i-2].loc.y))
			{
				ret = DoMove(robot,steps);
				if(ret==RET_STOP_MIS)
				{//Crucial for collect secured
					SetDirection(robot,GetOposit(m_facing));
					return RET_STOP_MIS;
				}else{
					return ret;
				}
			}
		}else{
			return DoMove(robot,steps);
		}
	}
	else
	{
		return RET_ERROR;
	}
	return RET_ERROR;
}
//-----------------------------------------------------------------------------------------------
BYTE CMyMission::FindNewWay(CRobot &robot)
{
BYTE ret;

	//Teleports
	ret = TryTeleport(robot);
	switch (ret) 
	{ 
		case RET_SUCCESS:
			return RET_SUCCESS_TELE;		
		break;
		case RET_STOP_MIS:
			return RET_STOP_MIS;		
		break;
	}
	//Crack door
	ret = CrackDoor(robot);
	switch (ret) 
	{ 
		case RET_SUCCESS:
			return RET_SUCCESS_DOOR;		
		break;
		case RET_STOP_MIS:
			return RET_STOP_MIS;		
		break;
	}	
	//Gutters
	ret = FindGutter(robot);
	switch (ret) 
	{ 
		case RET_SUCCESS:
			return RET_SUCCESS_GUTTER;		
		break;
		case RET_STOP_MIS:
			return RET_STOP_MIS;		
		break;
	}

	return RET_ERROR;
}
//------------------------------------------------------------------------------------------------
BYTE CMyMission::TryDirections(CRobot &robot)
{
BYTE d,ret,of;

	d = of = m_facing;

	for(int i=0;i<=3;i++)
	{
		ret = GoForward(robot);
		if(ret==RET_ERROR)
		{//Cant go that way
			d = GetNextDirection(d);
			if(d==of)
				break;
			if(GetFresh(m_curent,&d))
			{//Found unexplore
				SetDirection(robot,d);//Set face to there and try to go there
			}else{
				break;
			}
		}else{
			if((ret==RET_STOP_MIS)||(ret==RET_SUCCESS))
				return ret;
		}
	}

	SetDirection(robot,of);//Reset direction
	return RET_ERROR;
}
//-------------------------------------------------------------------------------------------------
void CMyMission::Finish(CRobot &robot)
{
BYTE d;
WORD steps;
int  bs,nt,nb;
	bs=nt=nb=0; 
	
	for(int i=(m_path_i-1);i>=1;i--)
	{
		//Update current
		CopyPoint(&m_curent,&m_path[i].loc);

		//Teleport
		if(m_path[i].att==PA_TELE)
		{
			CollectColor(robot,MODE_COLE_SECURE);
			robot.Teleport();
		}else{
	
		d = GetOposit(m_path[i].came_d);

		CollectColor(robot,MODE_COLE_SECURE);
		//Check space
		steps = GetFreeSteps(m_path[i].loc,d);
		//Set face
		if(d!=m_facing)
		{
			SetDirection(robot,d);
			CollectColor(robot,MODE_COLE_SECURE);
		}

		if(steps<m_path[i].steps)
		{//No space to go so PassBlacks
			if(i==1)
			{//Take a pic of starting pos(Otherwice PassColors,PassBlacks will not work)
				if(IsPicTaken(m_curent,m_facing)==false)
					DoTakePic(robot);
			}
			MakeWay(robot,GetOposit(m_facing),m_path[i].steps,MODE_NO_CHECK_HELTH);
		}
		
		//Lets move to the next location
		if(m_path[i].steps!=0)
		{
			if(DoMove(robot,m_path[i].steps,MODE_GOING_HOME)==RET_ERROR)
				break;//Move Fail (Critical - Mission fails) why ooo! why
		}
		}
	}

#ifdef _DEBUG
	WORD y = GetScore();
	//WriteMap();
	int a = GetStock(ST_ACTUAL);
	int r = GetStock(ST_SECURE);
	int u = GetStock(ST_ALL);
	if(r!=0)
		MessageBox(NULL,"Not all collected!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
	if(a!=u)
		MessageBox(NULL,"Stock Unbalance!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
#endif

CRobotInfo rinf;
	CheckRet(robot.EndMission(rinf));
}
//--------------------------------------------------------------------------------------------------
BYTE CMyMission::MakeWay(CRobot &robot,BYTE drop,BYTE st,BYTE mode)
{
BYTE steps,bst;

	//Get the amount of steps can be cleared by removing blacks
	bst = IsBlackFeasible();
	if(bst==5)//Can't advance by removing blacks
		return RET_ERROR;

	for(BYTE i=0;i<3;i++)
	{
		if(PassBlacks(robot,drop,bst,mode)==RET_STOP_MIS)
		{
			if(mode==MODE_CHECK_HELTH)
				return RET_STOP_MIS;
		}
   
		steps = GetFreeSteps(m_curent,m_facing);

		if(steps>=st)
		{//Got space
			break;
		}else{
			#ifdef _DEBUG
			MessageBox(NULL,"Re-trying drop!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
			#endif
			drop = GetNextDirection(drop);
		}
	}
	return RET_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
BYTE CMyMission::TakePictures(CRobot &robot,bool back)
{
BYTE d,of,ret,priority;

	of = m_facing;
	priority = 0;//Determind the finaly facing direction (Success door got the hiest pririty)

//Current facing ****************************
	if(IsPicTaken(m_curent,m_facing)==false)
	{
		DoTakePic(robot);
		if(CollectColor(robot,MODE_COLE_FRESH)==RET_STOP_MIS)
			return RET_STOP_MIS;
	}
	ret = FindNewWay(robot);
	switch (ret) 
	{ 
		case RET_SUCCESS_TELE:
			if(TakePictures(robot,true)==RET_STOP_MIS)
				return RET_STOP_MIS;	
			else
				return RET_SUCCESS;		
		break;
		case RET_SUCCESS_DOOR:
			of = m_facing;
			priority = 2;		
		break;
		case RET_SUCCESS_GUTTER:
			of = m_facing;
			priority = 1;		
		break;
		case RET_STOP_MIS:
			return RET_STOP_MIS;
	}	
	
//Next direction ****************************
	d = GetNextDirection(m_facing);
	if(IsPicTaken(m_curent,d)==false)
	{
		SetDirection(robot,d);
		DoTakePic(robot);
		if(CollectColor(robot,MODE_COLE_FRESH)==RET_STOP_MIS)
			return RET_STOP_MIS;

		ret = FindNewWay(robot);
		switch (ret) 
		{ 
			case RET_SUCCESS_TELE:
				if(TakePictures(robot,true)==RET_STOP_MIS)
					return RET_STOP_MIS;	
				else
					return RET_SUCCESS;		
			break;
			case RET_SUCCESS_DOOR:
				if(priority!=2)
				{
					of = d;
					priority = 2;
				}
			break;
			case RET_SUCCESS_GUTTER:
				if(priority<1)
				{
					of = d;
					priority = 1;
				}
			break;
			case RET_STOP_MIS:
				return RET_STOP_MIS;
		}
	}

//Oposite direction ************************
	d = GetOposit(d);
	if(IsPicTaken(m_curent,d)==false)
	{
		SetDirection(robot,d);
		DoTakePic(robot);
		if(CollectColor(robot,MODE_COLE_FRESH)==RET_STOP_MIS)
			return RET_STOP_MIS;

		ret = FindNewWay(robot);
		switch (ret) 
		{ 
			case RET_SUCCESS_TELE:
				if(TakePictures(robot,true)==RET_STOP_MIS)
					return RET_STOP_MIS;	
				else
					return RET_SUCCESS;		
			break;
			case RET_SUCCESS_DOOR:
				if(priority!=2)
				{
					of = d;
					priority = 2;
				}
			break;
			case RET_SUCCESS_GUTTER:
				if(priority<1)
				{
					of = d;
					priority = 1;
				}
			break;
			case RET_STOP_MIS:
				return RET_STOP_MIS;
		}
	}
	
	//Original facing
	SetDirection(robot,of);
	return RET_SUCCESS;
}
//---------------------------------------------------------------------------------------------------
void CMyMission::GetReady(CRobot &robot)
{
	if(CheckRet(robot.StartMission())==RET_SUCCESS)
	{
		m_path_i = 0;
		InitBat(robot);

		if(InitStore(robot)==0)
		{//No pakeges (Empty ship)
			Finish(robot);
			return;
		}
		//Starting pos
	CLocation loc;
		robot.GetLocation(loc);
		m_curent.x  = (WORD)loc.GetX();
		m_curent.y  = (WORD)loc.GetY();
		AddToPath(robot,0);
		BatConsume(2);//For StartMission(),GetLocation()
		//Take the up picture
	CPicture picture;
		SetDirection(robot,FACE_UP);
		DoTakePic(robot);
		if(CollectColor(robot,MODE_COLE_FRESH)==RET_STOP_MIS)
		{
			Finish(robot);
			return;
		}
		SetDirection(robot,FACE_DOWN);
		//Let's play
		Start(robot);
	}
}
//-----------------------------------------------------------------------------------------------
BYTE CMyMission::DeployTug(CRobot &robot)
{
CMyTug		tug;
BYTE		d;
WORD		steps,i;
bool		got = false;

	//Initialize tugga
	tug.Initialize(this,m_curent,m_facing);

//********************** TUG ******************
	for(i=(m_path_i-1);i>=1;i--)
	{
		//Teleport
		if(m_path[i].att==PA_TELE)
		{
			//Search all directions
			if(tug.TryDirections())
			{
				got = true;
				break;
			}
			tug.Teleport(&i);
		}else{
			d = GetOposit(m_path[i].came_d);
			//Set face
			tug.SetFacing(d);
			//Search all directions
			if(tug.TryDirections())
			{
				got = true;
				break;
			}  
			//Lets move to the next location
			tug.Move(m_path[i].steps);
		}
	}
//********************** TUG ******************
	if(got==false)
	{
		#ifdef _DEBUG
		MessageBox(NULL,"Map sweeped!   ","Marc",MB_OK|MB_ICONEXCLAMATION);
		#endif
		return RET_ERROR;
	}
WORD p_size = m_path_i - (m_path_i-i-1);
//********************* ROBO ******************
//WORD s;
	for(i=0;i<=tug.m_index-1;i++)
	{
		/*s = i;
		if(tug.FindShortCut(tug.m_path[i].loc,&s))
		{//Found short cut
			if(s!=0)
				i = s;
		}*/
		//Teleport
		if(tug.m_path[i].att==PA_TELE)
		{
			CollectColor(robot,MODE_COLE_SECURE);
			robot.Teleport();
			CopyPoint(&m_curent,&tug.m_path[i+1].loc);
		}else{
			//Collect secure packs
			CollectColor(robot,MODE_COLE_SECURE);
			SetDirection(robot,tug.m_path[i].came_d);

	   		if(tug.m_path[i].steps!=0)
			{
				//Check space
				steps = GetFreeSteps(tug.m_path[i].loc,tug.m_path[i].came_d);
				
				if(steps<tug.m_path[i].steps)
				{//No space to go so PassBlacks
					CopyPoint(&m_curent,&tug.m_path[i].loc);
					MakeWay(robot,GetOposit(m_facing),tug.m_path[i].steps,MODE_NO_CHECK_HELTH);
				}
				//These move not save
				if(DoMoveEx(robot,tug .m_path[i].steps,m_path_i-i)==RET_STOP_MIS)
				{
					//Clip path
					m_path_i -= i;
					return RET_STOP_MIS;
				}
			}
		}
	}
	
	//Clip path
	m_path_i = p_size;

//******************** ROBO ******************
return RET_SUCCESS;
}
