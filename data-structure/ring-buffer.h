#pragma once

#include <array>
#include <cassert>

template <typename Type, size_t Size> class RingBuffer
{
  public:
    RingBuffer() : m_Head(0), m_Tail(0), m_Full(false){};
    void push(const Type &val)
    {
        if (isFull()) {
            m_Tail = (m_Tail + 1) % Size;
        }

        m_Buffer[m_Head] = val;
        m_Head = (m_Head + 1) % Size;
        m_Full = (m_Head == m_Tail);
    }
    Type pop()
    {
        assert(!isEmpty());

        Type item = m_Buffer[m_Tail];
        m_Tail = (m_Tail + 1) % Size;
        m_Full = false;

        return std::move(item);
    }
    void reset()
    {
        m_Head = m_Tail = 0;
        m_Full = false;
    }
    Type &front() const
    {
        assert(!isEmpty());

        return m_Buffer[m_Tail];
    }
    bool isEmpty() const { return (!m_Full && (m_Head == m_Tail)); }
    bool isFull() const { return m_Full; }
    size_t capacity() const { return Size; }
    size_t size() const
    {
        if (isFull()) {
            return Size;
        }

        return (m_Head >= m_Tail) ? m_Head - m_Tail : Size - (m_Tail - m_Head);
    }

  private:
    Type m_Buffer[Size];
    size_t m_Head; // The next item of buffer
    size_t m_Tail; // The oldest item of buffer
    bool m_Full;
};