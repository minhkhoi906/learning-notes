#include <algorithm>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <string>

using namespace std;

// The IObserver interface remains the same
class IObserver
{
  public:
    virtual ~IObserver() = default;
    virtual void update(float price) = 0;
};

// The ISubject interface remains the same
class ISubject
{
  public:
    virtual ~ISubject() = default;
    virtual void attach(shared_ptr<IObserver> observer) = 0;
    virtual void detach(shared_ptr<IObserver> observer) = 0;
    virtual void notify(float price) = 0;
};

// DummyProduct is renamed to Product and enhanced
class Product : public ISubject
{
  public:
    // Attaches an observer, but first locks the mutex for thread safety.
    void attach(shared_ptr<IObserver> observer) override
    {
        lock_guard<mutex> lock(m_mutex);
        m_observers.push_back(observer);
    }

    // Detaches an observer using modern C++ idioms.
    void detach(shared_ptr<IObserver> observer) override
    {
        lock_guard<mutex> lock(m_mutex);
        // Use std::remove_if with a lambda to find and remove the observer
        auto newEnd = remove_if(m_observers.begin(), m_observers.end(),
            [&](const weak_ptr<IObserver>& wp) {
                // Lock the weak_ptr to compare the shared_ptr it holds
                return wp.lock() == observer;
            });
        m_observers.erase(newEnd, m_observers.end());
    }

    // Notifies observers asynchronously.
    void notify(float price) override
    {
        // lock_guard automatically unlocks when it goes out of scope
        lock_guard<mutex> lock(m_mutex);

        // Iterate over the observers and launch a separate task for each one.
        // This is where the async behavior happens.
        for (const auto &wp : m_observers) {
            if (auto observer = wp.lock()) {
                // Use std::async to run the update call on a new thread.
                // This makes the notify call non-blocking.
                async(launch::async, &IObserver::update, observer, price);
            }
        }
    }

    // Public method to change the price and trigger notification.
    void changePrice(float price) {
        cout << "Product price is changing to " << price << endl;
        notify(price);
    }

  private:
    // Use std::weak_ptr to prevent circular dependencies (e.g., if an observer
    // also held a shared_ptr to the subject).
    vector<weak_ptr<IObserver>> m_observers;
    
    // A mutex to protect the observers vector from concurrent access.
    mutex m_mutex;
};

// Shop remains a simple concrete observer.
class Shop : public IObserver
{
  public:
    explicit Shop(const string &name) : price_(0.0), name_(name) {}

    void update(float price) override {
        cout << "[Observer: " << name_ << "] Received price update: " << price << endl;
        price_ = price;
    }

    void showShopInfo() const
    {
        cout << "Shop: " << name_ << ", Current Price: " << price_ << endl;
    }

  private:
    float price_;
    string name_;
};

int main()
{
    Product product;

    auto shop1 = make_shared<Shop>("Shop 1");
    auto shop2 = make_shared<Shop>("Shop 2");

    product.attach(shop1);
    product.attach(shop2);

    // Change the product's price. The notification will happen on a different thread.
    product.changePrice(23.0f);
    this_thread::sleep_for(chrono::milliseconds(100)); // Give time for async updates to finish.

    shop1->showShopInfo();
    shop2->showShopInfo();

    // Now shop2 is not interested, so it unsubscribes.
    product.detach(shop2);
    cout << "\nShop 2 has detached." << endl;

    // Change the product's price again.
    product.changePrice(26.0f);
    this_thread::sleep_for(chrono::milliseconds(100));

    shop1->showShopInfo();
    shop2->showShopInfo(); // shop2 should not have been updated

    return 0;
}
