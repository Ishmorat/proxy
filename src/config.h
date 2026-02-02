#pragma once

#include <string>
#include <iostream>

namespace {
    constexpr int           DEFAULT_PORT     = 5555;
    constexpr std::string   LOCALHOST        = "127.0.0.1";
    constexpr int           DEFAULT_PG_PORT  = 5432;
    constexpr std::string   DEFAULT_LOG_FILE = "log.txt";
}

struct Config {
public:
    int         port;
    std::string pg_addr;
    int         pg_port;
    std::string log_filepath;
public:
    Config(int port_ = DEFAULT_PORT, const std::string& pg_addr_ = LOCALHOST, int pg_port_ = DEFAULT_PG_PORT, const std::string& log_filepath_ = DEFAULT_LOG_FILE);
    static Config create(int args, char* argv[]);
    void print();
};
