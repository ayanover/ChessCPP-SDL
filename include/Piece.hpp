//
// Created by rexiv on 17.05.2024.
//

#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

#include <string>
#include <map>
#include <vector>
#include "SDL.h"
#include "../src/game/enums/PieceType.hpp"
#include "../src/game/enums/ColorType.hpp"
#include "MoveHelper.hpp"
#include "Board.hpp"
class Board;

class Piece {
public:
    Piece(ColorType colorType, PieceType pieceType, Board& parentBoard, int x, int y);
    Piece(const Piece& other);
    ~Piece();

    ColorType getColor() const;
    PieceType getPiece() const;
    int getPosX() const;
    int getPosY() const;
    void setPosX(int x);
    void setPosY(int y);
    bool hasMoved = false;
    bool hasDoubleMoved = false;
    void setHasDoubleMoved(int oldX, int oldY, int newX, int newY);

    void display(SDL_Renderer* renderer, int x, int y);
    std::vector<std::pair<int, int>> calculatePossibleMoves(Board& board, bool isRealMove = true);

private:
    void initializeTexture(const std::string &path);
    SDL_Texture *texture{};
    SDL_Renderer* renderer;
    Board& parentBoard;

    std::string getPieceTexturePath();
    ColorType color;
    PieceType piece;
    int posX;
    int posY;
};

#endif //CHESS_PIECE_HPP
