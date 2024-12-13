cmake_minimum_required(VERSION 3.29)
project(WuZiQi)

set(CMAKE_CXX_STANDARD 20)

# 引入 FetchContent 模块
include(FetchContent)

# 配置 nlohmann/json 的下载和引入
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# 添加可执行文件
add_executable(MyAlphaGo
        main.cpp
        Player.cpp
        Player.h
        Game.cpp
        Game.h
        Board.cpp
        Board.h
        Config.cpp
        Config.h
        gtp.cpp
        gtp.h
)

# 链接 nlohmann/json 到 MyAlphaGo
target_link_libraries(MyAlphaGo PRIVATE nlohmann_json::nlohmann_json)
