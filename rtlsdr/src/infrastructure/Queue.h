//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//

#ifndef CONCURRENT_QUEUE_
#define CONCURRENT_QUEUE_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>

namespace backscatter {
namespace infrastructure {

template<typename T>
class Queue {
public:

	T pop() {
		std::unique_lock<std::mutex> mlock(mutex_);
		if (queue_.empty()) {
			cond_.wait(mlock);
			if (queue_.empty()) {
				//Happens when purging
				return NULL;
			}
		}
		auto val = queue_.front();
		queue_.pop();
		return val;
	}

	void pop(T& item) {
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty()) {
			cond_.wait(mlock);
			if(purged){
				return;
			}
		}
		if (queue_.empty())
			return;
		item = queue_.front();
		queue_.pop();
	}

	void push(const T& item) {
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.push(item);
		mlock.unlock();
		cond_.notify_one();
	}

	bool empty() {
		return queue_.empty();
	}

	bool isPurged() {
		return purged;
	}

	void purge() {
		{
			std::unique_lock<std::mutex> mlock(mutex_);
			while (!empty()) {
				queue_.pop();
			}
		}
		purged = true;
		cond_.notify_all();
	}

	int size() {
		return queue_.size();
	}

	Queue() = default;
	Queue(const Queue&) = delete;            // disable copying
	Queue& operator=(const Queue&) = delete;            // disable assignment

private:
	bool purged = false;
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
};
}
}
#endif
