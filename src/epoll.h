#pragma once

#include "socket.h"
#include <vector>
#include <sys/epoll.h>

class Epoll : public FileDesc {
private:
    std::vector<epoll_event> events;
private:
    explicit Epoll(int fd, size_t max_events);
public:
    static Epoll create(size_t max_events);
    bool add(const Socket& sock, uint32_t events_type) noexcept;
    void del(const Socket& sock) noexcept;
    int wait(int timeout = -1) noexcept;
    const epoll_event& operator[](size_t idx) const;
};
