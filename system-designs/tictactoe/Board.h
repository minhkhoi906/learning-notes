#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>

// The Board class represents a game board, like for Tic-Tac-Toe.
class Board
{
  public:
    // Explicit constructor to prevent implicit conversions.
    explicit Board(int size = 3);

    // Modern C++ best practice: The Rule of Zero.
    // std::vector handles copying, moving, and destruction, so we
    // don't need to declare our own special member functions.
    // The compiler-generated ones are sufficient.

    // Accessors for class properties
    int getSize() const;
    char getCell(int row, int col) const;
    char getEMPTY() const;

    // Check state of the board or a specific cell
    bool isEmpty(int row, int col) const;
    bool isValidPosition(int row, int col) const;
    bool isFull() const;

    // Mutator for making a move
    bool makeMove(int row, int col, char symbol);

    // Win condition check
    bool checkWin(char symbol) const;

    // Board management
    void display() const;
    void reset();

  private:
    // Member variables
    std::vector<std::vector<char>> m_grid;
    const int m_size;
    const char m_EMPTY;

    // Private helper methods for checking win conditions
    bool checkRows(char symbol) const;
    bool checkColumns(char symbol) const;
    bool checkDiagonals(char symbol) const;
};

#endif
