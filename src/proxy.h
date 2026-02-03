#pragma once

#include "config.h"
#include "connection.h"
#include "epoll.h"
#include "logger.h"
#include <memory>
#include <unordered_map>

namespace {
    constexpr int   BUFFER_SIZE = 1024;
    constexpr int   MAX_EVENTS  = 1024;
}

class Proxy {
    using ConnPtr = std::shared_ptr<Connection>;
private:
    Config      cfg;
    Socket      listener;
    Epoll       epoll;
    Logger      logger;
    std::unordered_map<int, ConnPtr> conns;                     // Stores pointers to connections in two instances: one for the client and the other for the server 
private:
    void init();
    void handle_new_client();
    void handle_connection(const epoll_event& ev);
    void close_connection(ConnPtr conn);
    void parse_and_log(ConnPtr conn, char* buff, size_t len) noexcept;
public:
    Proxy(const Config& cfg_);
    void run();
    ~Proxy() {}
};
