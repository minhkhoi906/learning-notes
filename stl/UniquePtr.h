#pragma once

#include <utility>

template <typename T> struct UniquePtr {
    explicit UniquePtr(T *point = nullptr) noexcept : m_ptr(point) {}
    ~UniquePtr() { delete m_ptr; }

    // Delete copy constructor and copy assignment
    UniquePtr(const UniquePtr &) = delete;
    UniquePtr &operator=(const UniquePtr &) = delete;

    // Move constructor
    UniquePtr(UniquePtr &&other) : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }
    // Move assignment
    UniquePtr &operator=(UniquePtr &&other)
    {
        if (this != &other) {
            delete m_ptr;
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }

        return *this;
    }

    T *release()
    {
        T *temp = m_ptr;
        m_ptr = nullptr;
        return temp;
    }

    void reset(T *pointer = nullptr)
    {
        delete m_ptr;
        m_ptr = pointer;
    }

    T *get() const { return m_ptr; }

    T &operator*() const { return *m_ptr; }
    T *operator->() const { return m_ptr; }

    explicit operator bool() const noexcept { return m_ptr != nullptr; }

  private:
    T *m_ptr;
};
