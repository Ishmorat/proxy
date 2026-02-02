#include "file_desc.h"

FileDesc::FileDesc(int fd_) noexcept
    : fd{ fd_ }
{}

void FileDesc::close() noexcept {
    if (fd >= 0) {
        ::close(fd);
        fd = INVALID_FD;
    }
}

void FileDesc::swap(FileDesc& other) noexcept {
    std::swap(fd, other.fd);
}

bool FileDesc::is_valid() const noexcept {
    return fd >= 0;
}

int FileDesc::get_fd() const noexcept {
    return fd;
}

void FileDesc::check_validity() const {
    if (!is_valid()) throw std::runtime_error("Invalid file descriptor");
}

FileDesc::~FileDesc() {
    close();
}
