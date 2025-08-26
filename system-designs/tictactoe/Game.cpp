#include "Game.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <utility> // For std::pair and std::move

// Game class implementation
Game::Game(int boardSize) : m_board(boardSize), m_gameOver(false)
{
    // Use smart pointers to manage player objects, no manual new/delete.
    p_player1 = std::make_unique<Player>("Human", 'X');
    p_player2 = std::make_unique<Player>("Computer", 'O', false); // Computer player
    p_currentPlayer = p_player1.get();

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void Game::play()
{
    while (!m_gameOver) {
        m_board.display();

        if (p_currentPlayer->isHumanPlayer()) {
            int row, col;
            std::cout << p_currentPlayer->getName()
                      << "'s turn (symbol: " << p_currentPlayer->getSymbol()
                      << ")\n";
            std::cout << "Enter row (0-" << m_board.getSize() - 1 << "): ";

            // Check for valid input
            if (!(std::cin >> row)) {
                std::cout << "Invalid input! Please enter a number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            std::cout << "Enter column (0-" << m_board.getSize() - 1 << "): ";
            if (!(std::cin >> col)) {
                std::cout << "Invalid input! Please enter a number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            makeMove(row, col);
        } else {
            std::cout << "Computer's turn...\n";
            computerMove();
        }
    }
    displayResult();
}

void Game::makeMove(int row, int col)
{
    if (!m_board.isValidPosition(row, col) || !m_board.isEmpty(row, col)) {
        std::cout << "Invalid move! Please try again.\n";
        return;
    }

    m_board.makeMove(row, col, p_currentPlayer->getSymbol());

    if (m_board.checkWin(p_currentPlayer->getSymbol())) {
        m_gameOver = true;
        return;
    }

    if (m_board.isFull()) {
        m_gameOver = true;
        p_currentPlayer = nullptr;
        return;
    }

    switchPlayer();
}

void Game::switchPlayer()
{
    p_currentPlayer =
        (p_currentPlayer == p_player1.get()) ? p_player2.get() : p_player1.get();
}

void Game::displayResult() const
{
    m_board.display();
    if (p_currentPlayer) {
        std::cout << p_currentPlayer->getName() << " wins!\n";
    } else {
        std::cout << "It's a draw!\n";
    }
}

bool Game::isGameOver() const { return m_gameOver; }

Player *Game::getCurrentPlayer() const { return p_currentPlayer; }

void Game::computerMove()
{
    auto [row, col] = findBestMove();
    makeMove(row, col);
}

std::pair<int, int> Game::findBestMove() const
{
    int bestScore = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = {0, 0};

    // Iterate through all possible moves
    for (int i = 0; i < m_board.getSize(); ++i) {
        for (int j = 0; j < m_board.getSize(); ++j) {
            if (m_board.isEmpty(i, j)) {
                // A single copy of the board is passed to the minimax function.
                Board tempBoard = m_board;
                tempBoard.makeMove(i, j, p_player2->getSymbol());

                // The original code passed 'false' and '0' to minimax.
                // The current player's perspective is 'false' (minimizer) and
                // depth 1.
                int score = minimax(std::move(tempBoard), false, 1);

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {i, j};
                }
            }
        }
    }
    return bestMove;
}

int Game::minimax(Board board, bool isMax, int depth) const
{
    // Base cases with a copied board
    if (board.checkWin(p_player2->getSymbol()))
        return 10 - depth;
    if (board.checkWin(p_player1->getSymbol()))
        return depth - 10;
    if (board.isFull())
        return 0;

    int bestScore =
        isMax ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    for (int i = 0; i < board.getSize(); ++i) {
        for (int j = 0; j < board.getSize(); ++j) {
            if (board.isEmpty(i, j)) {
                board.makeMove(
                    i, j, isMax ? p_player2->getSymbol() : p_player1->getSymbol());
                int score = minimax(board, !isMax, depth + 1);

                // The original code was fine, just made it more explicit.
                bestScore =
                    isMax ? std::max(score, bestScore) : std::min(score, bestScore);

                // Undo the move to restore the board state for the next iteration.
                // Note: a more efficient way to do this is to add an 'undoMove'
                // function to the Board class.
                board.makeMove(i, j, m_board.getEMPTY());
            }
        }
    }
    return bestScore;
}
