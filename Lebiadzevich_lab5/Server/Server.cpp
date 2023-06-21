#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <cctype>
#include "Server.h"

int main() {
  char lpConsumeApp[] = "Consume.exe";

  int size;
  std::cout << "Enter size of array: ";
  std::cin >> size;

  char* arr = new char[size];
  std::cout << "Enter elements of array: ";
  for (int i = 0; i < size; i++) {
    std::cin >> arr[i];
  }

  _cprintf("size = %d\n", size);
  for (int i = 0; i < size; i++) {
    _cprintf("\t%c", arr[i]);
  }
/*
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  HANDLE hWritePipe1, hReadPipe1, hInheritWritePipe1;
  HANDLE hWritePipe2, hReadPipe2, hInheritReadPipe2;
  // pipe 1 - server read, consume write
  // pipe 2 - server write, consume read
  // ������� ��������� �����
  if (!CreatePipe(&hReadPipe1, &hWritePipe1, NULL, 0)) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Create pipe failed.\n";
    std::cout << "Press any key to finish.\n";
    std::cin.get();
    return -1;
  }

  // ������ ����������� �������� ����������� hWritePipe1
  if (!DuplicateHandle(GetCurrentProcess(), hWritePipe1, GetCurrentProcess(),
                       &hInheritWritePipe1, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
    std::cout << "Duplicate handle failed.\n";
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Press any key to finish.\n";
    std::cin.get();
    return -1;
  }

  CloseHandle(hWritePipe1);

  if (!CreatePipe(&hReadPipe2, &hWritePipe2, NULL, 0)) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Create pipe failed.\n";
    std::cout << "Press any key to finish.\n";
    std::cin.get();
    return -1;
  }

  // ������ ����������� �������� ����������� hReadPipe2
  if (!DuplicateHandle(GetCurrentProcess(), hReadPipe2, GetCurrentProcess(),
                       &hInheritReadPipe2, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
    std::cout << "Duplicate handle failed.\n";
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Press any key to finish.\n";
    std::cin.get();
    return -1;
  }

  CloseHandle(hReadPipe2); */
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  HANDLE hWritePipe1, hReadPipe1;
  SECURITY_ATTRIBUTES sa;

  // ������������� �������� ������ ������
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = NULL;	// ������ �� ���������
  sa.bInheritHandle = TRUE;		// ����������� �����������

  // ������� ��������� �����
  if (!CreatePipe(
	  &hReadPipe1,	// ���������� ��� ������
	  &hWritePipe1,// ���������� ��� ������
	  &sa,		// �������� ������ �� ���������,
	  // ����������� �����������
	  0))			// ������ ������ �� ���������

  {
	  _cputs("Create pipe failed.\n");
	  _cputs("Press any key to finish.\n");
	  _getch();
	  return GetLastError();
  }
  // ������������� �������� ������ ��������
  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  // ������������ ����������� �����������
  si.dwFlags = STARTF_USESTDHANDLES;
  // ������������� ����������� �����������
  si.hStdInput = hReadPipe1;
  si.hStdOutput = hWritePipe1;
  si.hStdError = hWritePipe1;
  // ��������� ������� �������
  if (!CreateProcess(
	  NULL,	// ��� ��������
	  lpszCom Line1,	// ��������� ������
	  NULL,	// �������� ������ �������� �� ���������
	  NULL,	// �������� ������ ���������� ������ �� ���������
	  TRUE,	// ����������� ����������� �������� ��������
	  // ����������� ����� ���������
	  CREATE_NEW_CONSOLE,	// ������� ����� �������
	  NULL,	// ���������� ����� ��������� �������� ������
	  NULL,	// ������� ���� � ������� ��� � � �������� ������
	  &si,	// ��� �������� ���� - �� ���������
	  &pi		// ����� ����� ����������� � ��������������
	  // ������ �������� � ��� ���������� ������
  )
	  )
  {
	  _cputs("Create process failed.\n");
	  _cputs("Press any key to finish.\n");
	  _getch();
	  return GetLastError();
  }
  // ��������� ����������� ������� �������
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  // ��������� ������� �������
  if (!CreateProcess(
	  NULL,	// ��� ��������
	  lpszComLine2,	// ��������� ������
	  NULL,	// �������� ������ �������� �� ���������
	  NULL,	// �������� ������ ���������� ������ �� ���������
	  TRUE,	// ����������� ����������� �������� ��������
	  // ����������� ����� ���������
	  CREATE_NEW_CONSOLE,	// ������� ����� �������
	  NULL,	// ���������� ����� ��������� �������� ������
	  NULL,	// ������� ���� � ������� ��� � � �������� ������
	  &si,	// ��� �������� ���� - �� ���������
	  &pi		// ����� ����� ����������� � ��������������
	  // ������ �������� � ��� ���������� ������
  )
	  )
  {
	  _cputs("Create process failed.\n");
	  _cputs("Press any key to finish.\n");
	  _getch();
	  return GetLastError();
  }
  // ��������� ����������� ������� �������
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  // ��������� ����������� ������
  CloseHandle(hReadPipe1);
  CloseHandle(hWritePipe1);
  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);

  // ��������� ��������� ������
  char lpConsume[1000] = "";
  wsprintf(lpConsume, "Consume.exe %d %d", (int)hInheritWritePipe1,
           (int)hInheritReadPipe2);

  //������� �������
  if (!CreateProcess(NULL, lpConsume, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL,
                     NULL, &si, &pi)) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "The Consume process is not created.\n";
    std::cout << "Press any key to finish.\n";
    std::cin.get();
    return -1;
  }

  // ��������� ����������� ������ ��������
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  // ��������� �������� ���������� ������
  CloseHandle(hInheritWritePipe1);
  CloseHandle(hInheritReadPipe2);

  // ����� � ��������� �����
  //������� ������ �������, �������� ������� � ����� N �������� Small.

  // pipe 1 - server read, consume write
  // pipe 2 - server write, consume read

  DWORD dwBytesWritten;
  WriteFile(hWritePipe2, &size, sizeof(size), &dwBytesWritten, NULL);
  _cprintf("\nsize %d is written to the pipe.\n", size);
  for (int i = 0; i < size; i++) {
    if (!WriteFile(hWritePipe2, &arr[i], sizeof(arr[i]), &dwBytesWritten,
                   NULL)) {
      std::cout << "Write to file failed.\n";
      std::cout << "GetLastError = " << GetLastError() << "\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return -1;
    }

    _cprintf("Element %c is written to the pipe.\n", arr[i]);
  }
  //WriteFile(hWritePipe2, &N, sizeof(N), &dwBytesWritten, NULL);
  //_cprintf("N %d is written to the pipe.\n", N);

  // ������ �� ���������� ������
  DWORD dwBytesRead;
  ReadFile(hReadPipe1, &size, sizeof(size), &dwBytesRead, NULL);
  _cprintf("\nSize %d is read from the pipe.\n", size);
  for (int i = 0; i < size; i++) {
    if (!ReadFile(hReadPipe1, &arr[i], sizeof(arr[i]), &dwBytesRead, NULL)) {
      std::cout << "Read from the pipe failed.\n";
      std::cout << "GetLastError = " << GetLastError() << "\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return -1;
    }
    _cprintf("Element %d is read from the pipe.\n", int(arr[i]));
  }

  // ��������� ���������� ������
  CloseHandle(hReadPipe1);
  CloseHandle(hWritePipe2);

  /*_cprintf("result = ");
  for (int i = 0; i < size; i++) {
      if (isdigit(arr[i])) {
      _cprintf("\t%d", int(arr[i]));
    }
  }
  _cprintf("\n");*/

  std::cout << "\nThe process finished.\n";
  std::cout << "Press any key to exit.\n";
  std::cin.get();

  return 0;
}