#include <cassert>
#include <initializer_list>
#include <mutex>
#include <stdexcept>
#include <utility>

using namespace std;

template <typename T> struct Queue {
    // Default ctor
    Queue() : pHead_{nullptr}, pTail_{nullptr}, size_{0} {}

    // Parameterized ctor
    Queue(std::initializer_list<T> list) : Queue()
    {
        for (auto &item : list) {
            enqueue(item);
        }
    }
    // Copy ctor
    Queue(const Queue<T> &other) : Queue()
    {
        Node *pCur = other.pHead_;
        while (pCur) {
            enqueue(pCur->data);
            pCur = pCur->pNext;
        }
    }
    // Copy assignment
    Queue &operator=(const Queue<T> &other)
    {
        if (this != &other) {
            clear();

            Node *pCur = other.pHead_;
            while (pCur) {
                enqueue(pCur->data);
                pCur = pCur->pNext;
            }
        }

        return *this;
    }
    // Move ctor
    Queue(Queue<T> &&other)
        : pHead_{other.pHead_}, pTail_{other.pTail_}, size_{other.size_}
    {
        other.pHead_ = other.pTail_ = nullptr;
        other.size_ = 0;
    }
    // Move assignment
    Queue &operator=(Queue<T> &&other)
    {
        if (this != &other) {
            clear();

            pHead_ = other.pHead_;
            pTail_ = other.pTail_;
            size_ = other.size_;

            other.pHead_ = other.pTail_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    // Dtor
    ~Queue() { clear(); }

    size_t size() const { return size_; }

    void enqueue(const T &value)
    {
        lock_guard<mutex> guard(lock_);
        Node *newNode = new Node{value, nullptr};
        if (!pHead_) {
            pHead_ = pTail_ = newNode;
        } else {
            pTail_->pNext = newNode;
            pTail_ = newNode;
        }

        ++size_;
    }

    T &front() const
    {
        if (empty()) {
            throw std::out_of_range("Queue is empty.");
        }

        return pHead_->data;
    }

    bool empty() const { return size_ == 0; }

    void dequeue() noexcept
    {
        lock_guard<mutex> guard(lock_);
        if (empty()) {
            return;
        }

        Node *temp = pHead_;
        pHead_ = pHead_->pNext;

        if (!pHead_) {
            pTail_ = nullptr;
        }

        delete temp;
        --size_;
    }

  private:
    struct Node {
        T data;
        Node *pNext;
    };

    Node *pHead_;
    Node *pTail_;
    size_t size_;
    mutex lock_;

    void clear()
    {
        while (!empty()) {
            dequeue();
        }
    }
};

int main()
{
    // Default ctor
    Queue<int> q0;
    Queue<float> q1;

    // Parameterized ctor
    Queue<int> q2 = {1, 2, 3, 4};
    assert(q2.size() == 4);

    // Copy ctor
    Queue<int> q3 = q2;
    assert(q3.size() == 4);

    // Copy assignment
    q0 = q3;
    assert(q0.size() == 4);

    // Move ctor
    Queue<int> q4 = std::move(q3);
    assert(q4.size() == 4);
    assert(q3.size() == 0);

    // Enqueue method
    q4.enqueue(5);
    assert(q4.size() == 5);

    // Front method
    auto item = q4.front();
    assert(item == 1);
    assert(q4.size() == 5);

    // Dequeue method
    q4.dequeue();
    assert(q4.size() == 4);

    return 0;
}