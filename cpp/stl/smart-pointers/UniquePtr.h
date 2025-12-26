#pragma once

#include <utility>
#include <algorithm>

template <typename Type> struct UniquePtr {
    // Default constructor
    UniquePtr() noexcept : m_ptr(nullptr) {}
    // Paremeterized constructor
    explicit UniquePtr(Type *ptr) noexcept : m_ptr(ptr) {}
    // Copy constructor
    UniquePtr(const UniquePtr &other) = delete; // Disable copy constructor
    // Move constructor
    UniquePtr(UniquePtr &&other) noexcept : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr; // Reset moved-from object
    }
    // Copy assignment operator
    UniquePtr &
    operator=(const UniquePtr &other) = delete; // Disable copy assignment operator
    // Move assigment operator
    UniquePtr &operator=(UniquePtr &&other) noexcept
    {
        if (this != &other) {
            delete m_ptr;
            m_ptr = other.m_ptr;   // Transfer ownership
            other.m_ptr = nullptr; // Reset moved-from object
        }

        return *this;
    }
    // Destructor
    ~UniquePtr()
    {
        delete m_ptr;    // Delete the managed object
        m_ptr = nullptr; // Avoid dangling pointer
    }

    Type &operator*() const { return *m_ptr; }
    Type *operator->() const { return m_ptr; }

    // Check if the pointer is not null
    explicit operator bool() const { return m_ptr != nullptr; }
    // Get the raw pointer
    Type *get() const { return m_ptr; }
    void reset(Type *ptr = nullptr)
    {
        delete m_ptr; // Delete the current object
        m_ptr = ptr;  // Assign new pointer
    }
    Type *release()
    {
        Type *temp = m_ptr; // Store current pointer
        m_ptr = nullptr;    // Reset managed pointer
        return temp;        // Return the old pointer
    }

  private:
    Type *m_ptr; // Pointer to the managed object
};

template <typename T> struct UniquePtr<T[]> {
  public:
    // Constructors
    UniquePtr() noexcept : m_ptr(nullptr) {}
    explicit UniquePtr(T *ptr) noexcept : m_ptr(ptr) {}
    explicit UniquePtr(size_t size) : m_ptr(new T[size]()) {}

    // Deleted copy constructor/assignment
    UniquePtr(const UniquePtr &) = delete;
    UniquePtr &operator=(const UniquePtr &) = delete;

    // Move constructor
    UniquePtr(UniquePtr &&other) noexcept
        : m_ptr(std::exchange(other.m_ptr, nullptr))
    {
    }

    // Move assignment
    UniquePtr &operator=(UniquePtr &&other) noexcept
    {
        if (this != &other) {
            delete[] m_ptr;
            m_ptr = std::exchange(other.m_ptr, nullptr);
        }
        return *this;
    }

    // Destructor
    ~UniquePtr() { delete[] m_ptr; }

    // Accessors
    T *get() const noexcept { return m_ptr; }
    T *release() noexcept
    {
        T *temp = m_ptr;
        m_ptr = nullptr;
        return temp;
    }

    void reset(T *ptr = nullptr) noexcept
    {
        if (m_ptr != ptr) {
            delete[] m_ptr;
            m_ptr = ptr;
        }
    }

    T &operator[](size_t index) const { return m_ptr[index]; }
    explicit operator bool() const noexcept { return m_ptr != nullptr; }

  private:
    T *m_ptr;
};
