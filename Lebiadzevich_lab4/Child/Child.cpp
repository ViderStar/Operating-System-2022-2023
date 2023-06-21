#include <iostream>
#include <windows.h>

const char* CHILD_EVENT_NAME = "ChildEvent";
const char* MUTEX_NAME = "ChildMutex";

int main() {
    HANDLE hChildEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, CHILD_EVENT_NAME);
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);

    WaitForSingleObject(hMutex, INFINITE);

    while (true) {
        WaitForSingleObject(hChildEvent, INFINITE);
        std::cout << "Child received message from Parent" << std::endl;
        break;
    }

    ReleaseMutex(hMutex);
    CloseHandle(hChildEvent);
    CloseHandle(hMutex);

    return 0;
}