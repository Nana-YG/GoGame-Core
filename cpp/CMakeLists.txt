cmake_minimum_required(VERSION 3.28)
project(GoGame-Core)

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

# HDF5
find_package(HDF5 REQUIRED COMPONENTS CXX) # Search for HDF5
if (HDF5_FOUND)
    message(STATUS "HDF5 include dir: ${HDF5_INCLUDE_DIRS}")
    message(STATUS "HDF5 libraries: ${HDF5_LIBRARIES}")
endif()

include_directories(${HDF5_INCLUDE_DIRS})

include_directories(include)


add_executable(GoGame-Core
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
        src/SGFUtil.cpp
        src/SGFUtil.h
)

# link nlohmann/json to project
target_link_libraries(GoGame-Core PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(GoGame-Core PRIVATE ${HDF5_LIBRARIES})
