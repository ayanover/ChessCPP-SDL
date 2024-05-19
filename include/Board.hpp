//
// Created by rexiv on 17.05.2024.
//

#ifndef BOARD_HPP
#define BOARD_HPP

#include "SDL.h"
#include <vector>
#include "Piece.hpp"

class Board {
public:
    explicit Board(SDL_Renderer* renderer);
    void initialize();
    void display();
private:
    SDL_Renderer* renderer;
    std::vector<std::vector<Piece*>> board;
};

#endif // BOARD_HPP
