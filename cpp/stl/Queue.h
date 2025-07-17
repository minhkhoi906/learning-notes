#pragma once

#include <utility>

template <typename Type> struct Queue {
    // Default ctor
    Queue() : m_head(nullptr), m_tail(nullptr), m_size(0) {}
    // Copy ctor
    Queue(const Queue &other) : m_head(nullptr), m_tail(nullptr), m_size(0)
    {
        Node *current = other.m_head;
        while (current) {
            push(current->val);
            current = current->p_next;
        }
    }
    // Copy assignment
    Queue &operator=(const Queue &other)
    {
        if (this != &other) {
            clear();
            Node *current = other.m_head;
            while (current) {
                push(current->val);
                current = current->p_next;
            }
        }
        return *this;
    }
    // Move ctor
    Queue(Queue &&other)
        : m_head(std::exchange(other.m_head, nullptr)),
          m_tail(std::exchange(other.m_tail, nullptr)),
          m_size(std::exchange(other.m_size, 0))
    {
    }
    // Move assignment
    Queue &operator=(Queue &&other)
    {
        if (this != &other) {
            clear();
            m_head = std::exchange(other.m_head, nullptr);
            m_tail = std::exchange(other.m_tail, nullptr);
            m_size = std::exchange(other.m_size, 0);
        }
        return *this;
    }

    ~Queue() { clear(); }

    bool isEmpty() const { return m_size == 0; }

    void push(const Type &value)
    {
        Node *new_node = new Node(value);
        if (isEmpty()) {
            m_head = m_tail = new_node;
        } else {
            m_tail->p_next = new_node;
            m_tail = new_node;
        }
        ++m_size;
    }

    void pop()
    {
        if (isEmpty())
            return;
        Node *tmp = m_head;
        m_head = m_head->p_next;
        delete tmp;
        --m_size;
        if (!m_head)
            m_tail = nullptr;
    }

    void clear()
    {
        while (!isEmpty())
            pop();
    }

    Queue front() const { return ; }

  private:
    struct Node {
        Type val;
        Node *p_next;
        explicit Node(Type value) : val(value), p_next(nullptr) {}
    };
    Node *m_head;
    Node *m_tail;
    size_t m_size;
};
