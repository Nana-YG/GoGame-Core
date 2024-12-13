//
// Created by nana on 12/13/24.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
private:
    Config() = default; // 私有构造函数
    Config(const Config&) = delete; // 禁止拷贝
    Config& operator=(const Config&) = delete; // 禁止赋值

public:
    int boardSize = 19;     // default board size
    float komi = 6.5;      // default komi
    int handicap = 0;       // default handicap
    std::string rule = "Chinese";

    // load configuration from ./config.json
    bool loadFromFile(const std::string& filePath);
    static Config & getInstance();
};


#endif //CONFIG_H
