#pragma once

#include <algorithm>
#include <initializer_list>
#include <utility>

template <typename T> struct Vector {
    Vector() : m_data(nullptr), m_size(0), m_capacity(0) {}
    explicit Vector(size_t size)
        : m_data(new T[size]), m_size(size), m_capacity(size)
    {
    }

    Vector(std::initializer_list<T> list)
        : m_data(new T[list.size()]), m_size(list.size()), m_capacity(list.size())
    {
        std::copy(list.begin(), list.end(), m_data);
    }

    // Copy ctor
    Vector(const Vector &other)
        : m_data(new T[other.m_capacity]), m_size(other.m_size), m_capacity(other.m_capacity) {
        std::copy(other.m_data, other.m_data + other.m_size, m_data);
    }

    // Copy assignment
    Vector &operator=(const Vector &other)
    {
        if (this != &other) {
            T *new_data = new T[other.m_capacity];
            std::copy(other.m_data, other.m_data + other.m_size, new_data);

            delete[] m_data;
            m_data = new_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
        }

        return *this;
    }

    // Move ctor
    Vector(Vector &&other)
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    // Move assignment
    Vector &operator=(Vector &&other)
    {
        if (this != &other) {
            delete[] m_data;

            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;

            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }

        return *this;
    }

    ~Vector() { delete[] m_data; }

    void reserve(size_t new_capacity)
    {
        if (new_capacity <= m_capacity) {
            return;
        }

        T *new_data = new T[new_capacity];
        std::move(m_data, m_data + m_size, new_data);

        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;
    }

    bool empty() const noexcept { return (m_size == 0); }
    size_t capacity() const noexcept { return m_capacity; }
    size_t size() const noexcept { return m_size; }

    void push_back(const T &val)
    {
        ensure_capacity();
        m_data[m_size++] = std::move(val);
    }

    void push_back(T &&val)
    {
        ensure_capacity();
        m_data[m_size++] = std::move(val);
    }

    T &operator[](int index)
    {
        if (index >= m_size)
            throw std::out_of_range("Index out of bounds");

        return m_data[index];
    }

  private:
    T *m_data;
    size_t m_size;
    size_t m_capacity;

    // Ensure there is enough capacity
    void ensure_capacity()
    {
        if (m_size == m_capacity) {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }
    }
};
