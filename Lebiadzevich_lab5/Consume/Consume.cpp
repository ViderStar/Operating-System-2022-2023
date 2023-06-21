#include <conio.h>
#include <windows.h>
#include <iostream>
#include <cctype>

int main(int argc, char *argv[]) {
  HANDLE hWritePipe, hReadPipe;

  // преобразуем символьное представление дескриптора в число
  hWritePipe = (HANDLE)atoi(argv[1]);
  hReadPipe = (HANDLE)atoi(argv[2]);

  // ждем команды о начале работы
  std::cout << "Press any key to start communication.\n";
  std::cin.get();
  int size;
  char *arr;

  // читаем из анонимного канала
  DWORD dwBytesRead;
  ReadFile(hReadPipe, &size, sizeof(size), &dwBytesRead, NULL);
  _cprintf("size %d is read from the pipe \n", size);
  arr = new char[size];

  for (int i = 0; i < size; i++) {
    if (!ReadFile(hReadPipe, &arr[i], sizeof(arr[i]), &dwBytesRead, NULL)) {
      std::cout << "Read from the pipe failed.\n";
      std::cout << "GetLastError = " << GetLastError() << "\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return -1;
    }
    _cprintf("Element %c is read from the pipe.\n", arr[i]);
  }

  //обработка массива
  int num_removed;  

  std::cout << "Enter the number of elements to be removed: ";
  std::cin >> num_removed;

  // генерируем случайные индексы для удаления элементов
  for (int i = 0; i < num_removed; i++) {
    int index = rand() % size;
    for (int j = 0; j < i; j++) {
      if (arr[j] == index) {
        index = rand() % size;
        j = -1;
      }
    }
    arr[i] = index;
    arr[index] = arr[size - 1 - i];

  }
  size = size - num_removed;

  // пишем в анонимный канал
  DWORD dwBytesWritten;
  WriteFile(hWritePipe, &size, sizeof(size), &dwBytesWritten, NULL);
  _cprintf("\nSize %d is written to the pipe.\n", size);
  for (int i = 0; i < size; i++) {
    if (!WriteFile(hWritePipe, &arr[i], sizeof(arr[i]), &dwBytesWritten,
                   NULL)) {
      std::cout << "Write to file failed.\n";
      std::cout << "GetLastError = " << GetLastError() << "\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return -1;
    }
    _cprintf("Element %d is written to the pipe.\n", int(arr[i]));
  }

  // закрываем дескриптор канала
  CloseHandle(hWritePipe);
  CloseHandle(hReadPipe);

  _cprintf("result = ");
  for (int i = 0; i < size; i++) {
    _cprintf("\t%d", int(arr[i]));
  }
  _cprintf("\n");

  std::cout << "\nThe process finished writing to the pipe.\n";
  std::cout << "Press any key to exit.\n";
  std::cin.get();

  return 0;
}
