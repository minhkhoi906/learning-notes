#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class IFactoryMethod
{
  public:
    virtual void execute() const = 0;
    virtual ~IFactoryMethod() = default; // Virtual destructor for interface
};

class IFactoryMethodA : public IFactoryMethod
{
  public:
    void execute() const override { cout << "IFactoryMethodA\n"; }
};

class IFactoryMethodB : public IFactoryMethod
{
  public:
    void execute() const override { cout << "IFactoryMethodB\n"; }
};

class Creator
{
  public:
    virtual ~Creator() = default; // Virtual destructor for base class
    virtual unique_ptr<IFactoryMethod> createMethod() const = 0;
    void doSomething() const
    {
        auto method = createMethod();
        method->execute();
    }
};

class CreatorA : public Creator
{
  public:
    unique_ptr<IFactoryMethod> createMethod() const override
    {
        return make_unique<IFactoryMethodA>();
    }
};

class CreatorB : public Creator
{
  public:
    unique_ptr<IFactoryMethod> createMethod() const override
    {
        return make_unique<IFactoryMethodB>();
    }
};

void client_code(const vector<unique_ptr<Creator>> &creators)
{
    for (auto &&creator : creators) {
        creator->doSomething();
    }
}

int main()
{
    vector<unique_ptr<Creator>> creators;

    creators.push_back(make_unique<CreatorA>());
    creators.push_back(make_unique<CreatorB>());

    client_code(creators);

    return 0;
}