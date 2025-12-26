#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>

struct CircularBuffer {
    CircularBuffer() = default;
    explicit CircularBuffer(size_t capacity)
        : m_buffer(std::make_unique<int[]>(capacity)), m_capacity(capacity),
          m_size(0), m_head(0), m_tail(0)
    {
        if (capacity == 0) {
            std::__throw_invalid_argument("Capacity must be > 0");
        }
    };

    ~CircularBuffer() = default;
    CircularBuffer(CircularBuffer &&other) = default;
    CircularBuffer &operator=(CircularBuffer &&other) = default;

    // Deep copy
    CircularBuffer(const CircularBuffer &other)
        : m_buffer(other.m_capacity ? std::make_unique<int[]>(other.m_capacity)
                                    : nullptr),
          m_capacity(other.m_capacity), m_size(other.m_size), m_head(other.m_head),
          m_tail(other.m_tail)
    {
        std::copy(other.m_buffer.get(), other.m_buffer.get() + other.m_capacity,
                  m_buffer.get());
    }

    CircularBuffer &operator=(const CircularBuffer &other)
    {
        if (this != &other) {
            auto new_buff = other.m_capacity
                                ? std::make_unique<int[]>(other.m_capacity)
                                : nullptr;
            std::copy(other.m_buffer.get(), other.m_buffer.get() + other.m_capacity,
                      new_buff.get());

            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_buffer = std::move(new_buff);
        }
        return *this;
    }

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

        m_head = (m_head + 1) % m_capacity;
        --m_size;

        return res;
    }

  private:
    std::unique_ptr<int[]> m_buffer;
    size_t m_capacity{0};
    size_t m_size{0};
    size_t m_head{0}; // index of oldest element
    size_t m_tail{0}; // index where next enqueue writes

    void swap() {}
};
