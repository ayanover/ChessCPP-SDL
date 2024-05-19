//
// Created by rexiv on 17.05.2024.
//

#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

#include "SDL.h"
class Piece {
public:
    enum Color { WHITE, BLACK };
    Piece(Color color);
    virtual ~Piece() = default;
    Color getColor() const;
    virtual void display(SDL_Renderer* renderer, int x, int y, int w, int h) const = 0;
private:
    Color color;
};

#endif //CHESS_PIECE_HPP
