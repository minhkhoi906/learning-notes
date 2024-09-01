// Implement a unique poiner
#include <cassert>
#include <iostream>

template <typename T> class UniquePtr
{
  private:
    T *ptr_;

  public:
    explicit UniquePtr<T>(T *ptr = nullptr) : ptr_(ptr) {}
    ~UniquePtr() { delete ptr_; }

    // Delete copy constructor and copy assignment operator
    UniquePtr(const UniquePtr<T> &other) = delete;
    UniquePtr<T> &operator=(const UniquePtr<T> &other) = delete;

    // Move ctor
    UniquePtr(UniquePtr<T> &&other) : ptr_(other.ptr_) { other.ptr_ = nullptr; };

    UniquePtr<T> &operator=(UniquePtr<T> &&other)
    {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }

        return *this;
    }

    // Get the raw pointer
    T *get() const { return ptr_; }
};

int main()
{
    UniquePtr<int> ptr_1;
    UniquePtr<int> ptr_2(new int(1));
    UniquePtr<float> ptr_3(new float(0.2));

    // ptr_1 = ptr_2; // compile will have an error

    assert(ptr_1.get() == nullptr);
    assert(*ptr_2.get() == 1);
    assert(*ptr_3.get() == 0.2f);

    ptr_1 = std::move(ptr_2);
    assert(ptr_2.get() == nullptr);
    assert(*ptr_1.get() == 1);

    return 0;
}