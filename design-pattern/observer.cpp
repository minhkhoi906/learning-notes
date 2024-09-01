#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "syscall.h"

using namespace std;

class IObserver
{
  public:
    virtual ~IObserver() = default;
    virtual void update(float price) = 0;
};

class ISubject
{
  public:
    virtual ~ISubject() = default;
    virtual void attach(shared_ptr<IObserver> observer) = 0;
    virtual void detach(shared_ptr<IObserver> observer) = 0;
    virtual void notify(float price) = 0;
};

class DummyProduct : public ISubject
{
  public:
    void attach(shared_ptr<IObserver> observer) override
    {
        observers.push_back(observer);
    }

    void detach(shared_ptr<IObserver> observer) override
    {
        observers.erase(remove(observers.begin(), observers.end(), observer),
                        observers.end());
    }

    void notify(float price) override
    {
        for (const auto &observer : observers) {
            observer->update(price);
        }
    }

    void changePrice(float price) { notify(price); }

  private:
    vector<shared_ptr<IObserver>> observers;
};

class Shop : public IObserver
{
  public:
    explicit Shop(const string &name) : price_(0.0), name_(name) {}

    void update(float price) override { price_ = price; }

    void showShopInfo() const
    {
        cout << "Shop: " << name_ << ", Price: " << price_ << endl;
    }

  private:
    float price_;
    string name_;
};

int main()
{
    DummyProduct product;

    auto shop1 = make_shared<Shop>("Shop 1");
    auto shop2 = make_shared<Shop>("Shop 2");

    product.attach(shop1);
    product.attach(shop2);

    // Change the product's price, this should update the shops automatically
    product.changePrice(23.0f);

    shop1->showShopInfo();
    shop2->showShopInfo();

    // Now shop2 is not interested in new prices so they unsubscribe
    product.detach(shop2);

    // Change the product's price again
    product.changePrice(26.0f);

    shop1->showShopInfo();
    shop2->showShopInfo(); // shop2 should not have updated

    return 0;
}
