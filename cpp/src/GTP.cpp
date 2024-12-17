//
// Created by nana on 12/16/24.
//

#include <string>
#include <iostream>
#include <vector>
#include "GTP.h"
#include "Game.h"
#include "Board.h"
#include "Player.h"

GTP::GTP() {
    boardsize = 19;
    board.init(boardsize);
    playerB.setColor(BLACK);
    playerW.setColor(WHITE);
    game = Game(&playerB, &playerW);
    game.init();
}

std::vector<std::string> GTP::parseCommand(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    // // Debug: Print parsed tokens
    // std::cout << "Parsed tokens: \n";
    // for (const auto& t : tokens) {
    //     std::cout << "TOKEN " << t << "\n";
    //     std::cout << "TOKEN == boardsize" << (t == "boardsize") << "\n";
    // }
    std::cout << std::endl;

    return tokens;
}

std::string GTP::handleBoardsize(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        return "? Invalid arguments for boardsize";
    }
    int size = std::stoi(args[0]);
    if (size < 1 || size > 19) {
        return "? Unsupported board size";
    }
    boardsize = size;
    board.init(size);
    return "= ";
}

std::string GTP::handleClearBoard() {
    board.init(boardsize);
    return "= ";
}

std::string GTP::handlePlay(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        return "? Invalid arguments for play";
    }

    spot_color color = (args[0] == "B") ? BLACK : (args[0] == "W") ? WHITE : EMPTY;
    if (color == EMPTY) {
        return "? Invalid color";
    }

    StonePosition pos = convertGTPCoordinate(args[1]);

    if (color == BLACK) {
        if (!playerB.makeMove(&game, board, pos)) {
            return "? Invalid move";
        }
    } else if (color == WHITE) {
        if (!playerW.makeMove(&game, board, pos)) {
            return "? Invalid move";
        }
    }

    cmdHistory.push_back("play " + args[0] + " " + args[1]);
    return "= ";
}

std::string GTP::handleGenmove(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        return "? Invalid arguments for genmove";
    }

    spot_color color = (args[0] == "B") ? BLACK : (args[0] == "W") ? WHITE : EMPTY;
    if (color == EMPTY) {
        return "? Invalid color";
    }

    // Generate move (placeholder logic)
    StonePosition bestMove = {1, 1}; // Entrance of AI engine

    board.move(nullptr, &bestMove, color);
    cmdHistory.push_back("genmove " + args[0]);
    return "= " + std::string(1, 'A' + bestMove.col) + std::to_string(boardsize - bestMove.row);
}

std::string GTP::handleShowBoard() {
    return "= \n" + board.showBoard();
}

std::string GTP::processCommand(const std::string& input) {
    std::vector<std::string> tokens = parseCommand(input);

    if (tokens.empty()) {
        return "? Empty command";
    }

    const std::string& command = tokens[0];
    std::cout << "Command: " << command << std::endl;

    if (command == "boardsize") {
        tokens.erase(tokens.begin());
        return handleBoardsize(tokens);
    } else if (command == "clear_board") {
        tokens.erase(tokens.begin());
        return handleClearBoard();
    } else if (command == "play") {
        tokens.erase(tokens.begin());
        return handlePlay(tokens);
    } else if (command == "genmove") {
        tokens.erase(tokens.begin());
        return handleGenmove(tokens);
    } else if (command == "showboard") {
        tokens.erase(tokens.begin());
        return handleShowBoard();
    } else {
        return "? Unknown command";
    }
}

void GTP::run() {
    std::string input;
    while (std::getline(std::cin, input)) {
        std::cout << processCommand(input) << "\n" << std::endl;
    }
}

StonePosition GTP::convertGTPCoordinate(std::string stringCoordinate) {
    StonePosition pos;
    pos.col = stringCoordinate[0] - 'A';
    stringCoordinate.erase(0, 1);
    pos.row = std::stoi(stringCoordinate) - 1;
    return pos;
}
