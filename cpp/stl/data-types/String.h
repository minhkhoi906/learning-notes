#pragma once

#include <cstring>   // for std::strlen, std::copy
#include <stdexcept> // for std::out_of_range
#include <memory>

struct String {
  public:
    // Default / C-string constructor
    explicit String(const char *str = "")
        : m_size(std::strlen(str)), m_data(std::make_unique<char[]>(m_size + 1))
    {
        std::copy(str, str + m_size + 1, m_data.get()); // Copy with null terminator
    }

    // Copy constructor
    String(const String &other)
        : m_size(other.m_size), m_data(std::make_unique<char[]>(other.m_size + 1))
    {
        std::copy(other.m_data.get(), other.m_data.get() + m_size + 1, m_data.get());
    }

    // Move constructor
    String(String &&other) noexcept
        : m_size(std::exchange(other.m_size, 0)), m_data(std::move(other.m_data))
    {
    }

    // Copy assignment
    String &operator=(const String &other)
    {
        if (this != &other) {
            m_size = other.m_size;
            m_data.reset(new char[m_size + 1]);
            std::copy(other.m_data.get(), other.m_data.get() + m_size + 1,
                      m_data.get());
        }
        return *this;
    }

    // Move assignment
    String &operator=(String &&other) noexcept
    {
        if (this != &other) {
            m_size = other.m_size;
            m_data = std::move(other.m_data);
            other.m_size = 0;
        }
        return *this;
    }

    ~String() = default; // Explicitly default it for Rule of 5/0 completeness

    // Accessors
    size_t size() const noexcept { return m_size; }
    const char *c_str() const noexcept { return m_data.get(); }

    // Subscript operator (const)
    const char& operator[](size_t index) const
    {
        if (index >= m_size)
            throw std::out_of_range("String index out of range");
        return m_data.get()[index];
    }

    // Subscript operator (non-const)
    char &operator[](size_t index)
    {
        if (index >= m_size)
            throw std::out_of_range("String index out of range");
        return m_data.get()[index];
    }

  private:
    size_t m_size = 0;
    std::unique_ptr<char[]> m_data;
};
