#pragma once

#include <fstream>
#include <string>
#include <iostream>

inline void print(const std::string& str) {
    std::cout << "[LOG] : " << str << std::endl;
}

class Logger {
private:
    std::ofstream file;
private:
    void open(const std::string& filepath);
public:
    explicit Logger(const std::string& filepath);
    void log(const char* buff, size_t len);
    ~Logger();
};
