#include <iostream>
#include <windows.h>

const char* PARENT_EVENT_NAME = "ParentEvent";
const char* CHILD_EVENT_NAME = "ChildEvent";
const char* SEMAPHORE_NAME = "ParentSemaphore";

int main() {
    HANDLE hParentEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, PARENT_EVENT_NAME);
    HANDLE hChildEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, CHILD_EVENT_NAME);
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SEMAPHORE_NAME);

    WaitForSingleObject(hSemaphore, INFINITE);

    while (true) {
        WaitForSingleObject(hParentEvent, INFINITE);
        std::cout << "Parent received message from Boss" << std::endl;
        SetEvent(hChildEvent);
        break;
    }

    ReleaseSemaphore(hSemaphore, 1, NULL);
    CloseHandle(hParentEvent);
    CloseHandle(hChildEvent);
    CloseHandle(hSemaphore);

    return 0;
}