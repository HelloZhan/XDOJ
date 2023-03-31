#include "buffer.h"

Buffer::Buffer(int initBuffSize) : buffer_(initBuffSize), readPos_(0), writePos_(0) {}

size_t Buffer::ReadableBytes() const { // 返回缓冲区能够读的数据的大小
    return writePos_ - readPos_;
}
size_t Buffer::WritableBytes() const { // 返回缓冲区能够写的数据的大小
    return buffer_.size() - writePos_;
}

size_t Buffer::PrependableBytes() const {  // 返回缓冲区读的位置的大小
    return readPos_;
}

const char* Buffer::Peek() const { // 返回缓冲区读的位置的指针
    return BeginPtr_() + readPos_;
}

void Buffer::Retrieve(size_t len) { // 更新读的位置（通过size_t)
    assert(len <= ReadableBytes());
    readPos_ += len;
}

void Buffer::RetrieveUntil(const char* end) { // 更新读的位置（通过位置指针）
    assert(Peek() <= end );
    Retrieve(end - Peek());
}

void Buffer::RetrieveAll() { // 读取全部进行初始化
    bzero(&buffer_[0], buffer_.size());
    readPos_ = 0;
    writePos_ = 0;
}

std::string Buffer::RetrieveAllToStr() { // 读取缓冲区所有数据，并将其初始化
    std::string str(Peek(), ReadableBytes());
    RetrieveAll();
    return str;
}

const char* Buffer::BeginWriteConst() const { // 返回缓冲区当前写指针
    return BeginPtr_() + writePos_;
}

char* Buffer::BeginWrite() { // 返回缓冲区当前写指针
    return BeginPtr_() + writePos_;
}

void Buffer::HasWritten(size_t len) { // 更新写指针位置（通过size_t）
    writePos_ += len;
}

void Buffer::Append(const std::string& str) { // 添加数据（通过string）重载函数
    Append(str.data(), str.length());
}

void Buffer::Append(const void* data, size_t len) { // 添加数据 重载函数
    assert(data);
    Append(static_cast<const char*>(data), len);
}

void Buffer::Append(const char* str, size_t len) { // 添加数据 重载函数
    assert(str);
    // 进行缓冲区空间调整（移动，扩容）
    EnsureWriteable(len);
    // 将其添加到缓冲区读指针后面
    std::copy(str, str + len, BeginWrite());
    // 进行写缓冲更新
    HasWritten(len);
}

void Buffer::Append(const Buffer& buff) { // 添加数据 重载函数
    Append(buff.Peek(), buff.ReadableBytes());
}

void Buffer::EnsureWriteable(size_t len) { // 看能不能写这么多数据，如果不想进行空间调整
    if(WritableBytes() < len) {
        MakeSpace_(len);
    }
    assert(WritableBytes() >= len);
}

ssize_t Buffer::ReadFd(int fd, int* saveErrno) { // 分散读
    char buff[65535];
    struct iovec iov[2];
    const size_t writable = WritableBytes();
    /* 分散读， 保证数据全部读完 */
    iov[0].iov_base = BeginPtr_() + writePos_;
    iov[0].iov_len = writable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);
    // readv，计算机函数。用来将读入的数据按上述同样顺序散布读到缓冲区中。readv总是先填满一个缓冲区，然后再填写下一个。readv返回读到的总字节数。如果遇到文件结尾，已无数据可读，则返回0。
    // 分散读入
    const ssize_t len = readv(fd, iov, 2);
    if(len < 0) {
        *saveErrno = errno;
    }
    else if(static_cast<size_t>(len) <= writable) {
        writePos_ += len;
    }
    else {
        writePos_ = buffer_.size();
        Append(buff, len - writable);
    }
    return len;
}

ssize_t Buffer::WriteFd(int fd, int* saveErrno) { // 一并写入
    size_t readSize = ReadableBytes();
    ssize_t len = write(fd, Peek(), readSize);
    if(len < 0) {
        *saveErrno = errno;
        return len;
    }
    readPos_ += len;
    return len;
}

char* Buffer::BeginPtr_() { // 返回缓冲区的首指针
    return &*buffer_.begin();
}

const char* Buffer::BeginPtr_() const { // 返回缓冲区的首指针
    return &*buffer_.begin();
}

void Buffer::MakeSpace_(size_t len) { // 整理缓冲区空间
    // 第一种，当缓冲区剩余空间不够装入所有读入数据，扩容
    if(WritableBytes() + PrependableBytes() < len) {
        buffer_.resize(writePos_ + len + 1);
    }
    // 第二种，当缓冲区剩余空间够装入所有读入数据，将空间数据前移
    else {
        size_t readable = ReadableBytes();
        std::copy(BeginPtr_() + readPos_, BeginPtr_() + writePos_, BeginPtr_());
        readPos_ = 0;
        writePos_ = readPos_ + readable;
        assert(readable == ReadableBytes());
    }
}
