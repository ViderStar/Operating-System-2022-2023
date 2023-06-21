#include <iostream>
#include <thread>
#include <vector>
#include <shared_mutex>
#include <chrono>

#define READERS_NUM 20
#define WRITERS_NUM 5
#define REPEATS_NUM 5
#define READER_WAIT_TIME 20		//in milliseconds
#define WRITER_WAIT_TIME 40		//in milliseconds


class shared_data {
	std::shared_mutex sm;
	std::mutex cout_mutex;
	int n = 5;
public:

	int read_data() {
		int result;
		//acquiring the shared lock
		if (!sm.try_lock_shared())
		{
			{
				std::lock_guard<std::mutex> lg(cout_mutex);
				std::cout << "Thread #" << std::this_thread::get_id()
					<< " (reader) is waiting\n";
			}
			sm.lock_shared();
		}

		//"reading"
		result = n;
		{
			std::lock_guard<std::mutex> lg(cout_mutex);
			std::cout << "Thread #" << std::this_thread::get_id()
				<< " is reading " << n << "\n";
		}

		std::this_thread::sleep_for(
			std::chrono::milliseconds(READER_WAIT_TIME));

		//releasing the lock
		sm.unlock_shared();

		return result;
	}

	void write_data(int new_data) {
		//acquiring the lock
		if (!sm.try_lock())
		{
			{
				std::lock_guard<std::mutex> lg(cout_mutex);
				std::cout << "Thread #" << std::this_thread::get_id()
					<< " (writer) is waiting\n";
			}
			sm.lock();
		}

		//"writing"
		{
			std::lock_guard<std::mutex> lg(cout_mutex);
			std::cout << "Thread #" << std::this_thread::get_id()
				<< " is writing " << new_data << "\n";
		}
		n = new_data;
		std::this_thread::sleep_for(
			std::chrono::milliseconds(WRITER_WAIT_TIME));

		//releasing the lock
		sm.unlock();
	}
};

void writerMain(shared_data* sd, int n) {
	sd->write_data(n);
}

void readerMain(shared_data* sd) {
	sd->read_data();
}

int main()
{
	for (int rep = 0; rep < REPEATS_NUM; rep++) {
		std::cout << "\nRepeat #" << rep + 1 << "\n";
		//shared data initialization
		shared_data sd;

		////threads initialization
		//writers initialization
		std::vector<std::thread> writers;
		for (int i = 0; i < WRITERS_NUM; i++)
			writers.push_back(std::thread(writerMain, &sd, i));

		//readers initialization
		std::vector<std::thread> readers;
		for (int i = 0; i < READERS_NUM; i++)
			readers.push_back(std::thread(readerMain, &sd));


		////waiting for threads to finish
		for (auto& a : readers)a.join();
		for (auto& a : writers)a.join();
	}
}