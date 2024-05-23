//
// Created by rexiv on 17.05.2024.
//

#ifndef BOARD_HPP
#define BOARD_HPP

#include "SDL.h"
#include <vector>
#include <string>
#include <memory>
#include "Piece.hpp"

class Board {
public:
    explicit Board(SDL_Renderer* renderer);
    Board(const Board& other);
    ~Board();
    void initialize();
    void initializeCopy();
    void display();
    static Piece* getPieceAt(int x, int y);
    Piece* getPiece(int x, int y);
    void displayPossibleMoves(const std::vector<std::pair<int, int>>& possibleMoves);
    bool movePiece(int oldX, int oldY, int newX, int newY);
    bool movePiece_(int oldX, int oldY, int newX, int newY);
    bool isKingUnderAttack(ColorType kingColor);
    bool isGameOver();
    int getScore();

private:
    SDL_Renderer* renderer;
    SDL_Texture* whiteTileTexture;
    SDL_Texture* blackTileTexture;
    static std::vector<std::vector<std::unique_ptr<Piece>>> board;
    std::vector<std::vector<std::unique_ptr<Piece>>> board_;
    SDL_Texture* loadTexture(const std::string& path);
};

#endif // BOARD_HPP
