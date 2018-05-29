#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <vector>
#include <time.h>
#include <commctrl.h>
using namespace std;

#pragma comment(lib, "comctl32")

#define MIN 0
#define MAX 255

HWND hProgress;
HWND hRedSlider, hGreenSlider, hBlueSlider;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	srand(time(NULL));

	MSG msg;
	HWND hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_Main), NULL, DlgProc);
	ShowWindow(hDialog, nCmdShow);


	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}



BOOL CALLBACK DlgProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	TCHAR szCaption[35];
	TCHAR szRedBuffer[4];
	TCHAR szGreenBuffer[4];
	TCHAR szBlueBuffer[4];

	switch (uMessage)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		return TRUE;

	case WM_INITDIALOG:

		hProgress = GetDlgItem(hWnd, IDC_PROGRESS);
		hRedSlider = GetDlgItem(hWnd, IDC_SLIDER_Red);
		hGreenSlider = GetDlgItem(hWnd, IDC_SLIDER_Green);
		hBlueSlider = GetDlgItem(hWnd, IDC_SLIDER_Blue);

		SendMessage(hRedSlider, TBM_SETRANGE, TRUE, MAKELPARAM(MIN, MAX));
		SendMessage(hGreenSlider, TBM_SETRANGE, TRUE, MAKELPARAM(MIN, MAX));
		SendMessage(hBlueSlider, TBM_SETRANGE, TRUE, MAKELPARAM(MIN, MAX));

		SendMessage(hRedSlider, TBM_SETPOS, 1, 153);
		SendMessage(hGreenSlider, TBM_SETPOS, 1, 255);
		SendMessage(hBlueSlider, TBM_SETPOS, 1, 204);

		SendMessage(hProgress, PBM_SETBKCOLOR, 0, LPARAM(RGB(
			SendMessage(hRedSlider, TBM_GETPOS, 0, 0),
			SendMessage(hGreenSlider, TBM_GETPOS, 0, 0),
			SendMessage(hBlueSlider, TBM_GETPOS, 0, 0)
		)));

		return TRUE;

	case WM_HSCROLL:

		if (LPARAM(wParam) == TB_THUMBTRACK
			|| LPARAM(wParam) == TB_THUMBPOSITION
			|| LPARAM(wParam) == TB_PAGEUP
			|| LPARAM(wParam) == TB_PAGEDOWN
			|| LPARAM(wParam) == TB_LINEUP
			|| LPARAM(wParam) == TB_LINEDOWN
			|| LPARAM(wParam) == TB_TOP
			|| LPARAM(wParam) == TB_BOTTOM
			|| LPARAM(wParam) == TB_ENDTRACK)
		{
			SendMessage(hProgress, PBM_SETBKCOLOR, 0, LPARAM(RGB(
				SendMessage(hRedSlider, TBM_GETPOS, 0, 0),
				SendMessage(hGreenSlider, TBM_GETPOS, 0, 0),
				SendMessage(hBlueSlider, TBM_GETPOS, 0, 0)
			)));

			szCaption[0] = 0;

			_itot(SendMessage(hRedSlider, TBM_GETPOS, 0, 0), szRedBuffer, 10);
			_itot(SendMessage(hGreenSlider, TBM_GETPOS, 0, 0), szGreenBuffer, 10);
			_itot(SendMessage(hBlueSlider, TBM_GETPOS, 0, 0), szBlueBuffer, 10);
			lstrcat(szCaption, L"Red: ");
			lstrcat(szCaption, szRedBuffer);
			lstrcat(szCaption, L"  Green: ");
			lstrcat(szCaption, szGreenBuffer);
			lstrcat(szCaption, L"  Blue: ");
			lstrcat(szCaption, szBlueBuffer);
			SetWindowText(hWnd, szCaption);
		}

		return TRUE;

	}
	return FALSE;
}