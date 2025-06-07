#pragma once

#include <utility> // For std::exchange, std::swap
#include <atomic>  // Required for std::atomic

template <typename Type> struct SharedPtr {
    // Constructor with raw pointer (also serves as default constructor)
    explicit SharedPtr(Type *ptr = nullptr) noexcept
        : m_ptr(ptr), m_count(ptr ? new std::atomic<long>(1) : nullptr)
    {
        // Note: new std::atomic<long>(1) can throw std::bad_alloc.
        // A robust std::shared_ptr often uses std::make_shared to
        // allocate the object and the control block (atomic counter)
        // in a single allocation for performance and strong exception safety.
        // For this exercise, we assume allocation success or accept partial construction.
    }

    // Copy constructor
    SharedPtr(const SharedPtr &other) noexcept // Marked noexcept
        : m_ptr(other.m_ptr), m_count(other.m_count)
    {
        add_ref();
    }

    // Move constructor
    SharedPtr(SharedPtr &&other) noexcept
        : m_ptr(std::exchange(other.m_ptr, nullptr)),
          m_count(std::exchange(other.m_count, nullptr))
    {
    }

    // Copy/Move Assignment Operator (Unified using copy-and-swap idiom)
    SharedPtr &operator=(SharedPtr other) noexcept // 'other' is passed by value (copy or move-constructed)
    {
        // 'other' will be copy/move-constructed from the source.
        // Then, we swap 'this' with 'other'.
        // 'other's' destructor will automatically handle the cleanup of 'this's' original resources.
        swap(*this, other);
        return *this;
    }

    // Destructor
    ~SharedPtr() noexcept {
        release();
    }

    // --- Operators ---

    // Dereference operator (non-const)
    Type &operator*() { return *m_ptr; }
    // Dereference operator (const)
    const Type &operator*() const { return *m_ptr; }

    // Member access operator (non-const)
    Type *operator->() { return m_ptr; }
    // Member access operator (const)
    const Type *operator->() const { return m_ptr; }

    // Comparison operators (now const)
    bool operator==(const SharedPtr &other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const SharedPtr &other) const { return m_ptr != other.m_ptr; }

    // --- Observers ---

    // Explicit boolean conversion
    explicit operator bool() const noexcept { return m_ptr != nullptr; }

    // Get raw pointer
    Type *get() const noexcept { return m_ptr; }

    // Get use count (reads atomic counter)
    // Note: use_count() for std::shared_ptr is non-atomic and primarily for debugging.
    // This returns a snapshot, not a guaranteed exact count in highly concurrent scenarios.
    long use_count() const noexcept { return m_count ? m_count->load(std::memory_order_relaxed) : 0; }

    // Reset function
    void reset(Type *ptr = nullptr) {
        if (m_ptr == ptr) {
            return; // Corrected syntax
        }

        release(); // Release current resources
        m_ptr = ptr;
        m_count = ptr ? new std::atomic<long>(1) : nullptr;
    }

  private:
    Type *m_ptr;
    std::atomic<long> *m_count; // Changed to std::atomic for thread-safety

    // release: Decrements ref count, deletes object if count reaches zero.
    void release() noexcept {
        if (m_count) {
            // Decrement the count using fetch_sub with release memory order.
            // This ensures all writes to the managed object (by the thread
            // that is releasing the last reference) are visible before the
            // counter reaches zero.
            long old_count = m_count->fetch_sub(1, std::memory_order_release);

            if (old_count == 1) { // If old_count was 1, it means it's now 0
                // Acquire memory order fence to synchronize with the fetch_sub.
                // This ensures all memory operations by the thread that owned the last reference
                // are visible *before* deletion proceeds.
                std::atomic_thread_fence(std::memory_order_acquire);

                delete m_ptr;
                delete m_count; // Delete the counter itself
            }
        }
        m_ptr = nullptr;
        m_count = nullptr; // Always nullify pointers after release
    }

    // add_ref: Increments ref count.
    void add_ref() noexcept {
        if (m_count) {
            // Increment the count using fetch_add with relaxed memory order.
            // Relaxed order is sufficient here as no specific synchronization
            // is needed with other operations beyond ensuring the increment itself is atomic.
            m_count->fetch_add(1, std::memory_order_relaxed);
        }
    }

    // Friend swap function for copy-and-swap idiom and ADL
    friend void swap(SharedPtr &lhs, SharedPtr &rhs) noexcept
    {
        using std::swap;
        swap(lhs.m_ptr, rhs.m_ptr);
        swap(lhs.m_count, rhs.m_count);
    }
};