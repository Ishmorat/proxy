#pragma once

#include "config.h"
#include "connection.h"
#include "epoll.h"
#include "logger.h"
#include <memory>
#include <unordered_map>

#include <atomic>
#include <csignal>

namespace {
    constexpr int   BUFFER_SIZE = 1024;
    constexpr int   MAX_EVENTS  = 1024;

    std::atomic_flag stop = ATOMIC_FLAG_INIT;
    void signal_handler(int sig) {
        stop.test_and_set(std::memory_order_relaxed);
    }

    bool should_stop() {
        return stop.test(std::memory_order_relaxed);
    }
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
    void setup_signals();
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
