//
// Created by rexiv on 17.05.2024.
//

#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

#include <string>
#include <map>
#include "SDL.h"
#include "../src/game/enums/PieceType.hpp"
#include "../src/game/enums/ColorType.hpp"
class Piece {
public:
    Piece(ColorType color, PieceType pieceType);
    ~Piece();
    ColorType getColor() const;
    PieceType getPiece() const;

    void display(SDL_Renderer* renderer, PieceType type, int x, int y);

private:
    void initializeTexture(SDL_Renderer *renderer, const std::string &path, SDL_Texture *&texture);
    SDL_Texture *texture;
    SDL_Renderer* renderer;

    std::string getPieceTexturePath(PieceType type);
    ColorType color;
    PieceType piece;
};

#endif //CHESS_PIECE_HPP
