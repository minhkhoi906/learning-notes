// Car.cpp – modern-C++ version
#include <format> // C++20
#include <iostream>
#include <memory>
#include <string_view>
#include <utility>

// ─────────────────────────────────────── //
//               Components               //
// ─────────────────────────────────────── //

class Engine final
{
    int horsepower_{};

  public:
    explicit Engine(int hp) noexcept : horsepower_{hp} {}

    void start() const
    {
        std::cout << std::format("Engine started with {} HP.\n", horsepower_);
    }
};

enum class WheelType { Alloy, Steel, Carbon };

constexpr std::string_view to_string(WheelType wt)
{
    using enum WheelType;
    switch (wt) {
    case Alloy:
        return "Alloy";
    case Steel:
        return "Steel";
    case Carbon:
        return "Carbon";
    }
    return "Unknown";
}

class Wheel final
{
    WheelType type_{WheelType::Alloy};

  public:
    explicit Wheel(WheelType t) noexcept : type_{t} {}

    void rotate() const
    {
        std::cout << std::format("The {} wheel is rotating.\n", to_string(type_));
    }
};

enum class TransmissionType { Automatic, Manual, CVT };

constexpr std::string_view to_string(TransmissionType t)
{
    using enum TransmissionType;
    switch (t) {
    case Automatic:
        return "Automatic";
    case Manual:
        return "Manual";
    case CVT:
        return "CVT";
    }
    return "Unknown";
}

class Transmission final
{
    TransmissionType type_{TransmissionType::Automatic};

  public:
    explicit Transmission(TransmissionType t) noexcept : type_{t} {}

    void shift_gear() const
    {
        std::cout << std::format("Transmission shifted: {}\n", to_string(type_));
    }
};

// ─────────────────────────────────────── //
//                  Car                   //
// ─────────────────────────────────────── //

class Car
{
    std::unique_ptr<Engine> engine_;
    std::unique_ptr<Wheel> wheel_;
    std::unique_ptr<Transmission> transmission_;

  public:
    Car(std::unique_ptr<Engine> e,
        std::unique_ptr<Wheel> w,
        std::unique_ptr<Transmission> t) noexcept
        : engine_{std::move(e)}, wheel_{std::move(w)}, transmission_{std::move(t)}
    {
    }

    /// High-level façade
    void drive() const
    {
        engine_->start();
        wheel_->rotate();
        transmission_->shift_gear();
        std::puts("Car is moving!");
    }
};

// ─────────────────────────────────────── //
//                 main()                 //
// ─────────────────────────────────────── //

int main()
{
    auto engine = std::make_unique<Engine>(150);
    auto wheel = std::make_unique<Wheel>(WheelType::Alloy);
    auto transmission = std::make_unique<Transmission>(TransmissionType::Automatic);

    Car car{std::move(engine), std::move(wheel), std::move(transmission)};
    car.drive();
}
