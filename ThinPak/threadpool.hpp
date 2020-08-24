//
// Created by kvxmmu on 8/23/20.
//

#ifndef THINPAK_THREADPOOL_HPP
#define THINPAK_THREADPOOL_HPP

#include <thread>
#include <deque>
#include <functional>
#include <vector>
#include <mutex>

#include "pipe_sleeper.hpp"


typedef struct {
    std::function<void(void *)> function;
    void *arg;
} FunctionQueueItem;

class ThreadWorker;
void thread_worker(ThreadWorker *worker);

class ThreadWorker {
public:
    std::deque<FunctionQueueItem> queue;
    std::thread *thread = nullptr;
    std::mutex *mtx = nullptr;

    PipeSleeper sleeper{};

    bool running = true;

    ThreadWorker() {
        this->mtx = new std::mutex;
        this->thread = new std::thread(thread_worker, this);
    }

    void push(const std::function<void(void *)> &f, void *arg) {
        this->mtx->lock();
        this->queue.push_back({f, arg});

        this->sleeper.wake();
    }

    FunctionQueueItem pop() {
        this->sleeper.sleep();

        if (this->queue.empty()) {
            return {nullptr, nullptr};
        }

        FunctionQueueItem func = this->queue.front();
        this->queue.pop_front();

        return func;
    }

    void join() {
        this->running = false;
        this->sleeper.wake();

        this->thread->join();
    }

    ~ThreadWorker() {
        this->join();

        delete this->mtx;
        delete this->thread;

        this->mtx = nullptr;
    }
};


void thread_worker(ThreadWorker *worker) {
    while (true) {
        FunctionQueueItem item = worker->pop();

        if (item.function == nullptr) {
            break;
        }

        item.function(item.arg);
    }
}


class ThreadPool {
public:
    size_t workers_count;

    std::vector<ThreadWorker> workers;
    size_t last_id = 0;

    explicit ThreadPool(size_t _workers_count) : workers_count(_workers_count) {
        this->init_threads();
    }

    void init_threads() {
        for (size_t id = 0; id < this->workers_count; id++) {
            this->workers.emplace_back();
        }
    }

    size_t get_id() {
        size_t last = this->last_id++;

        if (last >= this->workers.size()) {
            this->last_id = 0;

            return 0;
        }

        return last;
    }

    void submit(const std::function<void(void *)> &function, void *arg = nullptr) {
        auto id = this->get_id();

        ThreadWorker &worker = this->workers.at(id);
        worker.push(function, arg);
    }

    ~ThreadPool() {
        this->workers.clear();
        this->workers_count = 0;
    }
};


#endif //THINPAK_THREADPOOL_HPP
