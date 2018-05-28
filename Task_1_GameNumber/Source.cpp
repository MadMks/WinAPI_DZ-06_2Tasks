#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <vector>
#include <time.h>
#include <commctrl.h>
using namespace std;

#pragma comment(lib, "comctl32")

#define NUMBER_OF_BUTTONS 16
#define SECONDS_TO_PLAY_AT_STARTUP 30
#define RANGE_OF_NUMBERS_STOP 100

HWND hTempButton;
HWND hList, hProgress, hSpin;

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
	static int iGuessedNumbers = 0;	// отгадано чисел.

	static int nCurrentTimeSeconds;	// текущее значение секунд игры.
	static int nTimePlay;	// время игры (секунд).

	switch (uMessage)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		return TRUE;

	case WM_INITDIALOG:

		hList = GetDlgItem(hWnd, IDC_LIST);
		hProgress = GetDlgItem(hWnd, IDC_PROGRESS);
		hSpin = GetDlgItem(hWnd, IDC_SPIN);

		for (int i = IDC_BUTTON1; i < IDC_BUTTON16 + 1; i++)
		{
			// Делаем кнопки неактивными.
			EnableWindow(GetDlgItem(hWnd, i), FALSE);
		}

		// Установка секунд по умолчанию, при старте игры.
		SendMessage(hSpin, UDM_SETPOS32, 0, SECONDS_TO_PLAY_AT_STARTUP);

		return TRUE;

	case WM_TIMER:

		nCurrentTimeSeconds++;

		if (nCurrentTimeSeconds <= nTimePlay)
		{
			SendMessage(hProgress, PBM_STEPIT, 0, 0);
		}
		else
		{
			// Время вышло.
			KillTimer(hWnd, 1);
			MessageBox(hWnd, L"Вы не успели!", L"Время вышло", MB_OK | MB_ICONASTERISK);

			for (int i = IDC_BUTTON1; i < IDC_BUTTON16 + 1; i++)
			{
				// Делаем кнопки неактивными.
				EnableWindow(GetDlgItem(hWnd, i), FALSE);
			}
		}

		return TRUE;

	case WM_COMMAND:

		// Новая игра.
		if (LOWORD(wParam) == IDC_BUTTON_NewGame) {

			aRandomNumbers.clear();
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			iGuessedNumbers = 0;

			for (int i = 0, y = IDC_BUTTON1; i < NUMBER_OF_BUTTONS; i++, y++)
			{
				// Делаем кнопки активными.
				EnableWindow(GetDlgItem(hWnd, y), TRUE);

				int randNumber;
				if (!aRandomNumbers.empty())
				{
					do
					{
						randNumber = rand() % RANGE_OF_NUMBERS_STOP;
					} while (NumberIsInTheVector(aRandomNumbers, randNumber));
				}
				else
				{
					randNumber = rand() % RANGE_OF_NUMBERS_STOP;
				}

				aRandomNumbers.push_back(randNumber);

				hTempButton = GetDlgItem(hWnd, y);
				_itot(aRandomNumbers[i], szBufferText, 10);
				SetWindowText(hTempButton, szBufferText);
			}

			SortVector(aRandomNumbers);


			nTimePlay = SendMessage(hSpin, UDM_GETPOS32, 0, 0);

			//Установка интервала для индикатора
			SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, nTimePlay));
			//Установка шага приращения индикатора
			SendMessage(hProgress, PBM_SETSTEP, 1, 0);
			//Установка текущей позиции индикатора
			SendMessage(hProgress, PBM_SETPOS, 0, 0);

			// Установка таймера.
			SetTimer(hWnd, 1, 1000, NULL);
			nCurrentTimeSeconds = 0;

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

			if (iGuessedNumbers == NUMBER_OF_BUTTONS)
			{
				KillTimer(hWnd, 1);
				MessageBox(hWnd, L"Вы выиграли!", L"Поздравляем", MB_OK | MB_ICONINFORMATION);
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
				temp = aNumbers[j + 1];
				aNumbers[j + 1] = aNumbers[j];
				aNumbers[j] = temp;
			}
		}
	}
}