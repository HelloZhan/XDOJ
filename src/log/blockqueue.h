#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <sys/time.h>

template <class T>
class BlockDeque
{
public:
    explicit BlockDeque(size_t MaxCapacity = 1000);

    ~BlockDeque();

    void clear();

    bool empty();

    bool full();

    void Close();

    size_t size();

    size_t capacity();

    T front();

    T back();

    void push_back(const T &item);

    void push_front(const T &item);

    bool pop(T &item);

    bool pop(T &item, int timeout);

    void flush();

private:
    std::deque<T> deq_;

    size_t capacity_;

    std::mutex mtx_;

    bool isClose_;

    std::condition_variable condConsumer_;

    std::condition_variable condProducer_;
};

// 构造函数
template <class T>
BlockDeque<T>::BlockDeque(size_t MaxCapacity) : capacity_(MaxCapacity)
{
    // assert(MaxCapacity > 0);
    isClose_ = false;
}
// 析构函数
template <class T>
BlockDeque<T>::~BlockDeque()
{
    Close();
};

// 关闭
template <class T>
void BlockDeque<T>::Close()
{
    {
        std::lock_guard<std::mutex> locker(mtx_);
        deq_.clear();
        isClose_ = true;
    }
    condProducer_.notify_all();
    condConsumer_.notify_all();
};
// 刷新队列，就是通知消费者消费
template <class T>
void BlockDeque<T>::flush()
{
    condConsumer_.notify_one();
};
// 将队列的消息清空
template <class T>
void BlockDeque<T>::clear()
{
    std::lock_guard<std::mutex> locker(mtx_);
    deq_.clear();
}
// 取出队列头元素
template <class T>
T BlockDeque<T>::front()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.front();
}
// 取出队列尾元素
template <class T>
T BlockDeque<T>::back()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.back();
}
// 返回队列消息数量
template <class T>
size_t BlockDeque<T>::size()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.size();
}
// 返回队列容量大小
template <class T>
size_t BlockDeque<T>::capacity()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return capacity_;
}
// 向队列尾插入消息
template <class T>
void BlockDeque<T>::push_back(const T &item)
{
    std::unique_lock<std::mutex> locker(mtx_);
    // 如果队列满，阻塞
    while (deq_.size() >= capacity_)
    {
        condProducer_.wait(locker);
    }
    deq_.push_back(item);
    // 通知消费者消费
    condConsumer_.notify_one();
}
// 向队列头插入消息
template <class T>
void BlockDeque<T>::push_front(const T &item)
{
    std::unique_lock<std::mutex> locker(mtx_);
    // 如果队列满，阻塞
    while (deq_.size() >= capacity_)
    {
        condProducer_.wait(locker);
    }
    deq_.push_front(item);
    // 通知消费者消费
    condConsumer_.notify_one();
}
// 返回队列是否为空
template <class T>
bool BlockDeque<T>::empty()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.empty();
}
// 查看队列是否已满
template <class T>
bool BlockDeque<T>::full()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.size() >= capacity_;
}
// 取出对头元素
template <class T>
bool BlockDeque<T>::pop(T &item)
{
    std::unique_lock<std::mutex> locker(mtx_);
    // 如果队列为空，阻塞
    while (deq_.empty())
    {
        condConsumer_.wait(locker);
        if (isClose_)
        {
            return false;
        }
    }
    item = deq_.front();
    deq_.pop_front();
    // 通知生产者生产
    condProducer_.notify_one();
    return true;
}
// 取出对头元素，并设置超时时间
template <class T>
bool BlockDeque<T>::pop(T &item, int timeout)
{
    std::unique_lock<std::mutex> locker(mtx_);
    // 如果队列为空，阻塞，直到队列有元素，或者时间超时
    while (deq_.empty())
    {
        if (condConsumer_.wait_for(locker, std::chrono::seconds(timeout)) == std::cv_status::timeout)
        {
            return false;
        }
        if (isClose_)
        {
            return false;
        }
    }
    item = deq_.front();
    deq_.pop_front();
    // 通知生产者生产
    condProducer_.notify_one();
    return true;
}

#endif // BLOCKQUEUE_H
