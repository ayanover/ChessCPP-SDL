#ifndef BOARD_HPP
#define BOARD_HPP

#include "SDL.h"
#include <vector>
#include <string>
#include <memory>
#include <stack>
#include "Piece.hpp"
#include "Move.hpp"

class Piece;

class Board {
public:
    // Constructor taking the renderer as a parameter
    explicit Board(SDL_Renderer* renderer);
    Board* clone() const;
    ~Board();

    void initialize();
    void display();
    SDL_Renderer* getRenderer();
    Piece* getPieceAt(int x, int y);

    void revertMove(int oldX, int oldY, int newX, int newY);
    void displayPossibleMoves(const std::vector<std::pair<int, int>>& possibleMoves);
    bool movePiece(int oldX, int oldY, int newX, int newY, bool isReal = false);
    bool tempMovePiece(int oldX, int oldY, int newX, int newY);
    void updatePieceLocation(int oldX, int oldY, int newX, int newY);

    bool isMoveSafe(int oldX, int oldY, int newX, int newY, ColorType kingColor, bool isReal);
    bool isKingInCheck(ColorType kingColor, Board& board);
    bool isKingInCheckmate(ColorType kingColor, Board& board);
    bool isGameOver();
    std::vector<Move> generateMoves();
    bool isKingEndangered = false;
    int getScore();

    // Removed the copy constructor as it might not make sense with the new design
    Board(const Board& other);

private:
    std::pair<int, int> getKingPosition(ColorType kingColor, Board& board);

    std::unique_ptr<Piece> enPassantPiece = nullptr;
    std::stack<std::pair<std::unique_ptr<Piece>, bool>> tempPieces;
    SDL_Renderer* renderer;
    std::vector<std::vector<std::unique_ptr<Piece>>> board;
    std::vector<std::vector<std::pair<PieceType, ColorType>>> pieceBoard;
};

#endif // BOARD_HPP
