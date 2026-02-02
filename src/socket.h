#pragma once

#include "file_desc.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

class Socket : public FileDesc {
private:
    explicit Socket(int fd_ = INVALID_FD) noexcept;
public:
    static Socket create(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0) noexcept;
    static Socket connect(const char* addr, int port) noexcept;
    
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    void set_reuseaddr() noexcept;
    void set_nonblock() noexcept;
    void bind(const char* addr, int port);
    void listen(int count = SOMAXCONN);
    Socket accept() noexcept;
    
    size_t send(const char* buff, size_t len) noexcept;
    size_t send_nosignal(const char* buff, size_t len) noexcept;
    size_t recv(char* buff, size_t len) noexcept;
    
    ~Socket();
};
