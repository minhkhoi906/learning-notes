#include "Player.h"

Player::Player(std::string name, char symbol, bool isHuman)
    : m_name(name), m_symbol(symbol), m_isHuman(isHuman)
{
}

std::string Player::getName() const { return m_name; }
char Player::getSymbol() const { return m_symbol; }
bool Player::isHumanPlayer() const { return m_isHuman; }