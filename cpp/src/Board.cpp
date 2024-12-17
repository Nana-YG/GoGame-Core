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

void Board::clear() {
    clearGroups();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j]->color = EMPTY;
        }
    }
}

void Board::clearGroups() {
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
    std::vector<std::vector<spot_color>> oldState = boardMatrix();
    this->update(pos, color);
    if (game->superko(*this)) {
        readBoardFromMatrix(oldState);
        return false;
    }
    return true;
}

bool Board::legal(StonePosition *pos, spot_color color) {

    // Check range
    if (!isInBounds(pos->row, pos->col)) {
        return false;
    }

    // Check if the spot is empty
    if (this->board[pos->row][pos->col]->color != EMPTY) {
        return false;
    }

    // Self-kill

    return true;
}

void Board::update(StonePosition *pos, spot_color color) {

    board[pos->row][pos->col]->color = color;

    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    // Check its four neighbors
    for (int d = 0; d < 4; d++) {
        int ni = pos->row + dx[d];
        int nj = pos->col + dy[d];

        std::unordered_set<Group*> visitedGroups;
        // Ensure neighbor is within bounds
        if (isInBounds(ni, nj)) {
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
    groupStones();
    countLiberties();

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

void Board::groupStones() {
    clearGroups();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j]->color == EMPTY) continue;

            Group* newGroup = createNewGroup(i, j);
            addGroup(newGroup);

            // Check neighbors and combine groups
            checkAndCombineGroups(i, j, i - 1, j); // Check above
            checkAndCombineGroups(i, j, i, j - 1); // Check left
        }
    }
}

// Helper function to combine groups if colors match
void Board::checkAndCombineGroups(int x1, int y1, int x2, int y2) {
    if (isInBounds(x2, y2) && board[x1][y1]->color == board[x2][y2]->color) {
        Group* group1 = board[x1][y1]->group;
        Group* group2 = board[x2][y2]->group;

        if (group1 != group2) {
            Group* combinedGroup = combined(group1, group2, 2);
            removeGroup(group1);
            removeGroup(group2);
            addGroup(combinedGroup);
        }
    }
}

// Helper function: Update liberties for a given empty space
void Board::updateLibertiesForEmptySpace(int x, int y) {
    // Directions: up, down, left, right
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    std::unordered_set<Group*> visitedGroups; // Track already processed groups

    for (int d = 0; d < 4; d++) {
        int ni = x + dx[d];
        int nj = y + dy[d];

        if (!isInBounds(ni, nj)) continue; // Boundary check

        if (board[ni][nj]->color != EMPTY) {
            Group* neighborGroup = board[ni][nj]->group;

            // Increase liberty count only once per group
            if (visitedGroups.insert(neighborGroup).second) {
                neighborGroup->liberty++;
            }
        }
    }
}

// Helper function: Check if coordinates are within board boundaries
bool Board::isInBounds(int x, int y) const {
    return x >= 0 && x < size && y >= 0 && y < size;
}

// Main function: Calculate the liberties for all groups
void Board::countLiberties() {
    clearLiberties(); // Reset liberty count for all groups

    // Traverse the board to find empty spaces and update liberties
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j]->color == EMPTY) {
                updateLibertiesForEmptySpace(i, j);
            }
        }
    }
}

// Helper function: Reset liberty count for all groups
void Board::clearLiberties() {
    for (Group* group : whiteGroups) {
        group->liberty = 0;
    }
    for (Group* group : blackGroups) {
        group->liberty = 0;
    }
}

std::string Board::cellToString(Stone* stone) const {
    switch (stone->color) {
        case WHITE: return "O ";
        case BLACK: return "X ";
        case EMPTY: default: return ". ";
    }
}


std::string Board::showBoard() {

    std::ostringstream oss;

    // Print column mark: GTP format first line
    oss << "   "; // White space padding
    for (int i = 0; i < this->size; i++) {
        char colMark = 'A' + i;
        oss << colMark << " ";
    }
    oss << "\n";

    // Print rows
    for (int i = size - 1; i >= 0; i--) {
        oss << (i + 1) << (i + 1 < 10 ? "  " : " ");
        for (int j = 0; j < size; j++) {
            oss << cellToString(board[i][j]);
        }
        oss << "\n";
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

std::vector<std::vector<spot_color>> Board::boardMatrix() {

    std::vector<std::vector<spot_color>> result(size, std::vector<spot_color>(size)); // Initialize a matrix

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = board[i][j]->color; // Copy color from board
        }
    }

    return result;
}


void Board::readBoardFromMatrix(const std::vector<std::vector<spot_color>> matrix) {
    if (matrix.size() != size || matrix[0].size() != size) {
        throw std::invalid_argument("Matrix dimensions do not match the board size.");
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j]->color = matrix[i][j]; // Update each stone's color
        }
    }

    // Recalculate groups and liberties after updating the board
    groupStones();
    countLiberties();
}

void Board::readBoardFromString(const std::string boardString) {
    std::istringstream iss(boardString);
    std::string line;
    int rowIndex = size - 1; // Start from the last row (size-1)

    while (std::getline(iss, line)) {
        // Skip the first line with column markers
        if (line.empty() || line[0] == ' ') {
            continue;
        }

        std::istringstream lineStream(line);
        int rowLabel;
        lineStream >> rowLabel; // Read the row label (e.g., 10, 9, ...)

        for (int colIndex = 0; colIndex < size; colIndex++) {
            char cell;
            lineStream >> cell; // Read each cell in the row

            // Update the board based on the character
            switch (cell) {
                case 'X':
                    board[rowIndex][colIndex]->color = BLACK;
                break;
                case 'O':
                    board[rowIndex][colIndex]->color = WHITE;
                break;
                case '.':
                    board[rowIndex][colIndex]->color = EMPTY;
                break;
                default:
                    throw std::invalid_argument("Invalid boardString format.");
            }
        }
        rowIndex--; // Move to the next row
    }
}


std::vector<std::vector<int>> Board::libertyMatrix() {
    // Initialize a 2D vector with the same dimensions as the board
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));

    // Traverse the board
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // If the position is not empty, get the group's liberty
            if (board[i][j]->color != EMPTY) {
                result[i][j] = board[i][j]->group->liberty;
            }
        }
    }
    return result;
}

