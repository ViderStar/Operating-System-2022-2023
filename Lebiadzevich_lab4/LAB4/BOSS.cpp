#include <iostream>
#include <string>
#include <windows.h>

const char* PARENT_EVENT_NAME = "ParentEvent";
const char* CHILD_EVENT_NAME = "ChildEvent";
const char* SEMAPHORE_NAME = "ParentSemaphore";
const char* MUTEX_NAME = "ChildMutex";

void CreateProcessAndRun(const std::string& cmdLine) {
    PROCESS_INFORMATION pi;
    STARTUPINFO si;

    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (!CreateProcess(NULL, (LPSTR)cmdLine.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess() failed: " << GetLastError() << std::endl;
        return;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

int main() {
    // Создание объектов синхронизации
    HANDLE hParentEvent = CreateEvent(NULL, FALSE, FALSE, PARENT_EVENT_NAME);
    HANDLE hChildEvent = CreateEvent(NULL, FALSE, FALSE, CHILD_EVENT_NAME);
    HANDLE hSemaphore = CreateSemaphore(NULL, 2, 2, SEMAPHORE_NAME);
    HANDLE hMutex = CreateMutex(NULL, FALSE, MUTEX_NAME);

    // Запрос количества процессов Parent и Child
    int nParentProcesses, nChildProcesses;
    std::cout << "Enter the number of Parent processes: ";
    std::cin >> nParentProcesses;
    std::cout << "Enter the number of Child processes: ";
    std::cin >> nChildProcesses;

    // Запуск процессов Parent и Child
    for (int i = 0; i < nParentProcesses; ++i) {
        CreateProcessAndRun("parent.exe");
    }
    for (int i = 0; i < nChildProcesses; ++i) {
        CreateProcessAndRun("child.exe");
    }

    // Ожидание и отправка сообщений
    HANDLE events[] = { hParentEvent, hChildEvent };
    for (int i = 0; i < nParentProcesses + nChildProcesses; ++i) {
        DWORD result = WaitForMultipleObjects(2, events, FALSE, INFINITE);
        if (result == WAIT_OBJECT_0) {
            std::cout << "Received message from Parent process" << std::endl;
            SetEvent(hChildEvent);
        }
        else if (result == WAIT_OBJECT_0 + 1) {
            std::cout << "Received message from Child process" << std::endl;
            SetEvent(hParentEvent);
        }
    }

    // Завершение работы
    CloseHandle(hParentEvent);
    CloseHandle(hChildEvent);
    CloseHandle(hSemaphore);
    CloseHandle(hMutex);

    return 0;
}