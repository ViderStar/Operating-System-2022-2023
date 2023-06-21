#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include <stdio.h>

using namespace std;

int main() {
	int size;
	cout << "Input size of array: ";
	cin >> size;

	unsigned int* array = new unsigned int[size];
	for (int i = 0; i < size; i++) {
		cin >> array[i];
	}

	unsigned int numberToFind;
	cout << "Input number you want to find: ";
	cin >> numberToFind;

	unsigned int y;
	cout << "Input Y: ";
	cin >> y;

	string cmd_args;
	cmd_args = to_string(numberToFind);
	cmd_args += " ";
	cmd_args += to_string(size);
	cmd_args += " ";
	for (int i = 0; i < size; i++) {
		cmd_args += to_string(array[i]);
		if (i != size - 1) {
			cmd_args += " ";
		}
	}

	wstring stemp = wstring(cmd_args.begin(), cmd_args.end());

	LPCTSTR lpszAppName = L"child.exe";
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USEPOSITION;
	si.dwY = y;

	if (!CreateProcess(lpszAppName, &stemp[0], NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		cout << "New process is not created" << endl;
		return 0;
	}

	cout << "New process is created" << endl;
	WaitForSingleObject(piApp.hProcess, INFINITE);

	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	delete[]array;
	return 0;
}