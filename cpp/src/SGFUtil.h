//
// Created by nana on 12/17/24.
//

#ifndef SGFUTIL_H
#define SGFUTIL_H

#include <string>
#include <vector>
#include <filesystem>
#include "Board.h"

void readData(std::string inputDir, std::string outputDir);
StonePosition convertSGFCoordinate(std::string move);
void processOneFile(std::string inputFileName, std::string outputDir);
void mergeHDF5(const std::string& inputDir, const std::string& outputFilePath);
void saveToHDF5(const std::string& hdf5FilePath,
                const std::vector<std::vector<spot_color>>& boardMatrix,
                const std::vector<std::vector<int>>& libertyMatrix,
                const StonePosition& nextMove, int moveIndex);

#endif // SGFUTIL_H

