//
// Created by kvxmmu on 8/28/20.
//

#ifndef THINPAK_EVENT_LOOP_HPP
#define THINPAK_EVENT_LOOP_HPP

#include "abstract.hpp"
#include "packets_queue.hpp"
#include "selector.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


class EventLoop;
typedef _AbstractServer<EventLoop> AbstractServer;

class EventLoop {
public:
    EpollSelector selector;
    AbstractServer *server;
    PacketsQueue queue;

    bool running = false;

    explicit EventLoop(AbstractServer *_server);

    inline void stop() {
        this->running = false;
    }

    void run();
};


sockaddr_in create_addr(unsigned short port, in_addr_t addr);


#endif //THINPAK_EVENT_LOOP_HPP
