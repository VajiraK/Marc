
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
#define		MODE_DROP_SECURE		8
#define		MODE_DROP_SPACE			9
#define		MODE_GOING_HOME			10
#define		MODE_MINING_PACKS		11

#define		FACE_UP                 1
#define		FACE_RIGHT              2
#define		FACE_LEFT               3
#define		FACE_DOWN               4

#define		VALIED					1
#define		INVALIED				2

#define		ST_COLOR				1
#define		ST_BLACK				2
#define		ST_SECURE				3
#define		ST_COL_BK				4
#define		ST_COL_SEC				5

#define		STORE_SEC				1
#define		STORE_BEL				2

#define		ST_MAX					130

#define		CL_UNKNOWN              1
#define		CL_WALL                 2
#define		CL_FREE                 3
#define		CL_BATTERY              4
#define		CL_TELE                 5
#define		CL_DOOR                 6
#define		CL_BLACK                7
#define		CL_DIRTY                8
#define		CL_NOT_DIRTY            9
#define		CL_BLOCK				11//For reseve teleport perimeter
#define		CL_BROWN                10
#define		CL_PINK                 15
#define		CL_YELLOW               20

#define		RET_SUCCESS             1
#define		RET_STOP_MIS            2
#define		RET_ERROR               3
#define		RET_SUCCESS_TELE        4
#define		RET_SUCCESS_DOOR        5
#define		RET_SUCCESS_GUTTER      6

#define		PA_NORMAL				100
#define		PA_TELE					101

#define		MAP_MAX_PATH			1000
#define		MAP_EX					1000

#define		TELE_MAX				30

struct MY_POINT
{
	WORD x;
	WORD y;
};

struct MY_PIC_ORD
{
	MY_POINT p[25];
	BYTE	 nr[26];
	BYTE	 st[26];
};

struct MY_LOCATION
{
	MY_POINT	loc;
	BYTE		came_d;
	BYTE		steps;
	BYTE		att;//attribute
};

struct MY_MAP
{
	BYTE	realbk;
	BYTE	dirty;
	BYTE	type;
	BYTE	h;
};

struct MY_TELE
{
	MY_POINT	loc;
	bool		dirty;
};

struct MY_ITEM
{
	BYTE	h;
	BYTE	type;
	BYTE	realbk;
	WORD	ntag;
	char	stag[4];
};

struct MY_TAG
{
	WORD	ntag;
	char	stag[4];
};

struct MY_PICK_ORD
{
	BYTE		h;
	BYTE		ord;
	BYTE		type;
	MY_POINT	p_map;
	MY_POINT	p_pic;
};

//***** EXCHNGE PATTERNS ******
struct MY_PAT
{
	BYTE	h[5];
	BYTE	icount;
};

struct MY_EP
{
	MY_PAT	pat[6];
	BYTE	count;
};
//***** EXCHNGE PATTERNS ******

//********** STOCK ************
struct MY_CAT_ITEM
{
	WORD	ntag;
	BYTE	type;
	BYTE	h;
	BYTE	store;
	BYTE	dirty;//Use in ReleaseExchange() it's self
};
struct MY_ST_CATALOG
{
	MY_CAT_ITEM item[ST_MAX];
	BYTE		count;
};
struct MY_ST_MAP_ITEM
{
	WORD	count;
	WORD	collected;
};
struct MY_ST_MAP
{
	MY_ST_MAP_ITEM	yellow;
	MY_ST_MAP_ITEM	pink;
	MY_ST_MAP_ITEM	brown;
};
struct MY_SC_POINT
{
	BYTE		type;
	WORD		ntag;
	char		stag[4];
	BYTE		h;
	MY_POINT	p;//Package location in the map
	MY_POINT	loc;//Robo location when securing
};
struct MY_SC_LOC
{
	MY_POINT  p;
	BYTE	  d;
};
struct MY_SC_LOCATIONS
{
	MY_SC_LOC	loc[500];
	WORD		count;
};
struct MY_SC_PACKS
{
	MY_SC_POINT packs[ST_MAX];
	BYTE		count;		
	BYTE		totalh;
};
struct MY_ST_ITEM
{
	BYTE	realbk;
	BYTE	type;
	BYTE	h;
	WORD	ntag;
	char	stag[4];
};
struct MY_PACKS
{
	MY_ST_ITEM  packs[ST_MAX];
	BYTE		count;
	BYTE		totalh;
};
struct MY_STOCK
{
	MY_PACKS		color;
	MY_PACKS		black;
	MY_SC_PACKS		secure;
	MY_ST_MAP		map;
};

//********** STOCK ************

