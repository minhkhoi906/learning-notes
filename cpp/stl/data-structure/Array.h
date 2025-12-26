#pragma once

#include <algorithm>

template <typename Type, size_t N> struct Array {
    ~Array() = default;
    // Default constructor (zero-initialized if applicable)
    Array() = default;
    // Value-initialized constructor
    explicit Array(Type default_value)
    {
        std::fill(std::begin(m_data), std::end(m_data), default_value);
    }
    // Copy constructor
    Array(const Array &other)
    {
        std::copy(std::begin(other.m_data), std::end(other.m_data),
                  std::begin(m_data));
    }
    // Copy assignment
    Array &operator=(const Array &other)
    {
        if (this != &other) {
            std::copy(std::begin(other.m_data), std::end(other.m_data),
                      std::begin(m_data));
        }
        return *this;
    }
    // Move constructor
    Array(Array &&other) noexcept
    {
        std::move(std::begin(other.m_data), std::end(other.m_data),
                  std::begin(m_data));
    }
    // Move assignment
    Array &operator=(Array &&other)
    {
        if (this != &other) {
            std::move(std::begin(other.m_data), std::end(other.m_data),
                      std::begin(m_data));
        }
        return *this;
    }

    Type &operator[](size_t index)
    {
        if (index >= N)
            throw std::out_of_range("Index out of range");
        return m_data[index];
    }
    const Type &operator[](size_t index) const
    {
        if (index >= N)
            throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    // Size accessor
    constexpr size_t size() const noexcept { return N; }

  private:
    Type m_data[N];
};
