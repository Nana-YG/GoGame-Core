//
// Created by Yunyi Gao on 12/12/24.
//

#include "Board.h"
#include "Game.h"
#include "Config.h"
#include "GroupUtil.h"
#include <sstream>
#include <string>

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

void Board::clear() {

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
    newGroup->liberties = 0;
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

void Board::addGroup(Group* inputGroup) {
    if (inputGroup->color == WHITE) {
        whiteGroups.push_back(inputGroup);
        return;
    }
    if (inputGroup->color == BLACK) {
        blackGroups.push_back(inputGroup);
        return;
    }
    return;

}

void Board::group() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j]->color == EMPTY) {
                continue;
            }

            Group* newGroup = createNewGroup(i, j);

            if (i > 0) {
                if (board[i][j]->color == board[i - 1][j]->color) {
                    Group* combinedGroup = combined(board[i][j]->group, board[i - 1][j]->group, 2);
                    removeGroup(board[i][j]->group);
                    removeGroup(board[i - 1][j]->group);
                    addGroup(combinedGroup);
                }
            }
            if (j > 0) {
                if (board[i][j]->color == board[i][j - 1]->color) {
                    Group* combinedGroup = combined(board[i][j]->group, board[i][j - 1]->group, 2);
                    removeGroup(board[i][j]->group);
                    removeGroup(board[i][j - 1]->group);
                    addGroup(combinedGroup);
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
