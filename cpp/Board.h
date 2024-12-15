//
// Created by Yunyi Gao on 12/12/24.
//

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

class Game;

/**
 * Represents the position of a stone on the board.
 */
struct StonePosition {
  int row;
  int col;
};

/**
 * Enum to represent the state of a position on the board.
 */
typedef enum {
  WHITE = 1,
  EMPTY = 0,
  BLACK = -1
} spot_color;

/**
 * Class representing a board for a two-player game.
 */
class Board {

private:
    std::vector<std::vector<spot_color>> board;
    int size;
    bool gameOver;

public:
    Board();
    void init(int size);								// Initialize Go Board
    bool move(Game* game, StonePosition * pos, spot_color color);	// Play a move of <color> at <*pos>, return 1 if proceeded, return 0 if illegal
    bool legal(StonePosition *pos, spot_color color);	// Is a move at <row, col> legal
    Board update(StonePosition* pos, spot_color color); // Update the board for captures, liberties, etc.
    int getSize();										// Board size
    bool isOver();										// Game is over
    std::string showBoard();                            // Print the board to a String
    bool equalsTo(Board boardToCheck);                  // Check is this->board is equal to boardToCheck->board

};



#endif //BOARD_H
