#pragma once

#include <algorithm>
#include <memory>
#include <utility>

template <typename Type, size_t N> struct Array {
    // Ctor
    Array() : m_data(std::make_unique<Type[]>(N)) {}
    ~Array() = default;
    // Value-initialized constructor
    explicit Array(Type default_value) : m_data(std::make_unique<Type[]>(N))
    {
        std::fill(m_data.get(), m_data.get() + N, default_value);
    }
    // Copy constructor
    Array(const Array &other) : m_data(std::make_unique<Type[]>(N))
    {
        std::copy(other.m_data.get(), other.m_data.get() + N, m_data.get());
    }
    // Copy assignment
    Array &operator=(const Array &other)
    {
        if (this != &other) {
            auto new_data = std::make_unique<Type[]>(N);
            std::copy(other.m_data.get(), other.m_data.get() + N, new_data.get());
            m_data = std::move(new_data);
        }
        return *this;
    }
    // Move constructor
    Array(Array &&other) noexcept : m_data(std::move(other.m_data)) {}
    // Move assignment
    Array &operator=(Array &&other)
    {
        if (this != &other) {
            m_data = std::move(other.m_data);
        }
        return *this;
    }

    Type& operator[](size_t index) {
        if (index >= N)
            throw std::out_of_range("Index out of range");
        return m_data[index];
    }
    const Type& operator[](size_t index) const {
        if (index >= N)
            throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    // Size accessor
    constexpr size_t size() const noexcept { return N; }

  private:
    size_t m_size = N;
    std::unique_ptr<Type[]> m_data;
};
