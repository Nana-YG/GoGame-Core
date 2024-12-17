//
// Created by nana on 12/17/24.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include "H5Cpp.h"
#include <filesystem>
#include <regex>
#include <future>
#include <mutex>
#include "SGFUtil.h"
#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "BS_thread_pool.hpp"

namespace fs = std::filesystem;
std::mutex coutMutex;

void readData(std::string inputDir, std::string outputDir) {

    std::vector<std::string> files;
    std::vector<std::future<void>> futures;
    BS::thread_pool pool(std::thread::hardware_concurrency());

    try {

        if (fs::is_regular_file(inputDir)) {
            processOneFile(inputDir);
            return;
        }

        // Iterate all files
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            if (entry.is_regular_file()) { // Make sure it's a file, not a directory
                files.push_back(entry.path().string());
            }
        }

        // Output read play game on each file

        // Process files using multithreading
        for (const std::string& fileName : files) {
            futures.push_back(pool.submit_task(
                [fileName] {
                    return processOneFile(fileName);
                }));
        }

        // Wait for all threads to complete
        for (auto& f : futures) {
            f.get();
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

void processOneFile(std::string fileName) {

    // Regex pattern
    std::regex pattern(";[BW]\\[[a-z]{2}\\]|;A[WB]\\[[a-z]{2}\\]");

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

    std::string line;
    std::vector<std::string> moves;
    std::ifstream file(fileName);

    while (std::getline(file, line)) {
        std::sregex_iterator begin(line.begin(), line.end(), pattern), end;
        for (auto it = begin; it != end; ++it) {
            moves.push_back(it->str());
        }
    }

    for (int i = 0; i < moves.size(); i++) {

        std::string move = moves[i];
        // Handle Add Black (AB) and Add White (AW)
        if (move[1] == 'A') {
            move.erase(1, 1);
        }

        StonePosition pos = convertSGFCoordinate(move);
        if (move == ";B[]" || move == ";W[]" || move == ";B[tt]" || move == ";W[tt]") {
            continue;
        }
        if (move[1] == 'B') {
            if (!playerB.makeMove(&game, board, pos)) {
                std::cout << "Illegal at >>" << fileName << std::endl;
                std::cout << "The " << i <<" th Move: " << move << std::endl;
            }
        }
        if (move[1] == 'W') {
            if (!playerW.makeMove(&game, board, pos)) {
                std::cout << "Illegal at >>" << fileName << std::endl;
                std::cout << "The " << i <<" th Move: " << move << std::endl;
            }
        }
        //                std::cout << "---------------------------------------------" << std::endl;
        //                std::cout << board.showBoard() << std::endl;
        //                std::cout << board.showLiberties() << std::endl;
    }

    // Lock to write to std out
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << ">>" << fileName << std::endl;
    std::cout << board.showBoard() << std::endl;

    board.clear();
    file.close();
    // Unlocked
}
