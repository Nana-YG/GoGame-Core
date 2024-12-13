//
// Created by Yunyi Gao on 12/12/24.
//

#include "Board.h"
#include <sstream>
#include <string>

void Board::init() {
    this->size = 19;
    this->gameOver = false;
    this->board.resize(size, std::vector<spot_color>(size, EMPTY));;
}

bool Board::legal(int row, int col, StonePosition *pos) {
}

void Board::move(StonePosition *pos, spot_color color) {
}

int Board::getSize() {
}

bool Board::isOver() {
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
        oss << "\n"; // 每行结束后换行
    }

    return oss.str(); // 返回拼接好的字符串
}