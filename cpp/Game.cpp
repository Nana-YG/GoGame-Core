//
// Created by Yunyi Gao on 12/12/24.
//

#include "Game.h"
#include "Board.h"
#include "Config.h"

Game::Game(Player *playeB, Player *playerW, Board *board) {
    this->playeB = playeB;
    this->playerW = playerW;
}

void Game::init() {
    const Config& config = Config::getInstance();
    rule = config.rule;
    komi = config.komi;
    moveCount = 0;
    history.resize(300);
}

int Game::getMoveCount() {
    return moveCount;
}

void Game::addBoard(Board board) {
    history[moveCount] = board;
    moveCount++;
}

Board Game::getBoardFromHist(int index) {
    return history[index];
}

bool Game::superko(Board board) {
    for (int i = 0; i < moveCount; i++) {
        if (board.equalsTo(history[i])) {
            return false; // Repeated board state
        }
    }
    return true;
}



