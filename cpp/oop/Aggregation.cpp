#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Professor
{
  private:
    std::string m_name;
    std::string m_subject;

  public:
    Professor(std::string name, std::string subject)
        : m_name(std::move(name)), m_subject(std::move(subject))
    {
    }
    void teach() const
    {
        std::cout << std::format("{} is teaching {}.\n", m_name, m_subject);
    }
    [[nodiscard]] std::string_view getName() const noexcept { return m_name; }
};

class University
{
  private:
    std::string m_universityName;
    std::vector<std::shared_ptr<Professor>> m_professors;

  public:
    explicit University(std::string name) : m_universityName(std::move(name)) {}

    void addProfessor(std::shared_ptr<Professor> professor)
    {
        m_professors.push_back(std::move(professor));
    }

    void showProfessors() const
    {
        std::cout << std::format("Professors at {}.\n", m_universityName);
        for (const auto &professor : m_professors) {
            std::cout << std::format(" - {}.\n", professor->getName());
        }
    }
};

int main()
{
    auto prof1 = std::make_shared<Professor>("Dr. Smith", "Computer Science");
    auto prof2 = std::make_shared<Professor>("Dr. Johnson", "Mathematics");

    University university("Harvard University");
    university.addProfessor(prof1);
    university.addProfessor(prof2);

    university.showProfessors();

    // Professors can exist independently
    prof1->teach();
    prof2->teach();

    return 0;
}