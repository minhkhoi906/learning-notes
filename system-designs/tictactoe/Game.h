#ifndef GAME_HPP
#define GAME_HPP

#include "Board.h"
#include "Player.h"

#include <memory>
#include <utility> // For std::pair

class Game
{
  private:
    Board m_board;

    // Use std::unique_ptr for automatic memory management.
    std::unique_ptr<Player> p_player1;
    std::unique_ptr<Player> p_player2;

    // This pointer doesn't own the object, so a raw pointer is fine here.
    Player *p_currentPlayer;

    bool m_gameOver;

  public:
    Game(int boardSize = 3);

    // No need for a custom destructor when using smart pointers.
    // The default destructor will handle cleanup automatically.
    ~Game() = default;

    void play();
    void makeMove(int row, int col);
    void switchPlayer();
    void displayResult() const;
    bool isGameOver() const;
    Player *getCurrentPlayer() const;

  private:
    void computerMove();
    std::pair<int, int> findBestMove() const;

    // Updated to take a Board object by value to maintain const correctness.
    int minimax(Board board, bool isMax, int depth) const;
};

#endif
