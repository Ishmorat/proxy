#pragma once

#include "socket.h"
#include <cstdint>

namespace {
    constexpr int   HEADER_SIZE = 5;
}

struct Connection {
public:
    Socket  client;
    Socket  server;
    char    header_type;
    int     header_len;                     // Кол-во байт записанных в заголовок. Нужно, если вдруг заголовок будет разбит на несколько recv 
    // char    header[HEADER_SIZE];            // Заголовок : Q + len 
public:
    Connection(Socket&& client_, Socket&& server_) noexcept
        : client       (std::move(client_))
        , server       (std::move(server_))
        , header_type  { '\0'             }
        , header_len   { 0                }
    {}

    bool is_query() const noexcept {
        return header_type == 'Q';
    }

    bool is_header_complete() const noexcept {
        return header_len == HEADER_SIZE;
    }

    void clear_header() noexcept {
        header_type = '\0';
        header_len = 0;
    }

    // char get_header_type() const noexcept {
    //     return header[0];
    // }

    // uint32_t get_remaining_len() const noexcept {       // Кол-во байт, которые еще нужно записать для запроса 
    //     uint32_t len = chars_to_uint(header + 1);
    //     return len - HEADER_SIZE;
    // }

    // void set_remaining_len(uint32_t len) noexcept {
    //     if (len == 0) {
    //         header_len = 0;
    //         return;
    //     }
    //     uint_to_chars(len + HEADER_SIZE, header + 1);
    // }

    void add_in_header() noexcept {
        // header[header_len] = sym;
        ++header_len;
    }
};
