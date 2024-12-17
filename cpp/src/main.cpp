#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "Config.h"
#include "GTP.h"
#include "SGFUtil.h"
#include "H5Cpp.h"
#include <iostream>
#include <chrono>

void saveToHDF5(const std::vector<std::vector<SGFMove>>& allMoves, const std::string& hdf5FilePath);

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

        if (argc < 5) {
            std::cerr << "Usage: ./GTP-Core ReadData [SGF_PATH] [HDF5_OUTPUT_PATH]" << std::endl;
            return 1;
        }

        std::string inputDir = argv[2];
        std::string outputDir = argv[3];

        readData(inputDir, outputDir);
    }

    else {
        std::cerr << "Invalid mode! Use 'GTP' or 'ReadData'." << std::endl;
        return 1;
    }

    return 0;
}

