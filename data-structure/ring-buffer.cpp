#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <stdexcept>

template <typename T, size_t S> class RingBuffer
{
  public:
    RingBuffer() : m_Head(0), m_Tail(0), m_Full(false) {}

    inline size_t capacity() const { return m_Buffer.size(); }
    inline bool isFull() const { return m_Full; }
    inline bool isEmpty() const { return (!m_Full && (m_Head == m_Tail)); }

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

    size_t size() const
    {
        return (m_Head > m_Tail) ? (m_Head - m_Tail)
                                 : (capacity() - (m_Tail - m_Head));
    }

  private:
    std::array<T, S> m_Buffer;
    size_t m_Head; // Index of the newest element
    size_t m_Tail; // Index of the oldest element
    bool m_Full;   // Flag to indicate if buffer is full
};

int main()
{
    RingBuffer<int, 10> buffer;

    assert(buffer.capacity() == 10);
    assert(buffer.isEmpty());
    assert(!buffer.isFull());

    // Test: attempting to get from an empty buffer
    try {
        buffer.get();
    } catch (const std::out_of_range &e) {
        std::cout << "Caught exception: " << e.what() << '\n';
    }

    // Fill the buffer
    for (size_t idx = 0; idx < buffer.capacity(); ++idx) {
        buffer.put(static_cast<int>(idx));
    }

    assert(buffer.isFull());
    assert(buffer.size() == 10);

    assert(buffer.get() == 0); // First item inserted should be retrieved
    assert(buffer.size() == 9);

    // Continue to add and retrieve data
    buffer.put(10);
    assert(buffer.get() == 1); // Second item after buffer wrap

    assert(buffer.size() == 9);
    assert(buffer.get() == 2); // Third item after buffer wrap
    assert(buffer.size() == 8);

    std::cout << "RingBuffer tests passed.\n";
    return 0;
}
