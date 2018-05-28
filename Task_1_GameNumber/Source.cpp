#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <vector>
#include <time.h>
#include <commctrl.h>
using namespace std;

#pragma comment(lib, "comctl32")

HWND hTempButton;
HWND hList, hProgress;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

bool NumberIsInTheVector(vector<int> aRandNumbs, int randNumb);
void SortVector(vector<int> &aNumbers);

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
	static int iGuessedNumbers = 0;

	//static time_t tTime;	// время.
	static int nTime;

	switch (uMessage)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		return TRUE;

	case WM_INITDIALOG:

		hList = GetDlgItem(hWnd, IDC_LIST);
		hProgress = GetDlgItem(hWnd, IDC_PROGRESS);

		for (int i = IDC_BUTTON1; i < IDC_BUTTON16 + 1; i++)
		{
			// Делаем кнопки неактивными.
			EnableWindow(GetDlgItem(hWnd, i), FALSE);
		}

		// Тестовое заполнение - проверка сортировки.
		//for (int i = 0; i < aRandomNumbers.size(); i++)
		//{
		//	//aRandomNumbers[i];
		//	
		//	_itot(aRandomNumbers[i], szBufferText, 10);
		//	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)szBufferText);
		//}

		return TRUE;

	case WM_TIMER:
		//tTime = time(NULL);
		nTime++;

		if (nTime < 60) // TODO переменная
		{
			SendMessage(hProgress, PBM_STEPIT, 0, 0);
		}
		else
		{
			// Время вышло.
			KillTimer(hWnd, 1);
			MessageBeep(1);
		}

		return TRUE;

	case WM_COMMAND:

		// Новая игра.
		if (LOWORD(wParam) == IDC_BUTTON_NewGame) {

			aRandomNumbers.clear();
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			iGuessedNumbers = 0;

			for (int i = 0, y = IDC_BUTTON1; i < 16; i++, y++)
			{
				// Делаем кнопки активными.
				EnableWindow(GetDlgItem(hWnd, y), TRUE);

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

			SortVector(aRandomNumbers);


			//// TEST
			//Установка интервала для индикатора
			SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 60));
			//Установка шага приращения индикатора
			SendMessage(hProgress, PBM_SETSTEP, 1, 0);
			//Установка текущей позиции индикатора
			SendMessage(hProgress, PBM_SETPOS, 0, 0);
			//SetTimer(hWnd, 1, 100, NULL);

			// Установка таймера.
			SetTimer(hWnd, 1, 1000, NULL);
			nTime = 0;
		}
		// Нажатие на кнопки с числами.
		else if (LOWORD(wParam) >= IDC_BUTTON1 && LOWORD(wParam) <= IDC_BUTTON16)
		{
			hTempButton = GetDlgItem(hWnd, LOWORD(wParam));
			GetWindowText(hTempButton, szBufferText, 10);
			if (_wtoi(szBufferText) == aRandomNumbers[iGuessedNumbers])
			{
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)szBufferText);
				iGuessedNumbers++;
				EnableWindow(GetDlgItem(hWnd, LOWORD(wParam)), FALSE);
			}

			if (iGuessedNumbers == 15)
			{
				KillTimer(hWnd, 1);
			}
		}

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

void SortVector(vector<int> &aNumbers) {
	int temp;
	for (int i = 0; i < aNumbers.size() - 1; i++)
	{
		for (int j = 0; j < aNumbers.size() - 1; j++)
		{
			if (aNumbers[j + 1] < aNumbers[j])
			{
				//aNumbers[j] = aNumbers[j] + aNumbers[j + 1] - (aNumbers[j + 1] = aNumbers[j]);
				temp = aNumbers[j + 1];
				aNumbers[j + 1] = aNumbers[j];
				aNumbers[j] = temp;
			}
		}
	}
}