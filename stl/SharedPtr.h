#pragma once

#include <utility>

template <typename Type> struct SharedPtr {
    SharedPtr() noexcept : m_ptr(nullptr), m_count(new long(0)) {}
    // Constructor with raw pointer
    explicit SharedPtr(Type *ptr) noexcept
        : m_ptr(ptr), m_count(new long(ptr ? 1 : 0))
    {
    }
    // Copy ctor
    SharedPtr(const SharedPtr &other) : m_ptr(other.m_ptr), m_count(other.m_count)
    {
        add_ref();
    }
    // Move ctor
    SharedPtr(SharedPtr &&other) noexcept
        : m_ptr(other.m_ptr), m_count(other.m_count)
    {
        other.m_ptr = nullptr;
        other.m_count = nullptr;
    }
    // Copy assignment
    SharedPtr &operator=(const SharedPtr &other)
    {
        if (this != &other) {
            release(); // release current
            m_ptr = other.m_ptr;
            m_count = other.m_count;
            add_ref();
        }
        return *this;
    }
    // Move assignment
    SharedPtr &operator=(SharedPtr &&other)
    {
        if (this != &other) {
            release();
            m_ptr = other.m_ptr;
            m_count = other.m_count;
            other.m_ptr = nullptr;
            other.m_count = nullptr;
        }
        return *this;
    }
    // Dtor
    ~SharedPtr() { release(); }

    // Operators
    Type *operator->() const { return m_ptr; }
    Type &operator*() const { return *m_ptr; }
    const bool operator!() const { return m_ptr == nullptr; }
    bool operator==(const SharedPtr &other) { return m_ptr == other.m_ptr; }
    bool operator!=(const SharedPtr &other) { return m_ptr != other.m_ptr; }
    // Observers
    explicit operator bool() const noexcept { return m_ptr != nullptr; }
    Type *get() const { return m_ptr; }
    long use_count() const { return m_count ? *m_count : 0; }

    void reset(Type *ptr = nullptr)
    {
        release();
        m_ptr = ptr;
        m_count = new long(ptr ? 1 : 0);
    }

  private:
    Type *m_ptr;
    long *m_count;

    void release()
    {
        if (m_count && --(*m_count) == 0) {
            delete m_ptr;
            delete m_count;
        }
        m_ptr = nullptr;
        m_count = nullptr;
    }

    void add_ref() noexcept
    {
        if (m_count) {
            ++(*m_count);
        }
    }
};
