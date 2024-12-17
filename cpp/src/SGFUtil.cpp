//
// Created by nana on 12/17/24.
//

#include "SGFUtil.h"
#include "Game.h"
#include "Board.h"
#include "Player.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include "H5Cpp.h"
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

void readData(std::string inputDir, std::string outputDir) {

    // Initialize Game
    int boardsize = 19;
    Board board;
    Player playerB;
    Player playerW;
    Game game;

    board.init(boardsize);
    playerB.setColor(BLACK);
    playerW.setColor(WHITE);
    game = Game(&playerB, &playerW);
    game.init();

    std::vector<std::string> files;

    try {
        // Iterate all files
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            if (entry.is_regular_file()) { // Make sure it's a file, not a directory
                files.push_back(entry.path().string());
            }
        }

        // Output read play game on each file
        std::regex pattern(";[BW]\\[[a-z]{2}\\]");

        for (std::string fileName : files) {

            std::string line;
            std::vector<std::string> moves;
            std::ifstream file(fileName);

            while (std::getline(file, line)) {
                std::sregex_iterator begin(line.begin(), line.end(), pattern), end;
                for (auto it = begin; it != end; ++it) {
                    moves.push_back(it->str());
                }
            }

            for (std::string move : moves) {
                StonePosition pos = convertSGFCoordinate(move);
                if (move[1] == 'B') {
                    playerB.makeMove(&game, board, pos);
                }
                if (move[1] == 'W') {
                    playerW.makeMove(&game, board, pos);
                }
            }

            board.clear();
            file.close();
        }

    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}

StonePosition convertSGFCoordinate(std::string move) {
    StonePosition pos;
    pos.col = move[3] - 'a';
    pos.row = 19 - (move[4] - 'a') - 1;
    return pos;
}

