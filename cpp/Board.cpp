//
// Created by Yunyi Gao on 12/12/24.
//

#include "Board.h"
#include "Game.h"
#include "Config.h"
#include <sstream>
#include <string>

Board::Board() {
    const Config& config = Config::getInstance();
    this->init(config.boardSize);
}

void Board::init(int size) {
    this->size = size;
    this->gameOver = false;
    this->board.resize(size, std::vector<spot_color>(size, EMPTY));
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
    if (this->board[pos->row][pos->col] != EMPTY) {
        return false;
    }

    // TODO: 可扩展检查逻辑，如全局同型规则等
    return true;
}

Board Board::update(StonePosition *pos, spot_color color) {
    Board newBoard(*this);
    newBoard.board[pos->row][pos->col] = color;
    return newBoard;
}

int Board::getSize() {
    return size;
}

bool Board::isOver() {
    return gameOver;
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
    for (int i = 0; i < size; ++i) {
        oss << (i + 1) << " ";
        if (i + 1 < 10) oss << " "; // If <row number> < 10, add on more space
        for (int j = 0; j < size; ++j) {
            switch (board[i][j]) {
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
            if (this->board[i][j] != boardToCheck.board[i][j]) {
                return false;
            }
        }
    }
    return true;
}
