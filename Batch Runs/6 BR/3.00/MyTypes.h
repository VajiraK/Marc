
#include <marccppapi.h>
using namespace marc;

//NORMALY USING BYTE TYPE
//SO USE VALUES BETWEEN 0-255
#define		MODE_GOING_HOME			1
#define		MODE_MINING_PACKS		2
#define		MODE_DIRTY				3
#define		MODE_FREE_ST			4
#define		MODE_UN_ST				5
#define		MODE_DOOR				6
#define		MODE_BLACK				7
#define		MODE_TELE				8
#define		MODE_SAVE_LOC			9
#define		MODE_NO_SAVE_LOC		10				

#define		FACE_UP                 1
#define		FACE_RIGHT              2
#define		FACE_LEFT               3
#define		FACE_DOWN               4

#define		CL_UNKNOWN              1
#define		CL_WALL                 2
#define		CL_FREE                 3
#define		CL_BATTERY              4
#define		CL_TELE                 5
#define		CL_DOOR                 6
#define		CL_BLACK                7
#define		CL_YELLOW               8
#define		CL_BROWN                9
#define		CL_PINK                 10

#define		RET_SUCCESS             1
#define		RET_STOP_MIS            2
#define		RET_ERROR               3
#define		RET_DIRTY_CELL			4
#define		RET_DOOR_CELL			5
#define		RET_TELE_BLOCK			6

#define		TELE_MAX				20	//CV
#define		HELTH_MARGIN			60	//CV
#define		MAP_EX					1000//CV
#define		MAP_INVALIED			MAP_EX+111//CV
#define		TAG_BLACKT_INDEX_BASE	0	//CV
#define		TAG_COLORT_INDEX_BASE	200	//CV
#define		TAG_BLACK				1
#define		TAG_COLOR				2

#define		ID_NOT_TELE				100
#define		ID_TELE					200

struct MY_POINT
{
	WORD x;
	WORD y;
};

struct MY_LOCATION
{
	MY_POINT	loc;
	BYTE		came_d;
	WORD		steps;
	BYTE		tele;
};

struct MY_MAP
{
	BYTE	type;
	BYTE	h;
};

struct MY_TAGS
{
	char	tag[4];
	BYTE	h;
	BYTE	type;
};

struct MY_TELE
{
	MY_POINT	loc;
	bool		dirty;
};