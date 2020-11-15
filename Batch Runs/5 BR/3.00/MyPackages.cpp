// MyPackages.cpp: implementation of the CMyPackages class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyPackages.h"

CMyPackages::~CMyPackages(){}
CMyPackages::CMyPackages()
{
	m_bti   = TAG_BLACKT_INDEX_BASE;
	m_cti   = TAG_COLORT_INDEX_BASE;
	m_helth = 100;
}
//------------------------------------------------------------------------------------------------
BYTE CMyPackages::PassBlacks(CRobot &robot,BYTE mode,int pathi)
{//Pick and drop bihind Black packages
MY_POINT	p;
BYTE		bst;
int			x,y,oldcol;

	//Check helth
	if(m_helth<HELTH_MARGIN)
	{
		if(mode==MODE_MINING_PACKS)
			return RET_SUCCESS;
	}

	//Save color tag index
	oldcol = m_cti;
	//Get the amount of steps can be cleared by removing blacks
	bst = 5-VirScanPic(m_curent,m_facing,MODE_BLACK);

	for (y=4;y>=bst;y--)
	{for (x=0;x<=4;x++)
	{	
		p.x=x;p.y=y;
		BlackPick(p,robot,pathi,mode);
	}}

	if(DropBlackPacks(robot,oldcol,mode,pathi)==RET_STOP_MIS)
		return RET_STOP_MIS;

	return RET_SUCCESS;
}
//----------------------------------------------------------------------------------------------
BYTE CMyPackages::BlackPick(MY_POINT p,CRobot &robot,int pathi,BYTE mode)
{
BYTE	t,ret;
	
	//Check helth
	if(m_helth<HELTH_MARGIN)
	{
		if(mode==MODE_MINING_PACKS)
			return RET_SUCCESS;
	}

	//Set m_point
	PictureToMap(p);
	//In map eages  
	if(m_point.x==MAP_INVALIED)
		return RET_SUCCESS;

	t = m_map[m_point.x][m_point.y].type;

	if(t==CL_BLACK)
	{
		if(m_storage>=115)
		{//No room for blackies so drop some color
			DropColorPacks(robot,pathi,mode,10);
			//Remap m_point because DropColorPacks changed it
			PictureToMap(p);
		}

		//Make a new tag
		m_bti++;
		MakeTag(m_bti,TAG_BLACK);

		//Lest,s pick
		ret = DoPickUp(robot,p,m_tag[m_bti].tag);
		//ret = CheckRet(robot.PickupPackage(p.x,p.y,m_tag[m_bti].tag));
		switch (ret) 
		{ 
		case RET_SUCCESS:
			m_bat_passblack += 10;
			//Bind hieght to tag (To use when droping)
			m_tag[m_bti].h = m_map[m_point.x][m_point.y].h;
			m_tag[m_bti].type = m_map[m_point.x][m_point.y].type;
			//Add to store
			m_storage	   += m_map[m_point.x][m_point.y].h;
			if(m_helth!=0)
				m_helth--;//Drop two helth(It may meet again when going home)
			//Clear it from vertual map
			m_map[m_point.x][m_point.y].type = CL_FREE;
			return RET_SUCCESS;
		}

	}
	return RET_SUCCESS;
}
//--------------------------------------------------------------------------------------------
BYTE CMyPackages::DropBlackPacks(CRobot &robot,int oldcol,BYTE mode,int pathi)
{
BYTE ret;
int	x,y;
MY_POINT	p;
int			steps;
BYTE		d;

	if(m_bti==TAG_BLACKT_INDEX_BASE)
		return RET_SUCCESS;

	d = m_facing;
	//Drop them in came direction
	if(mode==MODE_MINING_PACKS)
	{
		SetDirection(robot,GetOposit(m_path[m_path_i-1].came_d));
	}else{
		SetDirection(robot,m_path[pathi+1].came_d);
	}

	for (y=4;y>=0;y--)
	{for (x=4;x>=0;x--)
		{	
			if(m_bti != TAG_BLACKT_INDEX_BASE)
			{
				p.x=x;p.y=y;
				PictureToMap(p);
				if(m_point.x!=MAP_INVALIED)
				{
					if(m_map[m_point.x][m_point.y].type==CL_FREE)
					{
					MY_POINT tmp;
						tmp.x=x;tmp.y=y;
						ret = DoDrop(robot,tmp,m_tag[m_bti].tag);
						//ret = CheckRet(robot.DropPackage(x,y,m_tag[m_bti].tag));
						switch (ret) 
						{
						case RET_SUCCESS:
							//Reduce storage
							m_storage -= m_tag[m_bti].h;
							//Place it in vertual map
							m_map[m_point.x][m_point.y].type = m_tag[m_bti].type;
							m_map[m_point.x][m_point.y].h	 = m_tag[m_bti].h;
							m_bti--;
							break;
						}
					}
				}
			}else{
				//All collected blacks proped
				break;
			}
		}
	}

	//Is some colors drop in the proccess
	if(oldcol!=m_cti)
		SmartPick(robot);
	
	if(mode==MODE_MINING_PACKS)
	{	//Reset direction (After droping back)
		SetDirection(robot,d);
		steps = VirScanPic(m_curent,m_facing,MODE_FREE_ST);
		if(steps!=0)
		{//Go through the cliered area
			ret = DoMove(robot,steps);
			if(ret==RET_STOP_MIS)
				return RET_STOP_MIS;
		}
	}else{//Going home (Call by Finish())
		SetDirection(robot,d);
	}
return RET_SUCCESS;
}
//--------------------------------------------------------------------------------------------
BYTE CMyPackages::DropColorPacks(CRobot &robot,int pathi,BYTE mode,BYTE amt)
{//This func make room to pick blakies (Store has fild)
int		dh = 0;//Drop height
int		x,y;
BYTE	d,ret;
MY_POINT p;

	d = m_facing;

	//Drop some color packs in came direction
	if(mode==MODE_MINING_PACKS)
	{
		SetDirection(robot,GetOposit(m_path[m_path_i-1].came_d));
	}else{
		SetDirection(robot,m_path[pathi+1].came_d);
	}

	for (y=4;y>=0;y--)
	{for (x=4;x>=0;x--)
		{	
			if(m_cti != TAG_COLORT_INDEX_BASE)
			{
				//Only some height of packs
				if(dh>=amt)
				{
					SetDirection(robot,d);
					return RET_SUCCESS;
				}
				p.x=x;p.y=y;
				PictureToMap(p);
				if(m_point.x!=MAP_INVALIED)
				{
					if(m_map[m_point.x][m_point.y].type==CL_FREE)
					{
					MY_POINT tmp;
						tmp.x=x;tmp.y=y;
						ret = DoDrop(robot,tmp,m_tag[m_cti].tag);
						//ret = CheckRet(robot.DropPackage(x,y,m_tag[m_cti].tag));
						switch (ret) 
						{
						case RET_SUCCESS:
							m_storage -= m_tag[m_cti].h;
							//Increase remaining packs
							m_rem_pack++;
							//Cal droped color height
							dh += m_tag[m_cti].h;
							//Place it in vertual map
							m_map[m_point.x][m_point.y].type = m_tag[m_cti].type;
							m_map[m_point.x][m_point.y].h	 = m_tag[m_cti].h;
							m_cti--;
							break;
						}
					}
				}
			}else{
				//Collected colors over
				SetDirection(robot,d);
				return RET_SUCCESS;
			}
		}
	}
	return RET_SUCCESS;
}
//-----------------------------------------------------------------------------------------
BYTE CMyPackages::SmartPick(CRobot &robot)
{
MY_POINT	p;
BYTE ret;
	for (p.x=0;p.x<=4;p.x++)
	{for (p.y=0;p.y<=4;p.y++)
	{//BATTERY
		//Set m_point
		PictureToMap(p);
		//In map eages
		if(m_point.x!=MAP_INVALIED)
		{
			if(m_map[m_point.x][m_point.y].type==CL_BATTERY)
			{
				//Battory pick
				ret = DoPickUp(robot,p);
				//Response res = robot.PickupPackage(p.x,p.y,"");
				if(ret==RET_SUCCESS)
				{
					//Recharge
					m_rem_bat += m_map[m_point.x][m_point.y].h*100;
					//Clear it from vertual map
					m_map[m_point.x][m_point.y].type = CL_FREE; 
					m_map[m_point.x][m_point.y].h	 = 0;
					if(m_rem_bat>m_full_bat)
						m_rem_bat = m_full_bat;
				}else{
					return ret;
				}
			}
		}
	}}
	//===================================================
	for (p.x=0;p.x<=4;p.x++)
	{for (p.y=0;p.y<=4;p.y++)
	{//YELLOW
			if(PickColor(robot,p,CL_YELLOW)==RET_STOP_MIS)
				return RET_STOP_MIS;
	}}
	//===================================================
	for (p.x=0;p.x<=4;p.x++)
	{for (p.y=0;p.y<=4;p.y++)
	{//PINK
			if(PickColor(robot,p,CL_PINK)==RET_STOP_MIS)
				return RET_STOP_MIS;
	}}
	//===================================================
	for (p.x=0;p.x<=4;p.x++)
	{for (p.y=0;p.y<=4;p.y++)
	{//BROWN
			if(PickColor(robot,p,CL_BROWN)==RET_STOP_MIS)
				return RET_STOP_MIS;
	}}
	//===================================================

return RET_SUCCESS;
}
//------------------------------------------------------------------------------------------------
BYTE CMyPackages::PickColor(CRobot &robot,MY_POINT p,BYTE c)
{
BYTE	t;
int		ret,oh;

	//Set m_point
	PictureToMap(p);
	//In map eages
	if(m_point.x==MAP_INVALIED)
		return 0;

	t = m_map[m_point.x][m_point.y].type;

	if(t==c)
	{
		oh = m_map[m_point.x][m_point.y].h;

		//Check with cur stock
		if((m_storage+oh)>120)
			return RET_STOP_MIS;

		//Make a new tag
		m_cti++;
		MakeTag(m_cti,TAG_COLOR);
		//Lest,s pick
		ret = DoPickUp(robot,p,m_tag[m_cti].tag);
		//ret = CheckRet(robot.PickupPackage(p.x,p.y,m_tag[m_cti].tag));

		switch (ret) 
		{ 
		case RET_SUCCESS:
			//Bind info to tag (To use when droping)
			m_tag[m_cti].h	  = oh;
			m_tag[m_cti].type = t;
			//Add to store
			m_storage	   += oh;
			//Clear it from vertual map
			m_map[m_point.x][m_point.y].type = CL_FREE; 
			m_map[m_point.x][m_point.y].h	 = 0;
			//Check remainig packs in the map
			m_rem_pack--;
			if(m_rem_pack==0)
				return RET_STOP_MIS;
			return RET_SUCCESS;
		case RET_ERROR:
			m_cti--;
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------------
void CMyPackages::MakeTag(WORD tinum,BYTE mode)
{//Capacity <9999
char* pt;
int		pos[4];
int		stop;
DWORD	n = tinum;//Reserve original

	if(mode==TAG_BLACK)
		pt = m_tag[m_bti].tag;
	else
		pt = m_tag[m_cti].tag;

	ZeroMemory(pt,4);

	//Set loop*********************************************
	if(n>999)
		{//1000
			stop = 3;
		}else{

			if(n>99)
			{//100
				stop = 2;
			}else{
				if(n>9)
				{//10
					stop = 1;
				}else{//1
					stop = 0;
				}
			}
		}

	//split intiger******************************************
	pos[3] =  n/1000;
	n = n - pos[3]*1000;
	pos[2] =  n/100;
	n = n - pos[2]*100;
	pos[1] =  n/10;
	n = n - pos[1]*10;
	pos[0] =  n;
	
	//convert to char****************************************
	for(int i=0;i<=stop;i++)
	{
		switch (pos[stop-i])
		{
		case 0:
			pt[i] = '0';break;
		case 1:
			pt[i] = '1';break;
		case 2:
			pt[i] = '2';break;
		case 3:
			pt[i] = '3';break;
		case 4:
			pt[i] = '4';break;
		case 5:
			pt[i] = '5';break;
		case 6:
			pt[i] = '6';break;
		case 7:
			pt[i] = '7';break;
		case 8:
			pt[i] = '8';break;
		case 9:
			pt[i] = '9';break;
		}
	}
}
//------------------------------------------------------------------------------------------------
void CMyPackages::PacInit(CRobot &robot)
{
CSpaceShipInfo	sinf;

	m_storage		= 0;

	//Packs collection
	CMarc::GetSpaceShipInfo(sinf);
	m_rem_pack  = 0;
	m_rem_pack  = sinf.GetBrownPackageCount();
	m_rem_pack += sinf.GetYellowPackageCount();
	m_rem_pack += sinf.GetPinkPackageCount();
}
