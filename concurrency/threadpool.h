#pragma once
#include "../data-structure/queue-safe.h"

#include <functional>
#include <future>
#include <vector>

class ThreadPool
{
  public:
    explicit ThreadPool(size_t numberOfWorker);

    ~ThreadPool();

  private:
    void WorkerFunc();

  private:
    std::vector<std::thread> m_Workers;
    QueueSafe<std::function<void()>> m_Tasks;
    bool m_Stop;
};