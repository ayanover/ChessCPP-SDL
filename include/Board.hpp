#ifndef BOARD_HPP
#define BOARD_HPP

#include "SDL.h"
#include <memory>
#include <stack>
#include "Piece.hpp"
#include "Move.hpp"

class Piece;
class Board {
public:
    explicit Board(SDL_Renderer* renderer);
    Board(const Board& other);
    Board* clone() const;
    ~Board();

    void initialize();
    void display();
    SDL_Renderer* getRenderer();
    Piece* getPieceAt(int x, int y);
    void revertMove(int oldX, int oldY, int newX, int newY);
    void displayPossibleMoves(const std::vector<std::pair<int, int>>& possibleMoves);

    void updatePieceLocation(int oldX, int oldY, int newX, int newY);
    bool movePiece(int oldX, int oldY, int newX, int newY, bool isReal = false);
    bool tempMovePiece(int oldX, int oldY, int newX, int newY);

    bool isMoveSafe(int oldX, int oldY, int newX, int newY, ColorType kingColor, bool isReal);
    bool isKingInCheck(ColorType kingColor, Board& board);
    bool isKingInCheckmate(ColorType kingColor, Board& board);

    int getScore();

private:
    std::pair<int, int> getKingPosition(ColorType kingColor, Board& board);

private:
    const int TILE_SIZE = 80;

    SDL_Renderer* m_Renderer;
    std::unique_ptr<Piece> m_EnPassantPiece = nullptr;
    std::stack<std::pair<std::unique_ptr<Piece>, bool>> m_TempPieces;
    std::vector<std::vector<std::unique_ptr<Piece>>> m_PieceBoard;
    std::vector<std::vector<std::pair<PieceType, ColorType>>> m_PieceInfoBoard;
};

#endif // BOARD_HPP
