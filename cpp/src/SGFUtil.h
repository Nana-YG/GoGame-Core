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
StonePosition convertSGFCoordinate(std::string);
void processOneFile(std::string fileName);

#endif // SGFUTIL_H

