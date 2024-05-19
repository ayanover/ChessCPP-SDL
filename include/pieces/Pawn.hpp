#ifndef PAWN_HPP
#define PAWN_HPP

#include "../Piece.hpp"

class Pawn : public Piece {
public:
    Pawn(Color color);
    void display(SDL_Renderer* renderer, int x, int y, int w, int h) const override;

private:
    void drawCircle(SDL_Renderer* renderer, int x, int y, int radius) const;
};

Pawn::Pawn(Color color) : Piece(color) {}

void Pawn::display(SDL_Renderer* renderer, int x, int y, int w, int h) const {
    // For simplicity, draw a filled circle to represent the pawn
    int radius = w / 4;
    int centerX = x + w / 2;
    int centerY = y + h / 2;

    SDL_SetRenderDrawColor(renderer, getColor() == WHITE ? 255 : 0, getColor() == WHITE ? 255 : 0, getColor() == WHITE ? 255 : 0, 255);
    drawCircle(renderer, centerX, centerY, radius);
}

void Pawn::drawCircle(SDL_Renderer* renderer, int x, int y, int radius) const {
    // Draw filled circle
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

#endif // PAWN_HPP