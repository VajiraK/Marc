
#include <marccppapi.h>
using namespace marc;

//NORMALY USING BYTE TYPE
//SO USE VALUES BETWEEN 0-255
#define		MODE_CHECK_HELTH		1
#define		MODE_NO_CHECK_HELTH		2
#define		MODE_TUG				3
#define		MODE_ROBO				4
#define		MODE_BLACK				5
#define		MODE_COLOR				6
#define		MODE_SECURE				7
#define		MODE_COLE_SECURE		8//Color collection modes
#define		MODE_COLE_FRESH			9
#define		MODE_DROP_SECURE		10//Color droping modes
#define		MODE_DROP_SPACE			11
#define		MODE_DROP_BAT			12
#define		MODE_GOING_HOME			13
#define		MODE_MINING_PACKS		14

#define		FACE_UP                 1
#define		FACE_RIGHT              2
#define		FACE_LEFT               3
#define		FACE_DOWN               4

#define		VALIED					1
#define		INVALIED				2

#define		ST_MAX_H				1
#define		ST_LOW_P				2
#define		ST_BLACK				3
#define		ST_SECURE				4
#define		ST_ACTUAL				5
#define		ST_ALL					6
#define		ST_MAX					130

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
#define		CL_DIRTY                11
#define		CL_NOT_DIRTY            12

#define		RET_SUCCESS             1
#define		RET_STOP_MIS            2
#define		RET_ERROR               3
#define		RET_SUCCESS_TELE        4
#define		RET_SUCCESS_DOOR        5
#define		RET_SUCCESS_GUTTER      6

#define		PA_NORMAL				100
#define		PA_TELE					101

#define		BAT_BOOM_MAX			100

#define		MAP_MAX_PATH			1000
#define		MAP_EX					500
#define		MAP_INVALIED			MAP_EX+111

#define		SECURE_LIST_MAX			125
#define		TELE_MAX				20
#define		HELTH_MARGIN			60

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
	BYTE		att;//attribute
};

struct MY_MAP
{
	BYTE	dirty;
	BYTE	type;
	BYTE	h;
};

struct MY_TAGS
{
	char	tag[4];
};

struct MY_TELE
{
	MY_POINT	loc;
	bool		dirty;
};

struct MY_ST_ITEM
{
	BYTE	points;
	BYTE	exist;
	BYTE	h;
	BYTE	type;
	WORD	ntag;
	char	stag[4];
};

struct MY_ITEM
{
	BYTE	h;
	BYTE	type;
	WORD	ntag;
	char	stag[4];
};

struct MY_LUGGAGE
{
	BYTE		totalh;
	MY_ST_ITEM	item[ST_MAX];
};

struct MY_BOOM
{
	BYTE		valied;
	WORD		st_srt;
	WORD		st_nxt_b;
	BYTE		h;
};

struct MY_BAT_BOOM
{
	MY_BOOM		boom[BAT_BOOM_MAX];
	BYTE		i;
};

struct MY_SC_POINT
{
	BYTE		s;
	MY_POINT	p;
	BYTE		id;//MY_BOOM id
};

struct MY_SC_LIST
{
	MY_SC_POINT sp[SECURE_LIST_MAX];
	BYTE		packs;
	BYTE		totalh;
};

struct MY_PICK_ORD
{
	BYTE		h;
	MY_POINT	p_map;
	MY_POINT	p_pic;
};
