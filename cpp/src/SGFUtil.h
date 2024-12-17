//
// Created by nana on 12/17/24.
//

#ifndef SGFUTIL_H
#define SGFUTIL_H

#include "Board.h"
#include <string>
#include <vector>
#include <filesystem>

void readData(std::string inputDir, std::string outputDir);
StonePosition convertSGFCoordinate(std::string);

#endif // SGFUTIL_H

