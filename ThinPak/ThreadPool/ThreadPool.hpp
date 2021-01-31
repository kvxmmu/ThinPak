//
// Created by kvxmmu on 1/5/21.
//

#ifndef FANPOOL_THREADPOOL_HPP
#define FANPOOL_THREADPOOL_HPP

#include "Queue.hpp"

#include <thread>
#include <vector>

#include <functional>

typedef std::shared_ptr<std::thread> shared_thread_t;

struct WorkerQueueItem {
    std::function<void(void *)> function;
    void *arg;
};

void threadpool_worker(FutexQueue<WorkerQueueItem> *queue,
        const bool *running);

class ThreadPool {
public:
    std::vector<shared_thread_t> threads;

    size_t workers_count;
    bool running = false;

    FutexQueue<WorkerQueueItem> queue;

    explicit ThreadPool(size_t workers_count_) : workers_count(workers_count_) {
        this->init_workers();
    }

    void init_workers() {
        this->running = true;

        for (size_t thread_id = 0; thread_id < this->workers_count; thread_id++) {
            this->threads.push_back(std::make_shared<std::thread>(
                    threadpool_worker, &this->queue, &this->running
                    ));
        }
    }

    void stop_work() {
        for (size_t pos = 0; pos < this->workers_count; pos++) {
            this->enqueue(nullptr, nullptr);
        }
    }

    void enqueue(const std::function<void(void *)> &callback, void *arg) {
        this->queue.push(WorkerQueueItem{
            .function = callback,
            .arg = arg
        });
    }

    ~ThreadPool() {
        this->running = false;
        this->stop_work();

        for (auto &thread : this->threads) {
            thread->join();
        }
    }
};

#endif //FANPOOL_THREADPOOL_HPP
