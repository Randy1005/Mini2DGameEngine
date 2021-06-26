#pragma once
#include <functional>
#include <deque>
#include <mutex>
#include <thread>
#include <atomic>
#include <algorithm>
#include <condition_variable>
#include <vector>

namespace JobSystem {
	void Initialize();

	// add a job into job pool to execute asynchronously, any idle thread would execute this job
	void RunJob(const std::function<void()>& job);

	// check if any threads are working currently
	bool IsBusy();

	// wait until all threads become idle (drain the pending jobs)
	void Wait();

	size_t JobPoolSize();

	void Terminate();



	template<class T, class Allocator = std::allocator<T>>
	class ThreadSafeDeque {
	public:
		inline bool pop_front(T& item) {
			{
				// automatically unlocks when this goes out of scope
				std::unique_lock<std::mutex> lock{ dequeMutex };
				
				// thread waits if deque is empty
				// conditionWake.wait(lock, [this] { return !deque.empty(); });
				if (deque.empty()) return false;

				item = deque.front();
				deque.pop_front();
			}

			return true;
		}

		inline bool push_back(const T& item) {
			bool result = false;
			{
				// automatically unlocks when this goes out of scope
				std::unique_lock<std::mutex> lock{ dequeMutex };

				deque.push_back(item);
				result = true;
			}
			conditionWake.notify_one();

			return result;
		}

		inline size_t size() const {
			return deque.size();
		}


	private:
		std::deque<T, Allocator> deque;
		std::mutex dequeMutex;
		std::condition_variable conditionWake;
	};

}
