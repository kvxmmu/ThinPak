//
// Created by kvxmmu on 8/24/20.
//

#include "packets_queue.hpp"

/// Helpers


int write_bytes(int fd, void *buffer, size_t length) {
    int result = write(fd, buffer, length);

#if (defined(THROW_IO_ERRORS) || defined(LOG_IO_ERRORS))
    if (result != 0) {
        #ifdef THROW_IO_ERRORS
            throw std::runtime_error(strerror(errno));
        #elif defined(LOG_IO_ERRORS)
            perror("write_bytes()");
        #endif
    }
#endif

    return result;
}

int read_bytes(int fd, void *buffer, size_t length) {
    int result = read(fd, buffer, length);

#if (defined(THROW_IO_ERRORS) || defined(LOG_IO_ERRORS))
    if (result != 0) {
        #ifdef THROW_IO_ERRORS
            throw std::runtime_error(strerror(errno));
        #elif defined(LOG_IO_ERRORS)
            perror("read_bytes()");
        #endif
    }
#endif

    return result;
}


/// PacketsQueue

PacketsQueue::PacketsQueue(EpollSelector &_selector) : selector(_selector) {

}

bool PacketsQueue::create_if_not_exists(int fd) {
    if (this->queues.find(fd) == this->queues.end()) {
        this->queues[fd] = NetPacketsQueue();

        return true;
    }

    return false;
}

bool PacketsQueue::remove_if_empty(int fd) {
    if (this->queues.find(fd) == this->queues.end()) {
        return false;
    }

    if (this->queues.at(fd).empty()) {
        this->queues.erase(fd);

        return true;
    }

    return false;
}

void PacketsQueue::check_fd(int fd) {
    if (this->queues.find(fd) == this->queues.end()) {
        return;
    }

    NetPacketsQueue &queue = this->queues.at(fd);

    if (queue.empty()) {
        this->remove_queue(fd);

        return;
    }

    QueueItem &front = queue.front();

    front.sent += write_bytes(fd, front.buffer+front.sent, front.length-front.sent);

    if (front.is_sent()) {
        this->pop(fd);
    }
}

QueueItem PacketsQueue::create_item(char *buffer, size_t size) {
    QueueItem item(buffer, size);

    return item;
}

void PacketsQueue::push(int fd, const QueueItem &item) {
    this->create_if_not_exists(fd);

    if (!this->selector.has_flag(fd, EPOLLOUT)) {
        this->selector.add(fd, EPOLLOUT);
    }

    this->queues.at(fd).push_back(item);
}

void PacketsQueue::pop(int fd) {
    if (this->queues.find(fd) == this->queues.end()) {
        return;
    }

    NetPacketsQueue &queue = this->queues.at(fd);

    if (queue.empty()) {
        this->queues.erase(fd);

        return;
    }

    queue.pop_front();
    this->selector.remove(fd, EPOLLOUT);

    this->remove_if_empty(fd);
}

void PacketsQueue::remove_queue(int fd) {
    if (this->queues.find(fd) == this->queues.end()) {
        return;
    }

    this->queues.at(fd).clear();
    this->queues.erase(fd);
}
