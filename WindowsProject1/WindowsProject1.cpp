#include "stdafx.h"
#include "WindowsProject1.h"
#include "DLL.h"
#include <windowsx.h>
#include <vector>
#include <string>
#include <fstream>
#include <Commdlg.h>
#include <commctrl.h>
#pragma comment(lib, "ComCtl32.lib")

using namespace std;


namespace PaintLibrary{

	Point newPoint(int x, int y)
	{
		Point p;
		p.x = x;
		p.y = y;

		return p;
	}

	COLORREF CShape::getColor()
	{
		return this->color;
	}

	int CShape::getSize()
	{
		return this->penSize;
	}

	void CShape::setType(ShapeMode type)
	{
		this->type = type;
	}

	RECT* CShape::getDimens()
	{
		RECT* rect = new RECT();
		rect->left = x1;
		rect->top = y1;
		rect->right = x2;
		rect->bottom = y2;

		return rect;
	}

	void CShape::setDimens(RECT* rect)
	{
		this->x1 = rect->left;
		this->x2 = rect->right;
		this->y1 = rect->top;
		this->y2 = rect->bottom;
	}

	void CShape::setColor(COLORREF color)
	{
		this->color = color;
	}


	int CShape::getPenStyle()
	{
		return this->penStyle;
	}


	void CLine::SetData(int a, int b, int c, int d, COLORREF color, int penStyle, int penSize)
	{
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;

		this->type = LINE;
		this->color = color;
		this->penStyle = penStyle;
		this->penSize = penSize;
	}

	void CLine::Draw(HDC hdc)
	{
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
	}

	CShape* CLine::Create()
	{
		return new CLine;
	}

	ShapeMode CLine::getType()
	{
		return this->type;
	}
	
}


#define MAX_LOADSTRING 100
#define MAX_BUFF		255

HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];		

HWND g_hWnd;
HWND hStatusBar;
Point p1, p2;
bool isDown = false;
vector<CShape*> shapes;
CShape* currShape = NULL;
COLORREF currColor = RGB(219, 50, 54); 
int currPenSize = 1;
int currPenStyle = PS_SOLID;
HPEN hPen = CreatePen(currPenStyle, currPenSize, currColor);
int windowWidth = 1080, windowHeight = 720;




ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


void initNewObject();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	MSG msg;
	HACCEL hAccelTable;


	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYPAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);


	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPAINT));


	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPAINT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYPAINT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
 
   hInst = hInstance;

   hWnd = CreateWindowEx(0, szWindowClass, L"Проект для малювання", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	   CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
					  INITCOMMONCONTROLSEX icc;
					  icc.dwSize = sizeof(icc);
					  icc.dwICC = ICC_WIN95_CLASSES;
					  InitCommonControlsEx(&icc);

					  g_hWnd = hWnd;

					  HFONT hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
					  hStatusBar = CreateWindowEx(0,
						  STATUSCLASSNAME,
						  L"Мій проект",
						  WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
						  0, windowHeight,
						  windowWidth, 20,
						  hWnd, (HMENU)IDC_STATUS_BAR,
						  hInst, NULL);
					  SendMessage(hStatusBar, WM_SETFONT, WPARAM(hFont), TRUE);  
	}
		break;
	case WM_LBUTTONDOWN:
		if (!isDown) 
		{
			p1 = newPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			initNewObject();
			isDown = true;
		}
		break;
	case WM_LBUTTONUP:
	{
						 isDown = false;
						 p2 = newPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

					

									  ::CLine* line = new CLine;
									  line->SetData(p2.x, p2.y, p1.x, p1.y, currColor, currPenStyle,currPenSize);
									  shapes.push_back(line);

						 InvalidateRect(hWnd, NULL, TRUE);
	}
		break;

	case WM_MOUSEMOVE:
	{
						 int x = GET_X_LPARAM(lParam);
						 int y = GET_Y_LPARAM(lParam);

						 //Status bar
						 WCHAR buffer[128];
						 wsprintf(buffer, L"%d, %dpx", x,y);
						 SetWindowText(hStatusBar, buffer);

						 if (isDown)
						 {
							 p2 = newPoint(x, y);
							 InvalidateRect(hWnd, NULL, TRUE);
						 }
	}
		break;
		
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case ID_EDIT_CLEAR:
			shapes.clear();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_COLOR_RED:
			currColor = RGB(219, 50, 54);
			break;
		case ID_COLOR_GREEN:
			currColor = RGB(60, 186, 84);
			break;
		case ID_COLOR_BLUE:
			currColor = RGB(72, 133, 237);
			break;
		case ID_COLOR_YELLOW:
			currColor = RGB(244, 194, 13);
			break;

		case ID_PENSTYLE_PS_SOLID:
			currPenStyle = PS_SOLID;
			break;
		case ID_PENSTYLE_PS_DASH:
			currPenStyle = PS_DASH;
			break;
		case ID_PENSTYLE_PS_DOT:
			currPenStyle = PS_DOT;
			break;
		case ID_PENSTYLE_PS_DASHDOT:
			currPenStyle = PS_DASHDOT;
			break;

		case ID_PENSTYLE_PS_1:
			currPenSize = 1;
			break;
		case ID_PENSTYLE_PS_2:
			currPenSize = 2;
			break;
		case ID_PENSTYLE_PS_3:
			currPenSize = 3;
			break;
		case ID_PENSTYLE_PS_4:
			currPenSize = 5;
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < shapes.size(); i++) 
		{
			hPen = CreatePen(shapes[i]->getPenStyle(), shapes[i]->getSize(), shapes[i]->getColor());
			SelectObject(hdc, hPen);
			shapes[i]->Draw(hdc);
		}

		if (isDown)
		{
			hPen = CreatePen(currPenStyle, currPenSize, currColor);
			SelectObject(hdc, hPen);
			currShape->SetData(p2.x, p2.y, p1.x, p1.y, currColor, currPenStyle, currPenSize);
			currShape->Draw(hdc);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
	{
					   PostQuitMessage(0);
	}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void initNewObject()
{
		currShape = new CLine();
}