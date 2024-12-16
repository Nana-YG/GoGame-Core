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

// Minus '-' operator of StonePosition
StonePosition operator-(const StonePosition &pos1, const StonePosition &pos2);

/**
 * Enum to represent the state of a position on the board.
 */
typedef enum {
    WHITE = 1,
    EMPTY = 0,
    BLACK = -1,
} spot_color;

struct Stone;
struct Group;

struct Group {
    spot_color color;
    int liberty;
    std::vector<Stone*> stones;
};

struct Stone {
    spot_color color = EMPTY;
    Group * group = nullptr;
    Stone() = default;
};


/**
 * Class representing a board for a two-player game.
 */
class Board {

private:
    std::vector<std::vector<Stone*>> board;
    int size;
    bool gameOver;
    std::vector<Group*> whiteGroups;
    std::vector<Group*> blackGroups;


public:
    Board();
    // ~Board();
    void init(int size);								// Initialize Go Board
    void clear();                                       // Clear the board and all data (Groups and stones)
    void clearGroups();                                  // Clear group-related data
    int getSize();										// Board size
    bool isOver();										// Game is over
    std::vector<std::vector<Stone*>> getBoard();        // Get the board
    bool move(Game* game, StonePosition * pos, spot_color color);	// Play a move of <color> at <*pos>, return 1 if proceeded, return 0 if illegal
    bool legal(StonePosition *pos, spot_color color);	// Is a move at <row, col> legal
    void update(StonePosition* pos, spot_color color); // Update the board for groups, captures, liberties, etc.
    Group* createNewGroup(int row, int col);            // Create a new group and add it into the list
    void removeGroup(Group* group);                     // Remove input group from group list
    void addGroup(Group* group);                        // Group add group to whiteGroups/blackGroups
    void groupStones();                                       // Group all stones on the board, DO NOT use for update, use update() instead. This is for manually assigned board.
    void checkAndCombineGroups(int x1, int y1, int x2, int y2); // Helper function: Combine groups if colors match
    void updateLibertiesForEmptySpace(int x, int y);    // Helper function: Update liberties for a given empty space
    void countLiberties();                              // Count the liberties of each group
    bool isInBounds(int x, int y) const;                // Helper function: Check if coordinates are within board boundaries
    void clearLiberties();                              // Helper function: Reset liberty count for all groups
    std::string cellToString(Stone* stone) const;       // Helper function: Stone ==> '.' 'X' 'O'
    std::string showBoard();                            // Print the board to a String
    std::string showLiberties();                        // Print the board in form of liberties
    bool equalsTo(Board boardToCheck);                  // Check is this->board is equal to boardToCheck->board
    std::vector<std::vector<spot_color>> boardMatrix();   // Output the board to matrix of (-1, 0, 1)
    void readBoardFromMatrix(std::vector<std::vector<spot_color>> inputMatrix); // Change the board based on the input matrix
    void readBoardFromString(std::string board);        // Change the board based on the input string
    std::vector<std::vector<int>> libertyMatrix();        // Show libertyMatrix


};



#endif //BOARD_H
