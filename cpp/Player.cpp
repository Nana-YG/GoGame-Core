//
// Created by Yunyi Gao on 12/12/24.
//
#include "Player.h"
#include <iostream>

// Constructor
Player::Player(const std::string& playerName, spot_color playerColor)
    : name(playerName), color(playerColor) {}

// Getter for the player's name
std::string Player::getName() const {
    return name;
}

// Getter for the player's color
spot_color Player::getColor() const {
    return color;
}

// Make a move on the board
bool Player::makeMove(Game* game, Board& board, StonePosition pos) {
    if (!board.legal(&pos, color)) {
        std::cout << "Move is illegal. Try again, " << name << "!" << std::endl;
        return false;
    }

    if (board.move(game, &pos, color)) {
        std::cout << name << " placed a stone at (" << pos.row << ", " << pos.col << ")." << std::endl;
        return true;
    }

    std::cout << "Move failed due to game rule: SuperKo" << std::endl;
    return false;
}
