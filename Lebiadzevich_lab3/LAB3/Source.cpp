#include <iostream>
#include <windows.h>
#include <algorithm>
using namespace std;

CRITICAL_SECTION cs;
CRITICAL_SECTION cs1;
int counterX;
HANDLE hOutEvent, hContinueEvent;
int k;

struct dataArray {
    int size;
    double* array;
    int X;
};

void bubble_sort(double* arr, int k, int sleep_time = 3) {
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

DWORD WINAPI work(void* obj) {
    EnterCriticalSection(&cs);
   
    dataArray* dAr = (dataArray*)obj;
    double* mas = dAr->array;
    bubble_sort(mas, dAr->size);

    cout << "Thread work is started" << endl;

    cout << "Input time of rest: ";
    int time;
    cin >> time;

    cout << "k array elements: ";
    for (int i = 0; i < k; ++i) {
        cout << mas[i] << " ";
        Sleep(time);
    }

    cout << endl;

    cout << endl << "Thread work is finished" << endl;
   // LeaveCriticalSection(&cs);
    LeaveCriticalSection(&cs);
    return 0;
}

DWORD WINAPI SumElement(void* obj) {
    EnterCriticalSection(&cs1);
    Sleep(3);
    dataArray* dAr = (dataArray*)obj;
    double* mas = dAr->array;
    cout << "Thread SumElement is started" << endl;

    int sum = 0;
    for (int i = 0; i < dAr->size; i++) {
        sum += mas[i];
    }
    cout << "Sum of array is " << sum << endl;
    LeaveCriticalSection(&cs1);

    SetEvent(hOutEvent);
    WaitForSingleObject(hContinueEvent, INFINITE);

    cout << endl << "Thread SumElement is finished" << endl;
    return 0;
}

int main() {
    int size;
    cout << "Input size of array: ";
    cin >> size;
    double* array = new double[size];
    cout << "Input elements: ";
    for (int i = 0; i < size; i++) {
        cin >> array[i];
    }

    cout << "Input k: ";
    cin >> k;

    cout << "The size of array is " << size << endl;
    cout << "Array: " << endl;
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }

    cout << endl;

    dataArray* d = new dataArray();
    d->size = size;
    d->array = array;

    InitializeCriticalSection(&cs);
    InitializeCriticalSection(&cs1);
    hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    hContinueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    HANDLE hThread;
    DWORD IDThread;
    DWORD dwSum;
    hThread = CreateThread(NULL, 0, work, (void*)d, CREATE_SUSPENDED, &IDThread);

    HANDLE hThreadSum;
    DWORD IDThreadSum;
    DWORD dwSumSum;
    hThreadSum = CreateThread(NULL, 0, SumElement, (void*)d, 0, &IDThreadSum);

    dwSum = ResumeThread(hThread);
   // dwSumSum = ResumeThread(hThreadSum);
    EnterCriticalSection(&cs);


    LeaveCriticalSection(&cs);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    DeleteCriticalSection(&cs);
    DeleteCriticalSection(&cs1);

    WaitForSingleObject(hOutEvent, INFINITE);

    SetEvent(hContinueEvent);

    WaitForSingleObject(hThreadSum, INFINITE);
    CloseHandle(hThreadSum);

    delete[] array;

    return 0;
}