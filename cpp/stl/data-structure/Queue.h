#pragma once

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T> class Queue
{
  public:
    // Default constructor
    Queue() = default;

    // Initializer list constructor
    Queue(std::initializer_list<T> init)
    {
        for (const auto &item : init) {
            push(item);
        }
    }

    // Copy constructor
    Queue(const Queue &other)
    {
        try {
            copy_from(other);
        } catch (...) {
            clear();
            throw;
        }
    }

    // Copy assignment operator
    Queue &operator=(const Queue &other)
    {
        if (this != &other) {
            Queue temp(other); // Copy-and-swap idiom
            swap(temp);
        }
        return *this;
    }

    // Move constructor
    Queue(Queue &&other) noexcept
        : m_head(std::exchange(other.m_head, nullptr)),
          m_tail(std::exchange(other.m_tail, nullptr)),
          m_size(std::exchange(other.m_size, 0))
    {
    }

    // Move assignment operator
    Queue &operator=(Queue &&other) noexcept
    {
        if (this != &other) {
            clear();
            m_head = std::exchange(other.m_head, nullptr);
            m_tail = std::exchange(other.m_tail, nullptr);
            m_size = std::exchange(other.m_size, 0);
        }
        return *this;
    }

    // Destructor
    ~Queue() { clear(); }

    // Element access
    T &front()
    {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return m_head->value;
    }

    const T &front() const
    {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return m_head->value;
    }

    T &back()
    {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return m_tail->value;
    }

    const T &back() const
    {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return m_tail->value;
    }

    // Capacity
    [[nodiscard]] bool empty() const noexcept { return m_size == 0; }

    [[nodiscard]] std::size_t size() const noexcept { return m_size; }

    // Modifiers
    void push(const T &value) { emplace(value); }

    void push(T &&value) { emplace(std::move(value)); }

    template <typename... Args> void emplace(Args &&...args)
    {
        auto new_node = std::make_unique<Node>(std::forward<Args>(args)...);

        if (empty()) {
            m_head = std::move(new_node);
            m_tail = m_head.get();
        } else {
            m_tail->next = std::move(new_node);
            m_tail = m_tail->next.get();
        }
        ++m_size;
    }

    void pop()
    {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }

        m_head = std::move(m_head->next);
        --m_size;

        if (!m_head) {
            m_tail = nullptr;
        }
    }

    void clear() noexcept
    {
        while (!empty()) {
            m_head = std::move(m_head->next);
            --m_size;
        }
        m_tail = nullptr;
    }

    void swap(Queue &other) noexcept
    {
        using std::swap;
        swap(m_head, other.m_head);
        swap(m_tail, other.m_tail);
        swap(m_size, other.m_size);
    }

    // Comparison operators
    bool operator==(const Queue &other) const
    {
        if (size() != other.size()) {
            return false;
        }

        auto current1 = m_head.get();
        auto current2 = other.m_head.get();

        while (current1 && current2) {
            if (current1->value != current2->value) {
                return false;
            }
            current1 = current1->next.get();
            current2 = current2->next.get();
        }

        return true;
    }

    bool operator!=(const Queue &other) const { return !(*this == other); }

  private:
    struct Node {
        T value;
        std::unique_ptr<Node> next;

        template <typename... Args>
        explicit Node(Args &&...args)
            : value(std::forward<Args>(args)...), next(nullptr)
        {
        }
    };

    void copy_from(const Queue &other)
    {
        auto current = other.m_head.get();
        while (current) {
            push(current->value);
            current = current->next.get();
        }
    }

    std::unique_ptr<Node> m_head{nullptr};
    Node *m_tail{nullptr};
    std::size_t m_size{0};
};

// Non-member swap
template <typename T> void swap(Queue<T> &lhs, Queue<T> &rhs) noexcept
{
    lhs.swap(rhs);
}