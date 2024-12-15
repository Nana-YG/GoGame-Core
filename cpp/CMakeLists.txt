cmake_minimum_required(VERSION 3.28)
project(GTP-Core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

# include FetchContent module
include(FetchContent)

# nlohmann/json download
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

add_executable(GTP-Core
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

# link nlohmann/json to project
target_link_libraries(GTP-Core PRIVATE nlohmann_json::nlohmann_json)
