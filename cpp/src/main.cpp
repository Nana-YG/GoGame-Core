#include "H5Cpp.h"
#include <iostream>
#include <chrono>
#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "Config.h"
#include "GTP.h"
#include "SGFUtil.h"



int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: ./GTP-Core [GTP | ReadData] [SGF_PATH] [HDF5_OUTPUT_PATH]" << std::endl;
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "GTP") {
        std::cout << "Entering GTP Mode..." << std::endl;
        GTP gtp;
        gtp.run(); // Start the GTP loop
    }


    // Read *.sgf in [SGF_PATH]/ and output HDF5 file to [HDF5_OUTPUT_PATH]/
    if (mode == "ReadData") {

        if (argc < 4) {
            std::cerr << "Usage: ./GTP-Core ReadData [SGF_PATH] [HDF5_OUTPUT_PATH]" << std::endl;
            return 1;
        }

        std::string inputDir = argv[2];
        std::string outputDir = argv[3];

        // Start timer
        auto start = std::chrono::high_resolution_clock::now();

        readData(inputDir, outputDir);

        // Stop timer
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Time taken by readData: " << duration.count() << " ms" << std::endl;

        return 0;
    }

    if (mode == "Merge") {
        if (argc < 4) {
            std::cerr << "Usage: ./GTP-Core Merge [SGF_PATH] [HDF5_OUTPUT_PATH]" << std::endl;
            return 1;
        }

        std::string inputDir = argv[2];
        std::string outputDir = argv[3];

        // Start timer
        auto start = std::chrono::high_resolution_clock::now();

        mergeHDF5(inputDir, outputDir);

        // Stop timer
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Time taken by mergeHDF5: " << duration.count() << " ms" << std::endl;

        return 0;
    }

    else {
        std::cerr << "Invalid mode! Use 'GTP' or 'ReadData'." << std::endl;
        return 1;
    }

    return 0;
}

