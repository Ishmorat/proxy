#include "epoll.h"

Epoll::Epoll(int fd, size_t max_events)
    : FileDesc(fd)
    , events(max_events)
{}

Epoll Epoll::create(size_t max_events) {
    int epfd = epoll_create1(0);
    // if (epfd < 0) throw std::runtime_error("Epoll error");
    return Epoll(epfd, max_events);
}

bool Epoll::add(const Socket& sock, uint32_t events_type) noexcept {
    epoll_event ev;
    ev.events = events_type;
    ev.data.fd = sock.get_fd();
    int res = epoll_ctl(fd, EPOLL_CTL_ADD, sock.get_fd(), &ev);
    // if (res < 0) throw std::runtime_error("Epoll add error");
    return res >= 0;
}

void Epoll::del(const Socket& sock) noexcept {
    int res = epoll_ctl(fd, EPOLL_CTL_DEL, sock.get_fd(), nullptr);
    // if (res < 0) throw std::runtime_error("Epoll del error");
}

int Epoll::wait(int timeout) noexcept {
    int nfds = epoll_wait(fd, events.data(), events.size(), timeout);
    // if (nfds < 0 && errno != EINTR) throw std::runtime_error("Epoll wait error");
    return nfds;
}

const epoll_event& Epoll::operator[](size_t idx) const {
    return events[idx];
}
