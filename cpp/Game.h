//
// Created by Yunyi Gao on 12/12/24.
//

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

class Player;
class Board;

class Game {

private:
    Player * playeB;
    Player * playerW;
    std::string rule;
    float komi{};
    int moveCount;
    std::vector<std::string*> history;

public:
    Game(Player * playerB, Player * playerW);
    void init();
    int getMoveCount();
    void addBoard(Board board);
    std::string getBoardFromHist(int index);
    bool superko(Board board);
};



#endif //GAME_H
