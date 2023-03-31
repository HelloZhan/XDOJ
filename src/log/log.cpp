#include "log.h"

using namespace std;

Log::Log()
{
    lineCount_ = 0;
    isAsync_ = false;
    writeThread_ = nullptr;
    deque_ = nullptr;
    toDay_ = 0;
    fp_ = nullptr;
}

Log::~Log()
{
    // 回收子线程
    if (writeThread_ && writeThread_->joinable())
    {
        while (!deque_->empty())
        {
            deque_->flush();
        };
        deque_->Close();
        writeThread_->join();
    }
    // 将文件描述符中的数据刷新
    if (fp_)
    {
        lock_guard<mutex> locker(mtx_);
        flush();
        fclose(fp_);
    }
}
// 返回级别
int Log::GetLevel()
{
    lock_guard<mutex> locker(mtx_);
    return level_;
}
// 设置级别
void Log::SetLevel(int level)
{
    lock_guard<mutex> locker(mtx_);
    level_ = level;
}
// 初始化，根据级别、路径、后缀、阻塞队列容量大小
void Log::init(int level = 1, const char *path, const char *suffix,
               int maxQueueSize)
{
    isOpen_ = true;
    level_ = level;
    if (maxQueueSize > 0)
    { // 如果最大队列容量大于，则是异步
        isAsync_ = true;
        if (!deque_)
        {
            // 创建阻塞队列
            unique_ptr<BlockDeque<std::string>> newDeque(new BlockDeque<std::string>);
            deque_ = move(newDeque);
            // 创建线程，不停的写数据
            std::unique_ptr<std::thread> NewThread(new thread(FlushLogThread));
            writeThread_ = move(NewThread);
        }
    }
    else
    { // 同步
        isAsync_ = false;
    }

    lineCount_ = 0; // 日志行数

    time_t timer = time(nullptr);
    struct tm *sysTime = localtime(&timer);
    struct tm t = *sysTime;
    path_ = path;
    suffix_ = suffix;
    char fileName[LOG_NAME_LEN] = {0};
    // 文件名 路径-年-月-日-后缀
    snprintf(fileName, LOG_NAME_LEN - 1, "%s/%04d_%02d_%02d%s",
             path_, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, suffix_);
    toDay_ = t.tm_mday;

    {
        lock_guard<mutex> locker(mtx_);
        buff_.RetrieveAll(); // 初始化缓冲区
        if (fp_)
        {
            flush();
            fclose(fp_);
        }
        // 打开文件描述符
        fp_ = fopen(fileName, "a");
        if (fp_ == nullptr)
        {
            mkdir(path_, 0777);
            fp_ = fopen(fileName, "a");
        }
        assert(fp_ != nullptr);
    }
}

void Log::write(int level, const char *format, ...)
{
    struct timeval now = {0, 0};
    gettimeofday(&now, nullptr);
    time_t tSec = now.tv_sec; // 微秒
    // 把秒数转换为年-月-日-秒
    struct tm *sysTime = localtime(&tSec);
    struct tm t = *sysTime;
    va_list vaList; // 不定参数

    /* 日志日期 日志行数 */
    // 日志日期不是当天，日志行数超过限制需要重新写入新的文件
    if (toDay_ != t.tm_mday || (lineCount_ && (lineCount_ % MAX_LINES == 0)))
    {
        unique_lock<mutex> locker(mtx_);
        locker.unlock();

        char newFile[LOG_NAME_LEN]; // 新的文件的名字
        char tail[36] = {0};        // 保存时间
        snprintf(tail, 36, "%04d_%02d_%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);

        if (toDay_ != t.tm_mday) // 如果不是当天
        {
            // 保存新文件名称
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s%s", path_, tail, suffix_);
            toDay_ = t.tm_mday;
            lineCount_ = 0;
        }
        else
        { // 如果日志行数超出限制
            // 保存新文件名称
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s-%d%s", path_, tail, (lineCount_ / MAX_LINES), suffix_);
        }

        locker.lock();
        flush();
        fclose(fp_);
        // 创建新文件
        fp_ = fopen(newFile, "a");
        assert(fp_ != nullptr);
    }
    // 将这条记录打印到日志文件中
    {
        unique_lock<mutex> locker(mtx_);
        lineCount_++;
        // buff保存要输出的文件内容
        int n = snprintf(buff_.BeginWrite(), 128, "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
                         t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                         t.tm_hour, t.tm_min, t.tm_sec, now.tv_usec);
        // buff改变写指针
        buff_.HasWritten(n);
        AppendLogLevelTitle_(level);

        // 将不定参数的内容写入buff中
        va_start(vaList, format);
        int m = vsnprintf(buff_.BeginWrite(), buff_.WritableBytes(), format, vaList);
        va_end(vaList);
        // buff更新写指针，并添加换行结束符
        buff_.HasWritten(m);
        buff_.Append("\n\0", 2);
        // 如果是异步就是写入到阻塞队列中，如果是同步就直接写入
        if (isAsync_ && deque_ && !deque_->full())
        {
            deque_->push_back(buff_.RetrieveAllToStr());
        }
        else
        {
            fputs(buff_.Peek(), fp_);
        }
        // 缓冲区重置
        buff_.RetrieveAll();
    }
}
// 向缓冲区中添加级别
void Log::AppendLogLevelTitle_(int level)
{
    switch (level)
    {
    case 0:
        buff_.Append("[debug]: ", 9);
        break;
    case 1:
        buff_.Append("[info] : ", 9);
        break;
    case 2:
        buff_.Append("[warn] : ", 9);
        break;
    case 3:
        buff_.Append("[error]: ", 9);
        break;
    default:
        buff_.Append("[info] : ", 9);
        break;
    }
}
// 刷新缓冲区
void Log::flush()
{
    if (isAsync_)
    {
        // 刷新队列
        deque_->flush();
    }
    // 刷新文件描述符
    fflush(fp_);
}
// 异步写
void Log::AsyncWrite_()
{
    string str = "";
    while (deque_->pop(str))
    {
        lock_guard<mutex> locker(mtx_);
        fputs(str.c_str(), fp_);
    }
}
// 单例模式，返回静态变量
Log *Log::Instance()
{
    static Log inst;
    return &inst;
}
// 线程执行函数,静态函数（一直执行异步写操作
void Log::FlushLogThread()
{
    Log::Instance()->AsyncWrite_();
}
