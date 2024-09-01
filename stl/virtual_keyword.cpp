#include <iostream>

using namespace std;

class Based
{
  public:
    Based() { cout << "Based ctor.\n"; }

    virtual ~Based() { cout << "Based dtor.\n"; }
};

class Derived : public Based
{
  public:
    Derived() { cout << "Derived ctor.\n"; }

    ~Derived() { cout << "Derived dtor.\n"; }
};

int main()
{
    Based *b = new Derived();
    delete b;
    return 0;
}