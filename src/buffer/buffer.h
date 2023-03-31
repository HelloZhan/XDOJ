#ifndef BUFFER_H
#define BUFFER_H
#include <cstring>   //perror
#include <iostream>
#include <unistd.h>  // write
#include <sys/uio.h> //readv
#include <vector> //readv
#include <atomic>
#include <assert.h>
class Buffer {
public:
    Buffer(int initBuffSize = 1024);
    ~Buffer() = default;

    size_t WritableBytes() const;
    size_t ReadableBytes() const ;
    size_t PrependableBytes() const;

    const char* Peek() const;
    void EnsureWriteable(size_t len);
    void HasWritten(size_t len);

    // 调整读位置
    void Retrieve(size_t len);
    void RetrieveUntil(const char* end);

    // 将缓冲区所有数据读出
    void RetrieveAll() ;
    std::string RetrieveAllToStr();

    const char* BeginWriteConst() const;
    char* BeginWrite();

    // 重载函数-将数据写入缓冲区
    void Append(const std::string& str);
    void Append(const char* str, size_t len);
    void Append(const void* data, size_t len);
    void Append(const Buffer& buff);

    // 分散读入数据
    ssize_t ReadFd(int fd, int* Errno);
    // 同一写出数据
    ssize_t WriteFd(int fd, int* Errno);

private:
    // 返回缓冲区首指针
    char* BeginPtr_();
    const char* BeginPtr_() const;
    // 调整缓冲区空间（1.移动，2.扩容）
    void MakeSpace_(size_t len);

    std::vector<char> buffer_;
    std::atomic<std::size_t> readPos_;
    std::atomic<std::size_t> writePos_;
};

#endif //BUFFER_H
