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

    Player(const std::string& playerName, spot_color playerColor);  // Constructor
    std::string getName() const;                                    // Getter for the player's name
    spot_color getColor() const;                                    // Getter for the player's color
    bool makeMove(Game* game, Board& board, StonePosition pos);     // Make a move on <pos>
};

#endif //PLAYER_H
