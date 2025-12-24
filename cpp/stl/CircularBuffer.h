#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

struct CircularBuffer {
    CircularBuffer() = default;
    explicit CircularBuffer(size_t capacity)
        : m_buffer(capacity), m_capacity(capacity), m_size(0), m_head(0), m_tail(0)
    {
        if (capacity == 0) {
            std::__throw_invalid_argument("Capacity must be > 0");
        }
    };

    // Functionalities
    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool isEmpty() const { return m_size == 0; }
    bool isFull() const { return m_size == m_capacity; }

    // Push
    void enqueue(int val)
    {
        if (isFull()) {
            m_head = (++m_head) % m_capacity;
        } else {
            ++m_size;
        }

        m_buffer[m_tail] = val;
        m_tail = (++m_tail) % m_capacity;
        ;
    }

    // dequeue
    int dequeue()
    {
        if (isEmpty()) {
            std::__throw_out_of_range("Queue is empty.");
        }

        int res = m_buffer[m_head];

        ++m_head;
        if (m_head == m_capacity) {
            m_head -= m_capacity;
        }

        --m_size;

        return res;
    }

  private:
    std::vector<int> m_buffer;
    size_t m_capacity{0};
    size_t m_size{0};
    size_t m_head{0}; // index of oldest element
    size_t m_tail{0}; // index where next enqueue writes
};
