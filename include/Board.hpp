//
// Created by rexiv on 17.05.2024.
//

#ifndef BOARD_HPP
#define BOARD_HPP

#include "SDL.h"
#include <vector>
#include <string>
#include "Piece.hpp"

class Board {
public:
    explicit Board(SDL_Renderer* renderer);
    ~Board();
    void initialize();
    void display();
private:
    SDL_Renderer* renderer;
    SDL_Texture* whiteTileTexture;
    SDL_Texture* blackTileTexture;
    std::vector<std::vector<Piece*>> board;
    SDL_Texture* loadTexture(const std::string& path);
};

#endif // BOARD_HPP
