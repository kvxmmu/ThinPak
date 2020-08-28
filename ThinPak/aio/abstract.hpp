//
// Created by kvxmmu on 8/28/20.
//

#ifndef THINPAK_ABSTRACT_HPP
#define THINPAK_ABSTRACT_HPP

template <typename EvLoop>
class _AbstractServer {
public:
    EvLoop *event_loop;

    int sockfd = 0;

    virtual void init(EvLoop *_event_loop) {
        this->event_loop = _event_loop;
    }

    virtual void on_connect() = 0;
    virtual void on_received(int fd) = 0;
    virtual void on_disconnect(int fd) = 0;
};

#endif //THINPAK_ABSTRACT_HPP
