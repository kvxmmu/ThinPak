//
// Created by kvxmmu on 8/24/20.
//

#ifndef THINPAK_PACKETSPOOL_HPP
#define THINPAK_PACKETSPOOL_HPP


#include <unordered_map>
#include <deque>

#include <cstring>
#include <unistd.h>

#include "selector.hpp"

#ifdef THROW_IO_ERRORS
# include <stdexcept>
#endif

int write_bytes(int fd, void *buffer, size_t length);
int read_bytes(int fd, void *buffer, size_t length);

class QueueItem {
public:
    char *buffer = nullptr;
    size_t sent = 0;
    size_t length = 0;

    QueueItem(char *_buffer,
            size_t _length) : \
                              length(_length) {
        this->buffer = new char[_length];
        memcpy(this->buffer, _buffer, length);
    }

    inline void add_sent(size_t value) {
        this->sent += value;
    }

    [[nodiscard]] inline bool is_sent() const {
        return this->sent >= this->length;
    }

    ~QueueItem() {
        delete[] this->buffer;
    }
};


typedef std::deque<QueueItem> NetPacketsQueue;

class PacketsQueue {
public:
    std::unordered_map<int, NetPacketsQueue> queues;
    EpollSelector &selector;

    explicit PacketsQueue(EpollSelector &_selector);

    bool create_if_not_exists(int fd);
    bool remove_if_empty(int fd);

    static QueueItem create_item(char *buffer, size_t size);

    void push(int fd, const QueueItem &item);
    void pop(int fd);
    void remove_queue(int fd);

    void check_fd(int fd);
};


#endif //THINPAK_PACKETSPOOL_HPP
