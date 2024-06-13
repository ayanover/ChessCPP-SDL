cmake_minimum_required(VERSION 3.22)
project(Chess)

set(CMAKE_CXX_STANDARD 14)

# Define build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()

# Compiler flags for release build
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/ai/ai.cpp
        include/ai/ai.h
        include/Piece.hpp
        src/game/Piece.cpp
        src/game/Board.cpp
        include/Board.hpp
        src/game/Game.cpp
        include/Game.hpp
        src/game/enums/GameState.hpp
        include/SDL_Helper.hpp
        include/MoveHelper.hpp
        src/utils/MoveHelper.cpp
        src/utils/BoardManager.cpp
        include/RendererManager.hpp
        src/game/Move.cpp
        include/Move.hpp
        src/game/Menu.cpp
        include/Menu.hpp
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_INCLUDE_DIR "SDL2/x86_64-w64-mingw32/include/SDL2")
set(SDL2_TTF_INCLUDE_DIR "SDL2/x86_64-w64-mingw32/include/SDL2")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})

find_package(SDL2_ttf REQUIRED)
if(SDL2_TTF_FOUND)
    include_directories(${SDL2_TTF_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARY})
else()
    message(FATAL_ERROR "SDL2_ttf not found")
endif()

find_package(SDL2_image REQUIRED)
if(SDL2_IMAGE_FOUND)
    include_directories(${SDL2_IMAGE_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARY})
else()
    message(FATAL_ERROR "SDL2_image not found")
endif()
