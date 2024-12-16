//
// Created by Yunyi Gao on 12/12/24.
//

#include "Game.h"
#include "Board.h"
#include "Config.h"

Game::Game(Player *playeB, Player *playerW) {
    this->playeB = playeB;
    this->playerW = playerW;
}

void Game::init() {
    const Config& config = Config::getInstance();
    rule = config.rule;
    komi = config.komi;
    moveCount = 0;
}

int Game::getMoveCount() {
    return moveCount;
}

void Game::addBoard(Board board) {
    // Convert the board to a string representation
    std::string* boardStr = new std::string(board.showBoard());

    // Add the new string to the history
    history.push_back(boardStr);
    moveCount++;
}

std::string Game::getBoardFromHist(int index) {
    return *(this->history[index]);
}

bool Game::superko(Board board) {
    for (int i = 0; i < moveCount; i++) {
        if (board.showBoard() == getBoardFromHist(i)) {
            return true; // Repeated board state
        }
    }
    return false;
}



