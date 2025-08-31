#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>

class Board
{
  public:
    explicit Board(std::size_t size);

    // Accessors for class properties
    [[nodiscard]] std::size_t getSize() const noexcept;
    [[nodiscard]] char getCell(std::size_t row, std::size_t col) const;
    [[nodiscard]] char getEmpty() const noexcept;

    // State queries
    [[nodiscard]] bool isEmpty(std::size_t row, std::size_t col) const;
    [[nodiscard]] bool isValidPosition(std::size_t row,
                                       std::size_t col) const noexcept;
    [[nodiscard]] bool isFull() const noexcept;
    [[nodiscard]] bool checkWin(char symbol) const noexcept;

    // Mutators
    bool makeMove(std::size_t row, std::size_t col, char symbol);
    void reset() noexcept;
    void display() const;

  private:
    static constexpr char EMPTY_CELL = '-';

    std::size_t m_size;
    std::vector<std::vector<char>> m_grid;

    // Helper methods
    [[nodiscard]] bool checkRows(char symbol) const noexcept;
    [[nodiscard]] bool checkColumns(char symbol) const noexcept;
    [[nodiscard]] bool checkDiagonals(char symbol) const noexcept;
};

#endif
