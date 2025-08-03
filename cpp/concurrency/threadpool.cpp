#include "threadpool.h"

#include <thread>

ThreadPool::ThreadPool(size_t numberOfWorker)
    : m_Stop(false)
{
    for (int i = 0; i < numberOfWorker; ++i) {
        m_Workers.emplace_back([this] { this->WorkerFunc(); });
    }
}

ThreadPool::~ThreadPool()
{
    unique_lock<mutex> lock(mtx);
    stop = true;
    lock.unlock();
    cv.notify_all();

    // wait for tasks to complete
    for (const auto &thread : m_Futures)
        thread.join();
}

void ThreadPool::WorkerFunc()
{
    std::function<void()> task;
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this] { return !m_Tasks.empty() || stop; });
        if (stop)
            return;
        task = move(m_Tasks.front());
        m_Tasks.pop();
        lock.unlock();
        task();
    }
}
