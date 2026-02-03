#pragma once

#include <unistd.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>

namespace {
    constexpr int INVALID_FD = -1;
}

/*
* RAII wraper for file descriptor 
*/
class FileDesc {
protected:
    int fd;
protected:
    explicit FileDesc(int fd_ = INVALID_FD) noexcept;
    void close() noexcept;
public:
    void swap(FileDesc& other) noexcept;
    bool is_valid() const noexcept;
    int get_fd() const noexcept;
    void check_validity() const;
    ~FileDesc();
};
