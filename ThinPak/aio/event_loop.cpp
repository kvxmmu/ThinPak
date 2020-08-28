//
// Created by kvxmmu on 8/28/20.
//

#include "event_loop.hpp"

EventLoop::EventLoop(AbstractServer *_server) : server(_server), queue(this->selector) {

}

void EventLoop::run() {
    this->running = true;

    this->server->init(this);
    this->selector.add(this->server->sockfd, EPOLLIN);

    char useless;

    while (this->running) {
        size_t nfds = this->selector.wait();

        for (size_t pos = 0; pos < nfds; pos++) {
            epoll_event &event = this->selector.events_buffer[pos];

            if (event.events & EPOLLOUT) {
                this->queue.check_fd(event.data.fd);
            } else {
                if (this->server->sockfd == event.data.fd) {
                    this->server->on_connect();

                    continue;
                }

                int bytes_received = recv(event.data.fd, &useless, sizeof(char), MSG_PEEK);

                if (bytes_received == 0) {
                    this->server->on_disconnect(event.data.fd);

                    continue;
                }

                this->server->on_received(event.data.fd);
            }
        }
    }
}

sockaddr_in create_addr(unsigned short port, in_addr_t addr) {
    sockaddr_in caddr{};
    caddr.sin_port = htons(port);
    caddr.sin_family = AF_INET;
    caddr.sin_addr.s_addr = addr;

    return caddr;
}
