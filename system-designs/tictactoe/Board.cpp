#include "Board.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

Board::Board(std::size_t size)
    : m_size(size), m_grid(size, std::vector<char>(size, EMPTY_CELL))
{
    // Constructor body is now empty - initialization done in member initializer list
}

std::size_t Board::getSize() const noexcept { return m_size; }

char Board::getCell(std::size_t row, std::size_t col) const
{
    if (!isValidPosition(row, col)) {
        throw std::out_of_range("Invalid board position");
    }
    return m_grid[row][col];
}

char Board::getEmpty() const noexcept { return EMPTY_CELL; }

bool Board::isEmpty(std::size_t row, std::size_t col) const
{
    return isValidPosition(row, col) && m_grid[row][col] == EMPTY_CELL;
}

bool Board::isValidPosition(std::size_t row, std::size_t col) const noexcept
{
    // size_t is unsigned, so no need to check >= 0
    return row < m_size && col < m_size;
}

bool Board::makeMove(std::size_t row, std::size_t col, char symbol)
{
    if (!isValidPosition(row, col) || !isEmpty(row, col)) {
        return false;
    }
    m_grid[row][col] = symbol;
    return true;
}

bool Board::isFull() const noexcept
{
    return std::all_of(m_grid.cbegin(), m_grid.cend(), [](const auto &row) {
        return std::none_of(row.cbegin(), row.cend(),
                            [](char cell) { return cell == EMPTY_CELL; });
    });
}

bool Board::checkWin(char symbol) const noexcept
{
    return checkRows(symbol) || checkColumns(symbol) || checkDiagonals(symbol);
}

void Board::display() const
{
    std::cout << "\nCurrent Board:\n";
    for (const auto &row : m_grid) {
        for (char cell : row) {
            std::cout << std::setw(3) << cell;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void Board::reset() noexcept
{
    for (auto &row : m_grid) {
        std::fill(row.begin(), row.end(), EMPTY_CELL);
    }
}

bool Board::checkRows(char symbol) const noexcept
{
    return std::any_of(m_grid.cbegin(), m_grid.cend(), [symbol](const auto &row) {
        return std::all_of(row.cbegin(), row.cend(),
                           [symbol](char cell) { return cell == symbol; });
    });
}

bool Board::checkColumns(char symbol) const noexcept
{
    for (std::size_t col = 0; col < m_size; ++col) {
        if (std::all_of(
                m_grid.cbegin(), m_grid.cend(),
                [col, symbol](const auto &row) { return row[col] == symbol; })) {
            return true;
        }
    }
    return false;
}

bool Board::checkDiagonals(char symbol) const noexcept
{
    // Check main diagonal (top-left to bottom-right)
    const bool main_diagonal = [this, symbol]() {
        for (std::size_t i = 0; i < m_size; ++i) {
            if (m_grid[i][i] != symbol)
                return false;
        }
        return true;
    }();

    if (main_diagonal)
        return true;

    // Check anti-diagonal (top-right to bottom-left)
    for (std::size_t i = 0; i < m_size; ++i) {
        if (m_grid[i][m_size - 1 - i] != symbol) {
            return false;
        }
    }
    return true;
}