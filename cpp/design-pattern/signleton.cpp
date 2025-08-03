#include <iostream>
#include <memory>

using namespace std;

class Singleton
{
  public:
    static Singleton &getInstance()
    {
        static Singleton instance;
        return instance;
    }

    void doSomething() { cout << "ObjID: " << _id << " - Doing something...\n"; }

  private:
    Singleton() : _id(1) {}
    int _id;
};

int main()
{
    auto obj1 = Singleton::getInstance();
    obj1.doSomething();

    auto obj2 = Singleton::getInstance();
    obj2.doSomething();
    obj1.doSomething();

    return 0;
}