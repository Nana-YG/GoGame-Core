//
// Created by Yunyi Gao on 12/12/24.
//

#include "Board.h"
#include "Game.h"
#include "Config.h"
#include "GroupUtil.h"
#include <sstream>
#include <string>
#include <unordered_set>

Board::Board() {
    const Config& config = Config::getInstance();
    this->init(config.boardSize);
}

// Board::~Board() {
//     for (int i = 0; i < size; ++i) {
//         for (int j = 0; j < size; ++j) {
//             delete this->board[i][j];
//         }
//     }
// }

void Board::init(int size) {
    this->size = size;
    this->gameOver = false;
    this->board.resize(size, std::vector<Stone*>(size, nullptr));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->board[i][j] = new Stone();
        }
    }
}


void Board::clearGroup() {
    std::unordered_set<Group*> uniqueGroups;

    // Collect all unique groups
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j]->group != nullptr) {
                uniqueGroups.insert(board[i][j]->group);
                board[i][j]->group = nullptr; // Clear the group pointer
            }
        }
    }

    // Delete all unique groups
    for (Group* group : uniqueGroups) {
        delete group;
    }

    // Clear group containers
    whiteGroups.clear();
    blackGroups.clear();
}

/**
 * Defines '-' operator of StonePosition
 * Distance between two positions
 */
StonePosition operator-(const StonePosition& pos1, const StonePosition& pos2) {
    return {(pos1.row - pos2.row), (pos1.col - pos2.col)};
}

bool Board::move(Game* game, StonePosition *pos, spot_color color) {
    if (!this->legal(pos, color)) {
        return false;
    }
    Board newBoard = this->update(pos, color);
    if (game->superko(newBoard)) {
        return false;
    }
    this->board = newBoard.board;
    return true;
}

bool Board::legal(StonePosition *pos, spot_color color) {
    // Check range
    if (pos->row < 0 || pos->row >= this->size || pos->col < 0 || pos->col >= this->size) {
        return false;
    }

    // Check if the spot is empty
    if (this->board[pos->row][pos->col]->color != EMPTY) {
        return false;
    }

    // TODO: 可扩展检查逻辑，如全局同型规则等
    return true;
}

Board Board::update(StonePosition *pos, spot_color color) {
    Board newBoard(*this);
    newBoard.board[pos->row][pos->col]->color = color;

    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    // Check its four neighbors
    for (int d = 0; d < 4; d++) {
        int ni = pos->row + dx[d];
        int nj = pos->col + dy[d];

        std::unordered_set<Group*> visitedGroups;
        // Ensure neighbor is within bounds
        if (ni >= 0 && ni < size && nj >= 0 && nj < size) {
            // Capture and subtract: Check if the neighbor belongs to the opponent group
            if (board[ni][nj]->color != EMPTY && board[ni][nj]->color != board[pos->row][pos->col]->color) {
                Group* neighborGroup = board[ni][nj]->group;

                // Only decrease liberty if this group hasn't been visited
                if (visitedGroups.find(neighborGroup) == visitedGroups.end()) {
                    neighborGroup->liberty--;
                    visitedGroups.insert(neighborGroup);
                }
                // Remove captured stones if liberty decreases to 0
                if (neighborGroup->liberty == 0) {
                    for (Stone* stone : neighborGroup->stones) {
                        stone->color = EMPTY;
                        stone->group = nullptr;
                    }
                    removeGroup(neighborGroup);
                }
            }
        }
    }
    group();
    countLiberties();

    return newBoard;
}

int Board::getSize() {
    return size;
}

bool Board::isOver() {
    return gameOver;
}

std::vector<std::vector<Stone*>> Board::getBoard() {
    return board;
}

Group* Board::createNewGroup(int row, int col) {
    Group* newGroup = new Group();
    newGroup->liberty = 0;
    newGroup->color = board[row][col]->color;

    board[row][col]->group = newGroup;

    newGroup->stones.push_back(board[row][col]);

    if (newGroup->color == WHITE) {
        whiteGroups.push_back(newGroup);
    } else if (newGroup->color == BLACK) {
        blackGroups.push_back(newGroup);
    }

    return newGroup;
}

void Board::removeGroup(Group* inputGroup) {
    if (inputGroup->color == WHITE) {
        for (auto it = whiteGroups.begin(); it != whiteGroups.end(); ++it) {
            if (*it == inputGroup) {
                whiteGroups.erase(it);
                break;
            }
        }
    } else if (inputGroup->color == BLACK) {
        for (auto it = blackGroups.begin(); it != blackGroups.end(); ++it) {
            if (*it == inputGroup) {
                blackGroups.erase(it);
                break;
            }
        }
    }
}

void Board::addGroup(Group* group) {
    if (group->color == WHITE) {
        whiteGroups.push_back(group);
        return;
    }
    if (group->color == BLACK) {
        blackGroups.push_back(group);
        return;
    }
    return;
}

void Board::group() {
    clearGroup();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j]->color == EMPTY) {
                continue;
            }
            Group* newGroup = createNewGroup(i, j);
            addGroup(newGroup);
            if (i > 0) {
                if (board[i][j]->color == board[i - 1][j]->color) {
                    Group* toRemove1 = board[i][j]->group;
                    Group* toRemove2 = board[i - 1][j]->group;
                    Group* combinedGroup = combined(board[i][j]->group, board[i - 1][j]->group, 2);
                    removeGroup(toRemove1);
                    removeGroup(toRemove2);
                    addGroup(combinedGroup);
                }
            }
            if (j > 0) {
                if (board[i][j]->color == board[i][j - 1]->color) {
                    Group* toRemove1 = board[i][j]->group;
                    Group* toRemove2 = board[i][j - 1]->group;
                    Group* combinedGroup = combined(board[i][j]->group, board[i][j - 1]->group, 2);
                    removeGroup(toRemove1);
                    removeGroup(toRemove2);
                    addGroup(combinedGroup);
                }
            }
        }
    }
}

void Board::countLiberties() {
    // Clear the liberties of all groups first
    for (Group* group : whiteGroups) {
        group->liberty = 0;
    }
    for (Group* group : blackGroups) {
        group->liberty = 0;
    }

    // Directions for checking neighbors (up, down, left, right)
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    // Traverse the board
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Check if the cell is empty
            if (board[i][j]->color == EMPTY) {
                // Use a set to track the groups around this empty space
                std::unordered_set<Group*> visitedGroups;

                // Check its four neighbors
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];

                    // Ensure neighbor is within bounds
                    if (ni >= 0 && ni < size && nj >= 0 && nj < size) {
                        // Check if the neighbor belongs to a group
                        if (board[ni][nj]->color != EMPTY) {
                            Group* neighborGroup = board[ni][nj]->group;

                            // Only increase liberty if this group hasn't been visited
                            if (visitedGroups.find(neighborGroup) == visitedGroups.end()) {
                                neighborGroup->liberty++;
                                visitedGroups.insert(neighborGroup);
                            }
                        }
                    }
                }
            }
        }
    }
}

std::string Board::showBoard() {

    std::ostringstream oss;

    // Column mark: GTP format first line
    oss << "   "; // White space padding
    for (int i = 0; i < this->size; i++) {
        char colMark = 'A' + i;
        oss << colMark << " ";
    }
    oss << "\n";

    // Iterate the entire board
    for (int i = (size - 1); i >= 0; i--) {
        oss << (i + 1) << " ";
        if (i + 1 < 10) oss << " "; // If <row number> < 10, add on more space
        for (int j = 0; j < size; j++) {
            switch (board[i][j]->color) {
                case WHITE:
                    oss << "O "; // White
                break;
                case BLACK:
                    oss << "X "; // Black
                break;
                case EMPTY:
                    default:
                        oss << ". "; // Empty
                break;
            }
        }
        oss << "\n"; // Next line
    }
    return oss.str();
}

std::string Board::showLiberties() {
    std::ostringstream oss;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j]->color == EMPTY) {
                oss << ". "; // Empty point
            } else {
                // Print the group's liberty count
                oss << board[i][j]->group->liberty << " ";
            }
        }
        oss << std::endl; // Newline for the next row
    }
    return oss.str(); // Return the generated string
}



bool Board::equalsTo(Board boardToCheck) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (this->board[i][j]->color != boardToCheck.board[i][j]->color) {
                return false;
            }
        }
    }
    return true;
}
