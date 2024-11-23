#pragma once

template<typename Type>
class SharedPtr {
private:
    Type* m_Ptr;
    int* m_Count;

public:
    explicit SharedPtr(Type* ptr = nullptr) : m_Ptr(ptr), m_Count(new int(0)) {
        if (m_Ptr) {
            (*m_Count)++;
        }
    }

    ~SharedPtr() {
        (*m_Count)--;
        if (*m_Count == 0) {
            delete m_Ptr;
            delete m_Count;
        }
    }

    SharedPtr(const SharedPtr& other) {
        if (this != &other) {
            if (other.m_Ptr == nullptr) {
                return nullptr;
            }
            m_Ptr = other.m_Ptr;
            m_Count = other.m_Count;
            (*m_Count)++;
        }

        return *this;
    }



};

void RunTestCases() {
    auto ptr1 = SharedPtr<int>(new int(1));
    auto ptr2 = ptr1;
}