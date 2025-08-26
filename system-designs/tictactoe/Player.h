#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player
{
  private:
    std::string m_name;
    char m_symbol;
    bool m_isHuman;

  public:
    Player(std::string name, char symbol, bool isHuman = true);

    std::string getName() const;
    char getSymbol() const;
    bool isHumanPlayer() const;
};

#endif