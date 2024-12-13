//
// Created by Yunyi Gao on 12/12/24.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "Board.h"

class Player {
private:
    std::string name;         // Player's name
    spot_color color;         // Player's stone color

public:
    // Constructor
    Player(const std::string& playerName, spot_color playerColor);

    // Getter for the player's name
    std::string getName() const;

    // Getter for the player's color
    spot_color getColor() const;

    // Make a move on the board
    bool makeMove(Game* game, Board& board, StonePosition pos);
};




#endif //PLAYER_H
