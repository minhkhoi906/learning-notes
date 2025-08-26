#include "Board.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

Board::Board(int size) : m_size(size), m_EMPTY('-') { reset(); }

int Board::getSize() const { return m_size; }

char Board::getCell(int row, int col) const { return m_grid[row][col]; }

char Board::getEMPTY() const { return m_EMPTY; }

bool Board::isEmpty(int row, int col) const { return m_grid[row][col] == m_EMPTY; }

bool Board::isValidPosition(int row, int col) const
{
    return row >= 0 && row < m_size && col >= 0 && col < m_size;
}

bool Board::makeMove(int row, int col, char symbol)
{
    if (!isValidPosition(row, col) || !isEmpty(row, col)) {
        return false;
    }
    m_grid[row][col] = symbol;
    return true;
}

bool Board::isFull() const
{
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            if (isEmpty(i, j))
                return false;
        }
    }
    return true;
}

bool Board::checkWin(char symbol) const
{
    return checkRows(symbol) || checkColumns(symbol) || checkDiagonals(symbol);
}

void Board::display() const
{
    std::cout << "\nCurrent Board:" << std::endl;
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            std::cout << std::setw(3) << m_grid[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Board::reset() { m_grid.resize(m_size, std::vector<char>(m_size, m_EMPTY)); }

bool Board::checkRows(char symbol) const
{
    for (int i = 0; i < m_size; i++) {
        bool win = true;
        for (int j = 0; j < m_size; j++) {
            if (m_grid[i][j] != symbol) {
                win = false;
                break;
            }
        }
        if (win)
            return true;
    }
    return false;
}

bool Board::checkColumns(char symbol) const
{
    for (int j = 0; j < m_size; j++) {
        bool win = true;
        for (int i = 0; i < m_size; i++) {
            if (m_grid[i][j] != symbol) {
                win = false;
                break;
            }
        }
        if (win)
            return true;
    }
    return false;
}

bool Board::checkDiagonals(char symbol) const
{
    // Check main diagonal
    bool win = true;
    for (int i = 0; i < m_size; i++) {
        if (m_grid[i][i] != symbol) {
            win = false;
            break;
        }
    }
    if (win)
        return true;

    // Check other diagonal
    win = true;
    for (int i = 0; i < m_size; i++) {
        if (m_grid[i][m_size - 1 - i] != symbol) {
            win = false;
            break;
        }
    }
    return win;
}