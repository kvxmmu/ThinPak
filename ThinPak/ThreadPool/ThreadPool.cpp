//
// Created by kvxmmu on 1/5/21.
//

#include "ThreadPool.hpp"

#include <cstring>

void threadpool_worker(FutexQueue<WorkerQueueItem> *queue,
                       const bool *running) {
    while (*running) {
        auto func = queue->pop();

        if (!func.function) {
            break;
        }

        func.function(func.arg);
        memset(&func, 0, sizeof(WorkerQueueItem));
    }
}


