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
            processOneFile(inputDir, outputDir);
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
        for (const std::string& inputFileName : files) {
            futures.push_back(pool.submit_task(
                [inputFileName, outputDir] {
                    return processOneFile(inputFileName, outputDir);
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

void processOneFile(std::string inputFileName, std::string outputDir) {

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
    std::ifstream file(inputFileName);

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
                std::cout << "Illegal at >>" << inputFileName << std::endl;
                std::cout << "The " << i <<" th Move: " << move << std::endl;
            }
        }
        if (move[1] == 'W') {
            if (!playerW.makeMove(&game, board, pos)) {
                std::cout << "Illegal at >>" << inputFileName << std::endl;
                std::cout << "The " << i <<" th Move: " << move << std::endl;
            }
        }
        //                std::cout << "---------------------------------------------" << std::endl;
        //                std::cout << board.showBoard() << std::endl;
        //                std::cout << board.showLiberties() << std::endl;


        auto boardMat = board.boardMatrix();
        auto libertyMat = board.libertyMatrix();

        // Check if the current move is for Black (even index i indicates Black's turn)
        // If it is Black's turn, negate the board and liberty matrices to switch perspective
        if (move[1] == 'B') { // Black's turn
            for (auto& row : boardMat) {
                for (auto& elem : row) {
                    elem = static_cast<spot_color>(-1 * elem); // Flip board matrix to White's perspective
                }
            }

            for (auto& row : libertyMat) {
                for (auto& elem : row) {
                    elem *= -1; // Flip liberty matrix to White's perspective
                }
            }
        }

        // Save the updated matrices and current move information to the HDF5 file
        std::lock_guard<std::mutex> lock(coutMutex);
        fs::path outputFilePath = fs::path(outputDir) /
                              fs::path(inputFileName).replace_extension(".h5").filename();
        std::string outputFileName = outputFilePath.string();
        saveToHDF5(outputFileName, boardMat, libertyMat, pos, i);
    }

    // Lock to write to std out
    // std::lock_guard<std::mutex> lock(coutMutex);
    // std::cout << ">>" << inputFileName << std::endl;
    // std::cout << board.showBoard() << std::endl;

    board.clear();
    file.close();
    // Unlocked
}

void saveToHDF5(const std::string& hdf5FilePath,
                const std::vector<std::vector<spot_color>>& boardMatrix,
                const std::vector<std::vector<int>>& libertyMatrix,
                const StonePosition& nextMove, int moveIndex) {
    try {
        // 如果文件不存在则创建，如果存在则追加数据
        H5::H5File file(hdf5FilePath, H5F_ACC_RDWR | H5F_ACC_CREAT);

        // 保存棋盘矩阵 (spot_color)
        std::string datasetName = "board_" + std::to_string(moveIndex);
        hsize_t dims[2] = {boardMatrix.size(), boardMatrix[0].size()};
        H5::DataSpace dataspace(2, dims);
        H5::DataSet dataset = file.createDataSet(datasetName, H5::PredType::NATIVE_INT, dataspace);
        std::vector<int> flattenedBoard;
        for (const auto& row : boardMatrix)
            flattenedBoard.insert(flattenedBoard.end(), row.begin(), row.end());
        dataset.write(flattenedBoard.data(), H5::PredType::NATIVE_INT);

        // 保存气矩阵 (libertyMatrix)
        std::string libertyName = "liberty_" + std::to_string(moveIndex);
        H5::DataSet libertyDataset = file.createDataSet(libertyName, H5::PredType::NATIVE_INT, dataspace);
        std::vector<int> flattenedLiberty;
        for (const auto& row : libertyMatrix)
            flattenedLiberty.insert(flattenedLiberty.end(), row.begin(), row.end());
        libertyDataset.write(flattenedLiberty.data(), H5::PredType::NATIVE_INT);

        // 保存下一步坐标
        std::string moveName = "nextMove_" + std::to_string(moveIndex);
        hsize_t moveDims[1] = {2};
        H5::DataSpace moveSpace(1, moveDims);
        H5::DataSet moveDataset = file.createDataSet(moveName, H5::PredType::NATIVE_INT, moveSpace);
        int moveData[2] = {nextMove.row, nextMove.col};
        moveDataset.write(moveData, H5::PredType::NATIVE_INT);

        std::cout << "Wrote to: " << hdf5FilePath << std::endl;
        file.close();
    } catch (H5::Exception& e) {
        std::cerr << "HDF5 Error: " << e.getCDetailMsg() << std::endl;
    }
}