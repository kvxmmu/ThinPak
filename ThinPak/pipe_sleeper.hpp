//
// Created by kvxmmu on 8/23/20.
//

#ifndef THINPAK_PIPE_SLEEPER_HPP
#define THINPAK_PIPE_SLEEPER_HPP

#include <sys/types.h>
#include <unistd.h>


class PipeSleeper {
public:
    int write_fd;
    int read_fd;

    PipeSleeper() {
        int fds[2];

        pipe(fds);

        this->write_fd = fds[1];
        this->read_fd = fds[0];
    }

    void wake() const {
        write(this->write_fd, "\x00", 1);
    }

    void sleep() const {
        char b;

        read(this->read_fd, &b, 1);
    }

    void destruct() const {
        close(this->write_fd);
        close(this->read_fd);
    }

    ~PipeSleeper() {
        this->destruct();
    }
};


#endif //THINPAK_PIPE_SLEEPER_HPP
