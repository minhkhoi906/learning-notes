#pragma once

#include <condition_variable>
#include <cstring>
#include <iostream>
#include <mutex>
#include <stdexcept>

template <typename T> class Queue
{
  public:
    // Default ctor
    Queue() : m_Head(nullptr), m_Tail(nullptr), m_Size(0) {}
    ~Queue() { clear(); }

    // Parameterized ctor
    Queue(size_t size, T default_val) : Queue()
    {
        for (size_t idx = 0; idx < size; ++idx) {
            enqueue(default_val);
        }
    }
    // Copy ctor
    Queue(const Queue &other) : Queue()
    {
        Node *pCur = other.m_Head;
        while (pCur) {
            enqueue(pCur->data);
            pCur = pCur->pNext;
        }
    }
    // Copy assignment
    Queue &operator=(const Queue &other)
    {
        if (this != &other) {
            clear();

            Node *pCur = other.m_Head;
            while (pCur) {
                enqueue(pCur->data);
                pCur = pCur->pNext;
            }
        }

        return *this;
    }

    // Move ctor
    Queue(Queue &&other)
        : m_Head(other.m_Head), m_Tail(other.m_Tail), m_Size(other.m_Size)
    {
        other.m_Head = other.m_Tail = nullptr;
        other.m_Size = 0;
    }

    // Move assignment
    Queue &operator=(Queue &&other)
    {
        if (this != &other) {
            clear();
            m_Head = other.m_Head;
            m_Tail = other.m_Tail;
            m_Size = other.m_Size;

            other.m_Head = other.m_Tail = nullptr;
            other.m_Size = 0;
        }
        return *this;
    }

    size_t size() const { return m_Size; }

    bool isEmpty() const { return m_Head == nullptr; }

    void enqueue(const T &val)
    {
        auto temp = new Node{val, nullptr};

        if (isEmpty()) {
            m_Tail = m_Head = temp;
        } else {
            m_Tail->pNext = temp;
            m_Tail = temp;
        }

        m_Size++;
    }

    void dequeue()
    {
        if (isEmpty()) {
            return;
        }

        Node *temp = m_Head;
        if (!m_Head->pNext) {
            m_Head = m_Tail = nullptr;
        } else {
            m_Head = m_Head->pNext;
        }

        delete temp;
        m_Size--;
    }

    T &front() const
    {
        if (isEmpty()) {
            throw std::out_of_range("Queue is Empty");
        }

        return m_Head->data;
    }

  private:
    struct Node {
        T data;
        Node *pNext;
    };

    void clear()
    {
        while (!isEmpty()) {
            dequeue();
        }
    }

    Node *m_Head;
    Node *m_Tail;
    size_t m_Size;
};

void RunTestCases();