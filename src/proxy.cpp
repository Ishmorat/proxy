#include "proxy.h"

void Proxy::init() {
    listener.check_validity();
    listener.set_reuseaddr();
    listener.bind(LOCALHOST.c_str(), cfg.port);
    listener.listen();
    listener.set_nonblock();

    epoll.check_validity();
    bool success = epoll.add(listener, EPOLLIN | EPOLLET);
    if (!success) throw std::runtime_error("Epoll add listener error");
}

void Proxy::handle_new_client() {
    while (true) {
        Socket client = listener.accept();
        if (!client.is_valid()) return;
        Socket server = Socket::connect(cfg.pg_addr.c_str(), cfg.pg_port);
        if (!server.is_valid()) return;

        client.set_nonblock();                                              // На всякий случай 
        server.set_nonblock();
        
        bool success_c = epoll.add(client, EPOLLIN | EPOLLET | EPOLLHUP);
        if (!success_c) return;
        bool success_s = epoll.add(server, EPOLLIN | EPOLLET | EPOLLHUP);
        if (!success_s) {
            epoll.del(client);
            return;
        }

        ConnPtr conn = std::make_shared<Connection>(std::move(client), std::move(server));
        conns[conn->client.get_fd()] = conn;
        conns[conn->server.get_fd()] = conn;
    }
}

void Proxy::handle_connection(const epoll_event& ev) {
    int fd = ev.data.fd;
    auto it = conns.find(fd);
    if (it == conns.end()) return;

    ConnPtr conn = it->second;
    if (ev.events & EPOLLHUP) {
        close_connection(conn);
        return;
    }
    
    bool is_client = (fd == conn->client.get_fd());
    Socket& src = is_client ? conn->client : conn->server;
    Socket& dst = is_client ? conn->server : conn->client;

    char buff[BUFFER_SIZE];
    while (true) {
        int recv_n = src.recv(buff, BUFFER_SIZE);
        if (recv_n == -1 && errno != EAGAIN || recv_n == 0) close_connection(conn);
        if (recv_n <= 0) break;

        size_t sent = 0;
        while (sent < recv_n) {
            size_t s = dst.send_nosignal(buff + sent, recv_n - sent);
            if (s < 0) break;
            sent += s;
        }

        if (is_client) parse_and_log(conn, buff, recv_n);
    }
}

void Proxy::close_connection(ConnPtr conn) {
    epoll.del(conn->client);
    epoll.del(conn->server);

    conns.erase(conn->client.get_fd());
    conns.erase(conn->server.get_fd());
}

void Proxy::parse_and_log(ConnPtr conn, char* buff, size_t len) noexcept {
    char* it = buff;
    char* end = buff + len;
    while (it < end) {
        if (conn->is_query()) {
            for ( ; it < end && !(conn->is_header_complete()); ++it) {
                conn->add_in_header();
            }

            char* begin = it;
            for ( ; it < end; ++it) {
                char& sym = *it;
                if      (sym == '\n') sym = ' ';
                else if (sym == '\0') { 
                    sym = '\n';
                    conn->clear_header();
                }
            }
            logger.log(begin, it - begin);
        }
        else if (conn->header_type == '\0') {
            conn->header_type = *it;
            conn->add_in_header();
            ++it;
        }
        else {
            for ( ; it < end; ++it) {
                if (*it == '\0') conn->clear_header();
            }
        }
    }
}

Proxy::Proxy(const Config& cfg_)
    : cfg     { cfg_                        }
    , listener{ Socket::create()            }
    , epoll   { Epoll::create(MAX_EVENTS)   }
    , logger  { Logger(cfg_.log_filepath)   }
{
    init();
}

void Proxy::run() {
    std::cout << "Running...\n";
    while (true) {
        int nfds = epoll.wait();
        for (size_t i = 0; i < nfds; ++i) {
            int fd = epoll[i].data.fd;
            if (fd == listener.get_fd()) handle_new_client();
            else                         handle_connection(epoll[i]);
        }
    }
}
