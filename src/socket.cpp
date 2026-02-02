#include "socket.h"

Socket::Socket(int fd_) noexcept
    : FileDesc(fd_)
{}

Socket Socket::create(int domain, int type, int protocol) noexcept {
    int res = ::socket(domain, type, protocol);
    // if (res < 0) throw std::runtime_error("Socket error");
    return Socket(res);
}

Socket Socket::connect(const char* addr, int port) noexcept {
    Socket sock = create();
    if (!sock.is_valid()) return Socket();
    sock.set_nonblock();

    in_addr ip;
	int res = inet_pton(AF_INET, addr, &ip);
    if (res <= 0) return Socket();
    
    sockaddr_in info;
    info.sin_family = AF_INET;
    info.sin_port = htons(port);
    info.sin_addr = ip;

    res = ::connect(sock.fd, reinterpret_cast<sockaddr*>(&info), sizeof(info));
    // if (res < 0) throw std::runtime_error("Connect error");
    if (res < 0 && errno != EINPROGRESS) return Socket();
    return sock;
}

Socket::Socket(Socket&& other) noexcept
    : Socket()
{
    swap(other);
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        Socket tmp = std::move(other);
        swap(tmp);
    }
    return *this;
}

void Socket::set_reuseaddr() noexcept {
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Socket::set_nonblock() noexcept {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void Socket::bind(const char* addr, int port) {
    in_addr ip;
	int res = inet_pton(AF_INET, addr, &ip);
	if (res <= 0) throw std::runtime_error("Inet_pton error");

	sockaddr_in info;
	info.sin_family = AF_INET;
	info.sin_port = htons(port);
	info.sin_addr = ip;

	res = ::bind(fd, reinterpret_cast<sockaddr*>(&info), sizeof(info));
	if (res == -1) throw std::runtime_error("Bind error");
}

void Socket::listen(int count) {
    int res = ::listen(fd, count);
    if (res < 0) throw std::runtime_error("Listen error");
}

Socket Socket::accept() noexcept {
	int res = ::accept(fd, nullptr, nullptr);
    // if (res < 0) throw std::runtime_error("Accept error");
    return Socket(res);
}

size_t Socket::send(const char* buff, size_t len) noexcept {
    return ::send(fd, buff, len, 0);
}

size_t Socket::send_nosignal(const char* buff, size_t len) noexcept {
    return ::send(fd, buff, len, MSG_NOSIGNAL);
}

size_t Socket::recv(char* buff, size_t len) noexcept {
    return ::recv(fd, buff, len, 0);
}

Socket::~Socket() {}
