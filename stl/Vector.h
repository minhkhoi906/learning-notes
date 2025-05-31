#pragma once

#include <stdexcept>
#include <utility>

template <typename Type> struct Vector {
    // Default constructor
    Vector() noexcept : m_data(nullptr), m_size(0), m_capacity(0){};

    // Parameterized constructor
    explicit Vector(size_t size) : m_size(size), m_capacity(size)
    {
        m_data = new Type[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = Type(); // Default initialize elements
        }
    }

    // Copy constructor
    Vector(const Vector &other) : m_size(other.m_size), m_capacity(other.m_capacity)
    {
        m_data = new Type[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i]; // Deep copy elements
        }
    }

    // Move constructor
    Vector(Vector &&other)
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        other.m_data = nullptr; // Reset moved-from object
        other.m_size = 0;
        other.m_capacity = 0;
    }

    // Destructor
    ~Vector()
    {
        delete[] m_data;
        m_data = nullptr; // Avoid dangling pointer
        m_size = 0;
        m_capacity = 0; // Reset size and capacity
    }

    // Copy assignment operator
    Vector &operator=(const Vector &other)
    {
        if (this != &other) {
            Vector temp(other);
            std::swap(temp, *this);
        }
        return *this;
    }

    // Move assignment operator
    Vector &operator=(Vector &&other)
    {
        if (this != &other) {
            delete[] m_data;
            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.m_data = nullptr; // Reset moved-from object
            other.m_size = 0;
            other.m_capacity = 0;
        }
        return *this;
    }

    void reserve(size_t new_capacity)
    {
        if (new_capacity > m_capacity) {
            Type *new_data = new Type[new_capacity];
            for (size_t i = 0; i < m_size; ++i) {
                new_data[i] = std::move(m_data[i]); // Move elements to new storage
            }
            delete[] m_data;           // Free old storage
            m_data = new_data;         // Point to new storage
            m_capacity = new_capacity; // Update capacity
        }
    }

    void push_back(const Type &value)
    {
        ensure_capacity();
        m_data[m_size++] = value;
    }

    void push_back(Type &&value)
    {
        ensure_capacity();
        m_data[m_size++] = std::move(value); // Move new element
    }

    template <typename... Args> void emplace_back(Args &&...args)
    {
        ensure_capacity();
        m_data[m_size++] = Type(std::forward<Args>(args)...);
    }

    bool empty() const
    {
        return m_size == 0; // Check if vector is empty
    }

    size_t size() const
    {
        return m_size; // Return current size
    }

    size_t capacity() const
    {
        return m_capacity; // Return current capacity
    }
    // Access operator
    Type &operator[](size_t index)
    {
        if (index < 0 || index >= m_size) {
            throw std::out_of_range("Index out of bounds");
        }
        return m_data[index];
    }

  private:
    Type *m_data;
    size_t m_size;
    size_t m_capacity;

    void ensure_capacity()
    {
        if (m_size >= m_capacity) {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }
    }
};