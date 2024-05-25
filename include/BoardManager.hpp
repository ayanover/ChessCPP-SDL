//
// Created by rexiv on 17.05.2024.
//

#ifndef BOARDMANAGER_HPP
#define BOARDMANAGER_HPP

#include "Board.hpp"
#include "SDL.h"
#include <vector>
#include <memory>

class BoardManager {
public:
    BoardManager(SDL_Renderer* renderer);

    std::unique_ptr<Board> createBoard();
    std::unique_ptr<Board> cloneBoard(const Board& board);

private:
    SDL_Renderer* renderer;
};

#endif // BOARDMANAGER_HPP
