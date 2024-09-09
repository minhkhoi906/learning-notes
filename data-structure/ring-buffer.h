#pragma once

#include <array>

template <typename T, size_t S> class RingBuffer
{
  public:
    RingBuffer() : m_Head(0), m_Tail(0), m_Full(false) {}
    constexpr size_t capacity() const { return S; }
    inline bool isFull() const { return m_Full; }

    inline bool isEmpty() const { return (!m_Full && (m_Head == m_Tail)); }
    inline size_t size() const
    {
        return (m_Head > m_Tail) ? (m_Head - m_Tail)
                                 : (capacity() - (m_Tail - m_Head));
    }
    // Put data into the buffer
    void put(const T &val)
    {
        m_Buffer[m_Head] = val;
        if (m_Full) {
            // Overwrite mode: move tail forward when full
            m_Tail = (m_Tail + 1) % capacity();
        }
        m_Head = (m_Head + 1) % capacity();
        m_Full = (m_Head == m_Tail); // Buffer is full when head catches up to tail
    }
    // Get data from the buffer
    T get()
    {
        if (isEmpty()) {
            throw std::out_of_range("Buffer is empty.");
        }
        T value = m_Buffer[m_Tail];
        m_Tail = (m_Tail + 1) % capacity();
        m_Full = false; // Buffer can't be full after a get operation
        return value;
    }

    void reset()
    {
        m_Head = m_Tail = 0;
        m_Full = false;
    }

  private:
    std::array<T, S> m_Buffer;
    size_t m_Head; // Index of the newest element
    size_t m_Tail; // Index of the oldest element
    bool m_Full;   // Flag to indicate if buffer is full
};