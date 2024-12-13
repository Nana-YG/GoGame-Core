//
// Created by Yunyi Gao on 12/12/24.
//

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

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
    void init(int size);								// Initialize Go Board
    bool legal(int row, int col, StonePosition * pos);	// Is a move at <row, col> legal
    void move(StonePosition * pos, spot_color color);	// Play a move of <color> at <*pos>
    int getSize();										// Board size
    bool isOver();										// Game is over
    std::string showBoard();

};



#endif //BOARD_H
