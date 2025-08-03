#pragma once

#include <Array>

template <typename T, size_t S>
struct CircularBuffer
{
    CircularBuffer() = default;
    CircularBuffer(size_t size = S) : {}

private:
    std::array<T, S> m_capability;
};
