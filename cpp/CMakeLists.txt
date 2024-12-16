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
        src/main.cpp
        src/Player.cpp
        src/Player.h
        src/Game.cpp
        src/Game.h
        src/Board.cpp
        src/Board.h
        src/Config.cpp
        src/Config.h
        src/GroupUtil.cpp
        src/GroupUtil.h
        src/GTP.cpp
        src/GTP.h
)

# link nlohmann/json to project
target_link_libraries(GTP-Core PRIVATE nlohmann_json::nlohmann_json)
