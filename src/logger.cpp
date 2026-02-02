#include "logger.h"

void Logger::open(const std::string& filepath) {
    file.open(filepath, std::ios::app);
    if (!file) throw std::runtime_error("File error");
}

Logger::Logger(const std::string& filepath) {
    open(filepath);
}

void Logger::log(const char* buff, size_t len) {
    file.write(buff, len);
    file.flush();
}

Logger::~Logger() {
    if (file.is_open()) file.close();
}
