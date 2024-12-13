//
// Created by Yunyi Gao on 12/12/24.
//

#include "Game.h"
#include "config.h"

Game::Game(Player *playeB, Player *playerW, Board *board) {
    this->playeB = playeB;
    this->playerW = playerW;
    this->board = board;
}

void Game::init(Board *board) {
    const Config& config = Config::getInstance();
    board->init(config.boardSize);
    rule = config.rule;
    komi = config.komi;
}

void Game::start() {

}
