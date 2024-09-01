#include <iostream>
#include <memory>

using namespace std;

class Strategy
{
  public:
    virtual void execute() const = 0;
    virtual ~Strategy() = default;
};

class Context
{
  public:
    void setStrategy(unique_ptr<Strategy> strategy)
    {
        _strategy = std::move(strategy);
    }

    void executeStrategy() const
    {
        if (_strategy) {
            _strategy->execute();
        }
    }

  private:
    unique_ptr<Strategy> _strategy{};
};

class StrategyA : public Strategy
{
  public:
    void execute() const override { cout << "Executing StrategyA\n"; }
};

class StrategyB : public Strategy
{
  public:
    void execute() const override { cout << "Executing StrategyB\n"; }
};

void client_code()
{
    Context context;

    context.setStrategy(make_unique<StrategyA>());
    context.executeStrategy();

    context.setStrategy(make_unique<StrategyB>());
    context.executeStrategy();
}

int main()
{
    client_code();
    return 0;
}
