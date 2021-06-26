#include "JobSystem.h"
#include "Console/ConsolePrint.h"

namespace JobSystem {
	uint64_t numThreads = 0;
	ThreadSafeDeque<std::function<void()>> jobPool;

	bool runJobSystem = true;


	std::condition_variable conditionWakeWorker;
	std::mutex mutexWorker;
	// track state of main thread
	uint64_t currentLabel = 0;
	// track state of worker threads 
	std::atomic<uint64_t> finishedLabel;

	inline void poll() {
		conditionWakeWorker.notify_one();
		std::this_thread::yield();
	}

	void Initialize() {


		// initialize worker execution state to 0
		finishedLabel.store(0);

		// get the number of cores
		auto numCores = std::thread::hardware_concurrency();

		numThreads = std::max(1u, numCores);

		// create worker threads
		for (uint64_t threadID = 0; threadID < numThreads; threadID++) {
			std::thread worker([threadID] {
				std::function<void()> job;

				while (runJobSystem) {
					if (jobPool.pop_front(job)) {
						// retrieved a job from job pool, execute it
						DEBUG_PRINT("worker %d got a job, will execute it.", threadID);
						job();

						// update worker label state
						finishedLabel.fetch_add(1);

					}
					else {
						DEBUG_PRINT("worker %d going to sleep.", threadID);
						// if there are no jobs to fetch, put thread to sleep

						std::unique_lock<std::mutex> lock{ mutexWorker };
						conditionWakeWorker.wait(lock, [] { return !(jobPool.size() == 0) || !runJobSystem; });
					}
				}

			});

			// leave the thread running
			worker.detach();
		}
	}


	void RunJob(const std::function<void()>& job) {
		// main thread label state updated
		currentLabel++;

		// push a new job until successfully pushed
		while (!jobPool.push_back(job)) {
			JobSystem::poll();
		}

		conditionWakeWorker.notify_one();
	}

	bool IsBusy() {
		return finishedLabel.load() < currentLabel;
	}

	void Wait() {
		while (IsBusy()) {
			JobSystem::poll();
		}
	}

	size_t JobPoolSize() {
		return jobPool.size();
	}

	void Terminate() {
		runJobSystem = false;
		conditionWakeWorker.notify_all();
	}

}