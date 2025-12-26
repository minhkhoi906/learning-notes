#pragma once

#include "Queue.h"

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
            std::lock_guard<std::mutex> guard(m_mutex);
            m_buffer.push(std::forward<U>(val)); // Use perfect forwarding
        }
        m_cv.notify_one(); // Notify waiting threads after the lock is released
    }

    // Pop element from the queue, blocks if the queue is empty
    T pop()
    {
        std::unique_lock<std::mutex> unique_guard(m_mutex);
        // Wait until there's an element to pop
        m_cv.wait(unique_guard, [&] { return !m_buffer.empty(); });

        T front = std::move(m_buffer.front()); // Move the front element
        m_buffer.pop();                        // Remove the front element
        return front;
    }

  private:
    Queue<T> m_buffer;            // Underlying buffer (Queue class implementation)
    std::mutex m_mutex;           // Mutex for thread safety
    std::condition_variable m_cv; // Condition variable to handle blocking
};
