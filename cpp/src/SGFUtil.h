//
// Created by nana on 12/17/24.
//

#ifndef SGFUTIL_H
#define SGFUTIL_H

#include <string>
#include <vector>
#include <filesystem>

// SGF 解析结果存储结构
struct SGFMove {
    std::string player;
    int row;
    int col;
};

void readData(std::string inputDir, std::string outputDir);

#endif // SGFUTIL_H

