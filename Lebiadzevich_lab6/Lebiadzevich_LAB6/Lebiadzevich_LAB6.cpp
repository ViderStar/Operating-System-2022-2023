#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <thread>

struct dataArray {
    int size;
    std::vector<double> array;
};

void bubble_sort(std::vector<double>& arr, int k, int sleep_time = 3) {
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void work(std::mutex& mtx, std::condition_variable& cv, dataArray* dAr) {
    std::unique_lock<std::mutex> lock(mtx);
    bubble_sort(dAr->array, dAr->size);

    std::cout << "Thread work is started" << std::endl;

    std::cout << "Input time of rest: ";
    int time;
    std::cin >> time;

    std::cout << "k array elements: ";
    for (int i = 0; i < dAr->size; ++i) {
        std::cout << dAr->array[i] << " ";
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }

    std::cout << std::endl;

    std::cout << std::endl << "Thread work is finished" << std::endl;
    cv.notify_all();
}

void sumElement(std::mutex& mtx, std::condition_variable& cv, dataArray* dAr) {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Thread SumElement is started" << std::endl;

    int sum = 0;
    for (int i = 0; i < dAr->size; i++) {
        sum += dAr->array[i];
    }
    std::cout << "Sum of array is " << sum << std::endl;

    cv.notify_all();

    std::cout << std::endl << "Thread SumElement is finished" << std::endl;
}

int main() {
    int size;
    std::cout << "Input size of array: ";
    std::cin >> size;
    std::vector<double> array(size);
    std::cout << "Input elements: ";
    for (int i = 0; i < size; i++) {
        std::cin >> array[i];
    }

    std::cout << "Input k: ";
    int k;
    std::cin >> k;

    std::cout << "The size of array is " << size << std::endl;
    std::cout << "Array: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;

    dataArray d;
    d.size = size;
    d.array = array;

    std::mutex mtx;
    std::mutex mtx2;
    std::condition_variable cv;

    std::thread t1(work, std::ref(mtx), std::ref(cv), &d);
    std::thread t2(sumElement, std::ref(mtx2), std::ref(cv), &d);

    t1.join();
    t2.join();

    return 0;
}
    dataArray* d = new dataArray();
    d->size = size;
    d->array = array;
    d->X = k;

    std::thread work_thread(work, d, time);
    std::thread sum_thread(SumElement, d);

    work_thread.join();
    sum_thread.join();

    delete[] array;

    return 0;
}
