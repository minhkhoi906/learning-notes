#include <iostream>
#include <memory>
#include <stdexcept> // For runtime_error
#include <vector>

// IFactoryMethod: The Product interface
class IFactoryMethod
{
  public:
    virtual void execute() const = 0;
    virtual ~IFactoryMethod() = default;
};

// IFactoryMethodA: A Concrete Product
class IFactoryMethodA final : public IFactoryMethod
{
  public:
    void execute() const override { std::cout << "Executing IFactoryMethodA\n"; }
};

// IFactoryMethodB: Another Concrete Product
class IFactoryMethodB final : public IFactoryMethod
{
  public:
    void execute() const override { std::cout << "Executing IFactoryMethodB\n"; }
};

// Creator: The Creator base class
class Creator
{
  public:
    virtual ~Creator() = default;

    // The Factory Method: returns a unique_ptr to an object
    virtual std::unique_ptr<IFactoryMethod> createMethod() const = 0;

    // The main logic that uses the factory method
    void doSomething() const
    {
        // The creator uses the factory method to create a product,
        // but it doesn't need to know the concrete class.
        std::unique_ptr<IFactoryMethod> method = this->createMethod();
        method->execute();
    }
};

// CreatorA: A Concrete Creator that produces IFactoryMethodA
class CreatorA final : public Creator
{
  public:
    // Using 'override' and 'final' for clarity and safety.
    std::unique_ptr<IFactoryMethod> createMethod() const override final
    {
        return std::make_unique<IFactoryMethodA>();
    }
};

// CreatorB: A Concrete Creator that produces IFactoryMethodB
class CreatorB final : public Creator
{
  public:
    // Using 'override' and 'final' for clarity and safety.
    std::unique_ptr<IFactoryMethod> createMethod() const override final
    {
        return std::make_unique<IFactoryMethodB>();
    }
};

// A more practical Factory Function.
// This function acts as a centralized factory for creating different Creator types.
enum class CreatorType {
    A,
    B,
    // Add more types as needed
};

// A helper function to create a concrete Creator based on an enum
std::unique_ptr<Creator> createCreator(CreatorType type)
{
    switch (type) {
    case CreatorType::A:
        return std::make_unique<CreatorA>();
    case CreatorType::B:
        return std::make_unique<CreatorB>();
    default:
        throw std::runtime_error("Invalid CreatorType");
    }
}

// Client code that works with any Creator
void client_code(const std::vector<std::unique_ptr<Creator>> &creators)
{
    std::cout << "Client: I'm not aware of the concrete creator's class.\n";
    for (const auto &creator : creators) {
        creator->doSomething();
    }
}

int main()
{
    // The main function can now use a clean, centralized way to create objects
    std::vector<std::unique_ptr<Creator>> creators;

    // Use the factory function to populate the vector
    creators.push_back(createCreator(CreatorType::A));
    creators.push_back(createCreator(CreatorType::B));

    client_code(creators);

    // Example of using the factory function directly
    std::cout << "\nClient: I'm now creating a single instance of a Creator.\n";
    std::unique_ptr<Creator> singleCreator = createCreator(CreatorType::A);
    singleCreator->doSomething();

    return 0;
}
