#pragma once

#include "queue.h"

#include <condition_variable>
#include <mutex>

template <typename T> class QueueSafe
{
  public:
    // Push new element into the queue using perfect forwarding
    template <typename U> void push(U &&val)
    {
        {
            // Lock the mutex for thread-safe access
            std::lock_guard<std::mutex> guard(m_Mtx);
            m_Buffer.enqueue(std::forward<U>(val)); // Use perfect forwarding
        }
        m_Cv.notify_one(); // Notify waiting threads after the lock is released
    }

    // Pop element from the queue, blocks if the queue is empty
    T pop()
    {
        std::unique_lock<std::mutex> unique_guard(m_Mtx);
        // Wait until there's an element to pop
        m_Cv.wait(unique_guard, [&] { return !m_Buffer.empty(); });

        T front = std::move(m_Buffer.front()); // Move the front element
        m_Buffer.pop();                        // Remove the front element
        return front;
    }

  private:
    Queue<T> m_Buffer;            // Underlying buffer (Queue class implementation)
    std::mutex m_Mtx;             // Mutex for thread safety
    std::condition_variable m_Cv; // Condition variable to handle blocking
};
