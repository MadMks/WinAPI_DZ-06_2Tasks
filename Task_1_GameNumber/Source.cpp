#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <vector>
#include <time.h>
using namespace std;

HWND hTempButton;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

bool NumberIsInTheVector(vector<int> aRandNumbs, int randNumb);

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
	static vector<int> aRandomNumbers;
	TCHAR szBufferText[10];
	//int randNumber;

	switch (uMessage)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		return TRUE;

	case WM_INITDIALOG:

		for (int i = 0, y = IDC_BUTTON1; i < 16; i++, y++)
		{


			int randNumber;
			if (!aRandomNumbers.empty())
			{
				do
				{
					randNumber = rand() % 16;
				} while (NumberIsInTheVector(aRandomNumbers, randNumber));
			}
			else
			{
				randNumber = rand() % 16;
			}

			aRandomNumbers.push_back(randNumber);

			hTempButton = GetDlgItem(hWnd, y);
			_itot(aRandomNumbers[i], szBufferText, 10);
			SetWindowText(hTempButton, szBufferText);
		}
		/*for (int i = IDC_BUTTON1; i <= IDC_BUTTON16; i++)
		{
			hTempButton = GetDlgItem(hWnd, i);
			_itot(aRandomNumbers[])
			SetWindowText(hTempButton, )
		}*/

		return TRUE;

	case WM_COMMAND:

		/*if (LOWORD(wParam) == IDC_BUTTON_Create) {

		}*/

		return TRUE;


	}
	return FALSE;
}

bool NumberIsInTheVector(vector<int> aRandNumbs, int randNumb) {
	for (int i = 0; i < aRandNumbs.size(); i++)
	{
		if (aRandNumbs[i] == randNumb)
		{
			return TRUE;
		}
	}
	return FALSE;
}