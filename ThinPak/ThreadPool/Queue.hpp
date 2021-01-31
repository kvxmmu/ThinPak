//
// Created by kvxmmu on 1/1/21.
//

#ifndef FANPOOL_QUEUE_HPP
#define FANPOOL_QUEUE_HPP

#include "futex_sleep.h"

#include <mutex>
#include <memory>
#include <deque>
#include <condition_variable>

#include <list>

template <typename T>
class FutexQueue {
public:
    std::deque<T> deque;
    std::shared_ptr<std::mutex> mutex;

    FutexQueue() {
        this->mutex = std::make_shared<std::mutex>();
    }

    void push(const T &value) {
        std::unique_lock<std::mutex> lock(*this->mutex.get());

        this->deque.push_back(value);
        this->cv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(*this->mutex.get());

        while (this->deque.empty()) {
            this->cv.wait(lock);
        }

        T value = this->deque.front();
        this->deque.pop_front();

        return value;
    }

private:
    std::condition_variable cv;
};


#endif //FANPOOL_QUEUE_HPP
