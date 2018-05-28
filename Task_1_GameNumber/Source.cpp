#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <vector>
#include <time.h>
using namespace std;

HWND hTempButton;
HWND hList;

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

	switch (uMessage)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		return TRUE;

	case WM_INITDIALOG:

		hList = GetDlgItem(hWnd, IDC_LIST);

		for (int i = IDC_BUTTON1; i < IDC_BUTTON16 + 1; i++)
		{
			// ������ ������ �����������.
			EnableWindow(GetDlgItem(hWnd, i), FALSE);
		}

		// �������� ���������� - �������� ����������.
		//for (int i = 0; i < aRandomNumbers.size(); i++)
		//{
		//	//aRandomNumbers[i];
		//	
		//	_itot(aRandomNumbers[i], szBufferText, 10);
		//	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)szBufferText);
		//}

		return TRUE;

	case WM_COMMAND:

		// ����� ����.
		if (LOWORD(wParam) == IDC_BUTTON_NewGame) {

			aRandomNumbers.clear();
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			iGuessedNumbers = 0;

			for (int i = 0, y = IDC_BUTTON1; i < 16; i++, y++)
			{
				// ������ ������ ���������.
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
		}
		// ������� �� ������ � �������.
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

			// TODO ���� ��� ��������
			// ���� ��� ����
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