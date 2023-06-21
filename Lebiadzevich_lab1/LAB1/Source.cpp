#include <windows.h>
#include <iostream>
using namespace std;

struct dataArray {
	int size;
	int* array;
};

DWORD WINAPI worker(void* obj) {
	dataArray* dAr = (dataArray*)obj;
	int* mas = dAr->array;
	cout << "Thread worker is started" << endl;
	for (int i = 0; i < dAr->size; i++) {
		if (mas[i] < 0 && mas[i] % 6 == 0) {
			cout << mas[i] << " ";
		}
		Sleep(5);
	}
	cout << endl << "Thread worker is finished" << endl;
	return 0;
}

DWORD WINAPI count(void* obj) {
	int number = (int)obj;
	int* array = new int[number];
	array[0] = 0;
	array[1] = 1;
	cout << "Thread count is started" << endl;
	cout << array[0] << " " << array[1] << " ";
	for (int i = 2; i <= number; i++) {
		array[i] = array[i - 1] + array[i - 2];
		cout << array[i] << " ";
	}
	cout << endl << "Thread count is finished" << endl;
	return 0;
}

int main()
{
	int size;
	cout << "Input size of the array: ";
	cin >> size;

	int* arr = new int[size];
	cout << "Input the elements of the array" << endl;
	for (int i = 0; i < size; i++) {
		cin >> arr[i];
	}

	int stopTime, startTime;
	cout << "Input stop time ";
	cin >> stopTime;
	cout << "Input start time ";
	cin >> startTime;

	HANDLE hThread;
	DWORD IDThread;
	DWORD dwCount;

	dataArray* d = new dataArray();
	d->size = size;
	d->array = arr;
	
	hThread = CreateThread(NULL, 0, worker, (void*)d, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();

	Sleep(startTime);
	dwCount = SuspendThread(hThread);
	cout << endl << "Thread is stopped" << endl;

	Sleep(stopTime);
	dwCount = ResumeThread(hThread);
	cout << endl << "Thread is started" << endl;

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	delete[] arr;

	int num;
	cout << "Input number: ";
	cin >> num;

	DWORD IDThread1;
	HANDLE countThread = CreateThread(NULL, 0, count, (void*)num, 0, &IDThread1);
	if (countThread == NULL)
		return GetLastError();
	DWORD countCount = SuspendThread(countThread);
	Sleep(stopTime);
	countCount = ResumeThread(countThread);

	WaitForSingleObject(countThread, INFINITE);
	CloseHandle(countThread);

	return 0;
}