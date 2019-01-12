#pragma once 
#include "Car.h"
#include "Rules.h"
#include <windows.h> 
#include <tchar.h>  
#include <windowsx.h>  

#define IDD_MENU1 3000
#define IDD_MENU2 3001
#define IDD_MENU3 3002
#define IDD_MENU4 3003
#define IDD_MENU5 3004


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");
TCHAR *menu = _T("Menu1");

//типы полей
enum Mode {Solid,One_gap,Two_Gap};

common_Rules *MainRule = NULL;
Mode Rmode = Two_Gap;
car * Car;
way * Way;

static int sx, sy;
bool turning = false;
int rool_nmbr = 0;

const double x_s = 650, y_s=100, dir_s=90;

BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	POINT pt;
	MoveToEx(hdc, x1, y1, &pt);
	return LineTo(hdc, x2, y2);
}

void draw_road(Mode sel,HBRUSH brush, HDC hdc)
{
	SelectObject(hdc, brush);
	Rectangle(hdc, 0, 0, 275, 1000);
	Rectangle(hdc, 775, 0, 1000, 1000);
	switch (sel)
	{
	case Solid:
		DrawLine(hdc,275,0,275,1000);
		DrawLine(hdc, 525, 0, 525, 1000);
		DrawLine(hdc, 775, 0, 775, 1000);
		break;
	case One_gap:
		DrawLine(hdc, 275 , 0, 275, 1000);
		DrawLine(hdc, 525, 0, 525, 400);
		DrawLine(hdc, 525, 600, 525, 1000);
		DrawLine(hdc, 775, 0, 775, 1000);
		break;
	case Two_Gap:
		DrawLine(hdc, 275, 0, 275, 1000);
		DrawLine(hdc, 525, 0, 525, 200);
		DrawLine(hdc, 525, 400, 525, 600);
		DrawLine(hdc, 525, 800, 525, 1000);
		DrawLine(hdc, 775, 0, 775, 1000);
		break;
	}
};

bool road_(double x, double y)
{
	return true;
	if (x >= 50 && x <= 950 && y >= 50 && y <= 950)
		return true;
	return false;
};

HBRUSH BackgroundBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 128, 0));
HBRUSH hGreenBrush = CreateSolidBrush(RGB(0, 128, 0));
HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH MainBrush = CreateSolidBrush(RGB(255, 255, 255));

int SCALE = 1000;


void transform(HDC& hdc)
{
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, SCALE, -SCALE, NULL);
	SetViewportExtEx(hdc, sx, sy, NULL);
	SetViewportOrgEx(hdc, 0, sy, NULL);
}

int APIENTRY _tWinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wc;
	HMENU hMainMenu = CreateMenu();
	HMENU hPopMenuRoad = CreatePopupMenu();
	HMENU hPopMenuRule = CreatePopupMenu();

	AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hPopMenuRoad, _T("Сменить участок дороги"));
	AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hPopMenuRule, _T("Сменить правила"));

	AppendMenu(hPopMenuRoad, MF_STRING, IDD_MENU1, _T("Сплошная"));
	AppendMenu(hPopMenuRoad, MF_STRING, IDD_MENU2, _T("Один разрыв"));
	AppendMenu(hPopMenuRoad, MF_STRING, IDD_MENU3, _T("Два разрыва"));

	AppendMenu(hPopMenuRule, MF_STRING, IDD_MENU4, _T("Правостороннее движение"));
	AppendMenu(hPopMenuRule, MF_STRING, IDD_MENU5, _T("Левостороннее движение"));

	wc.hInstance = This;
	wc.lpszClassName = WinName;
	wc.lpfnWndProc = WndProc; 
	wc.style = CS_HREDRAW | CS_VREDRAW; 
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.lpszMenuName = menu;
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hbrBackground = MainBrush;
	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "Не получилось зарегистрировать класс!", "Ошибка", MB_OK);
		return NULL; 
	}
	hWnd = CreateWindow(WinName, _T("Test"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,
		CW_USEDEFAULT,550,550,HWND_DESKTOP,NULL, This, NULL);
	ShowWindow(hWnd, mode); 
	SetMenu(hWnd, hMainMenu);
	SetMenu(hWnd, hPopMenuRoad);
	SetMenu(hWnd, hPopMenuRule);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rc;
	rc.top = 1000;
	rc.left = 280;
	rc.right = 770;
	rc.bottom = 950;
	HDC hdc;
	switch (message)
	{
	case WM_CREATE:
		MessageBox(NULL, "Стрелочки - перемещение.\nEsc - выход.\nTab - перезапуск дороги.", "Управление:", MB_OK);
		Car = new car(x_s, y_s, dir_s);
		Way = new way;
		Way->add_point(x_s, y_s);
		MainRule = new Rules(Rmode, 525, Way);
		break;
		//изменеие размеров окна
	case WM_COMMAND:
		//обработка меню
		switch (LOWORD(wParam)) 
		{
		case IDD_MENU1:
			Rmode = Solid;
			PostMessage(hWnd, WM_KEYDOWN, VK_TAB, 0);
			break;
		case IDD_MENU2:
			Rmode = One_gap;
			PostMessage(hWnd, WM_KEYDOWN, VK_TAB, 0);
			break;
		case IDD_MENU3:
			Rmode = Two_Gap;
			PostMessage(hWnd, WM_KEYDOWN, VK_TAB, 0);
			break;
		case IDD_MENU4:
			rool_nmbr = 0;
			MessageBox(NULL, "Правостороннее движение!", "Внимание", MB_OK);
			PostMessage(hWnd, WM_KEYDOWN, VK_TAB, 0);
			break;
		case IDD_MENU5:
			rool_nmbr = 1;
			MessageBox(NULL, "Левостороннее движение!", "Внимание", MB_OK);
			PostMessage(hWnd, WM_KEYDOWN, VK_TAB, 0);
			break;

		}
		break;
	case  WM_WINDOWPOSCHANGING:
		((LPWINDOWPOS)lParam)->cx = ((LPWINDOWPOS)lParam)->cy;
		((LPWINDOWPOS)lParam)->cy = ((LPWINDOWPOS)lParam)->cx;
		((LPWINDOWPOS)lParam)->cx = ((LPWINDOWPOS)lParam)->cy;
		if (((LPWINDOWPOS)lParam)->cx < 500)
		{
			((LPWINDOWPOS)lParam)->cx = 500;
			((LPWINDOWPOS)lParam)->cy = 500;
		}
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		case VK_TAB:
			delete Car;
			delete Way;
			delete MainRule;
			switch (rool_nmbr)
			{
			case 1:
				Car = new car(1000-x_s, y_s, dir_s);
				Way = new way;
				Way->add_point(1000-x_s, y_s);
				MainRule = new Rules1(Rmode, 525, Way);
				break;
			default:
				Car = new car(x_s, y_s, dir_s);
				Way = new way;
				Way->add_point(x_s, y_s);
				MainRule = new Rules(Rmode, 525, Way);
				break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_LEFT:
			Car->left();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_RIGHT:
			Car->right();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_UP:
			Car->forward(&road_,*Way);
			switch (MainRule->violation_id(*Car))
			{
			case 1:
				turning = false;
				MessageBox(NULL, "Пересечение края проезжей части!", "Нарушение", MB_OK);
				break;
			case 2:
				MessageBox(NULL, "Пересечение сплошной!", "Нарушение", MB_OK);
				turning = false;
				break;
			case 3:
				MessageBox(NULL, "Езда 'против шерсти'!", "Нарушение", MB_OK);
				turning = false;
				break;
			case 4:
				//в процессе разворота
				turning = true;
				break;
			case 5:
				turning = false;
				MessageBox(NULL, "Произведён разворот!", "Уведомление", MB_OK);
				break;
			default:
				break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			break;
		}
	case WM_PAINT:
		//рисование
		hdc = BeginPaint(hWnd, &ps);
		transform(hdc); 
		draw_road(Rmode,BackgroundBrush,hdc);
		SelectObject(hdc, hBlackBrush);
		SelectBrush(hdc, hGreenBrush);
		for (int i = 0; i < Way->get_l()-1; i++) 
		{
			DrawLine(hdc, Way->get_at(i).first, Way->get_at(i).second, Way->get_at(i+1).first, Way->get_at(i+1).second);		
		}
		Ellipse(hdc, Car->get_x() - 10, Car->get_y() + 10, Car->get_x() + 10, Car->get_y() - 10);
		DrawLine(hdc, Car->get_x(), Car->get_y(), Car->get_x() + Car->get_s()*cos(Car->get_dir()*pi / 180), Car->get_y() + Car->get_s()*sin(Car->get_dir()*pi / 180));
		if (turning) 
		{
			DrawText(hdc, TEXT("В процессе поворота!"), -1, &rc, DT_SINGLELINE | DT_VCENTER| DT_INTERNAL);
		}
		EndPaint(hWnd, &ps);
		break;
	
	case WM_DESTROY:
		//удаления созданный инструментарий рисования
		DeleteObject(hGreenBrush);
		DeleteObject(hBlackBrush);
		DeleteObject(MainBrush);
		delete Way;
		delete Car;
		delete MainRule;
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
