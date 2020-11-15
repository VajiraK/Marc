// Marc.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "MyMission.h"

#define MAX_LOADSTRING 100

HINSTANCE	hInst;
CMyMission	g_mission;
CRobot&		g_robot = CRobot::GetRobot();

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	MSG msg;

	g_mission.GetReady(g_robot);

	/*MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}


	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/

	return msg.wParam;
}
//-----------------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MARC);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "Marc";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}
//-----------------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance;

   hWnd = CreateWindow("Marc","Play Marc", WS_BORDER|WS_POPUP|WS_SYSMENU|WS_CAPTION|WS_POPUPWINDOW|WS_VISIBLE ,250,100,250,200,GetDesktopWindow(),NULL,hInstance,NULL);
   
   CreateWindow("BUTTON","Ok",WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,75,50,100,50,hWnd,(HMENU)ID_BUT_OK,hInstance,NULL );


   if (!hWnd)
   {
      return FALSE;
   }

   UpdateWindow(hWnd);

   return TRUE;
}
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

			switch (wmId)
			{
				case ID_BUT_OK:
					//g_mission.GetReady(g_robot);
					PostQuitMessage(0);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
