//
// Created by nana on 12/16/24.
//

#ifndef GTP_H
#define GTP_H

#include "Game.h"
#include "Board.h"
#include "Player.h"
#include <string>
#include <sstream>
#include <vector>

class GTP {
private:
    Board board; // Current board state
    Player playerB;
    Player playerW;
    Game game;
    int boardsize; // Current board size
    std::vector<std::string> cmdHistory; // Command and board history

    // Helper to parse GTP commands
    static std::vector<std::string> parseCommand(const std::string& input);

    // GTP command handlers
    std::string handleBoardsize(const std::vector<std::string>& args);
    std::string handleClearBoard();
    std::string handlePlay(const std::vector<std::string>& args);
    std::string handleGenmove(const std::vector<std::string>& args);
    std::string handleShowBoard();

public:

    GTP();                                                  // Constructor
    void run();                                             // Main loop to handle GTP commands
    std::string processCommand(const std::string& input);   // Process a single GTP command
    StonePosition convertCoordinate(std::string stringCoordinate);
};

#endif // GTP_H
