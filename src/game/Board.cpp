#include "../../include/Board.hpp"
#include <algorithm>
#include "enums/ColorType.hpp"
#include "enums/PieceType.hpp"
#include "../../include/SDL_Helper.hpp"
#include "../../include/Game.hpp"

Board::Board(SDL_Renderer* renderer) : m_Renderer(renderer) {
    initialize();
}

Board::Board(const Board& other) : m_Renderer(other.m_Renderer) {
    m_PieceBoard.resize(8);
    m_PieceInfoBoard.resize(8);
    for (int y = 0; y < 8; ++y) {
        m_PieceBoard[y].resize(8);
        m_PieceInfoBoard[y].resize(8);
        for (int x = 0; x < 8; ++x) {
            m_PieceInfoBoard[y][x] = other.m_PieceInfoBoard[y][x];
            if (other.m_PieceBoard[y][x] != nullptr) {
                m_PieceBoard[y][x] = std::make_unique<Piece>(*other.m_PieceBoard[y][x]); // Deep copy of Piece
            } else {
                m_PieceBoard[y][x] = nullptr;
            }
        }
    }
}



Board* Board::clone() const
{
    return new Board(*this);
}

Board::~Board() {
}

void Board::initialize()
{
    m_PieceBoard.resize(8);
    m_PieceInfoBoard.resize(8);
    for (auto& inner : m_PieceBoard)
    {
        inner.resize(8);
    }
    for (auto& inner : m_PieceInfoBoard)
    {
        inner.resize(8);
    }

    for (int i = 0; i < 8; ++i)
    {
        m_PieceBoard[6][i] = std::make_unique<Piece>(ColorType::WHITE, PieceType::PAWN, *this, i, 6);
        m_PieceBoard[1][i] = std::make_unique<Piece>(ColorType::BLACK, PieceType::PAWN, *this, i, 1);
    }
    m_PieceBoard[0][0] = std::make_unique<Piece>(ColorType::BLACK, PieceType::ROOK, *this, 0, 0);
    m_PieceBoard[0][1] = std::make_unique<Piece>(ColorType::BLACK, PieceType::KNIGHT, *this, 1, 0);
    m_PieceBoard[0][2] = std::make_unique<Piece>(ColorType::BLACK, PieceType::BISHOP, *this, 2, 0);
    m_PieceBoard[0][3] = std::make_unique<Piece>(ColorType::BLACK, PieceType::QUEEN, *this, 3, 0);
    m_PieceBoard[0][4] = std::make_unique<Piece>(ColorType::BLACK, PieceType::KING, *this, 4, 0);
    m_PieceBoard[0][5] = std::make_unique<Piece>(ColorType::BLACK, PieceType::BISHOP, *this, 5, 0);
    m_PieceBoard[0][6] = std::make_unique<Piece>(ColorType::BLACK, PieceType::KNIGHT, *this, 6, 0);
    m_PieceBoard[0][7] = std::make_unique<Piece>(ColorType::BLACK, PieceType::ROOK, *this, 7, 0);

    m_PieceBoard[7][0] = std::make_unique<Piece>(ColorType::WHITE, PieceType::ROOK, *this, 0, 7);
    m_PieceBoard[7][1] = std::make_unique<Piece>(ColorType::WHITE, PieceType::KNIGHT, *this, 1, 7);
    m_PieceBoard[7][2] = std::make_unique<Piece>(ColorType::WHITE, PieceType::BISHOP, *this, 2, 7);
    m_PieceBoard[7][3] = std::make_unique<Piece>(ColorType::WHITE, PieceType::QUEEN, *this, 3, 7);
    m_PieceBoard[7][4] = std::make_unique<Piece>(ColorType::WHITE, PieceType::KING, *this, 4, 7);
    m_PieceBoard[7][5] = std::make_unique<Piece>(ColorType::WHITE, PieceType::BISHOP, *this, 5, 7);
    m_PieceBoard[7][6] = std::make_unique<Piece>(ColorType::WHITE, PieceType::KNIGHT, *this, 6, 7);
    m_PieceBoard[7][7] = std::make_unique<Piece>(ColorType::WHITE, PieceType::ROOK, *this, 7, 7);

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece *piece = getPieceAt(x, y);
            if(piece == nullptr){
                m_PieceInfoBoard[x][y].first = PieceType::EMPTY;
                m_PieceInfoBoard[x][y].second = ColorType::NONE;
            }
            else{
                m_PieceInfoBoard[x][y].first = piece->getPiece();
                m_PieceInfoBoard[x][y].second = piece->getColor();
            }
        }
    }
}

Piece* Board::getPieceAt(int x, int y)
{
    if (x < 0 || x >= 8 || y < 0 || y >= 8)
    {
        return nullptr;
    }
    return m_PieceBoard[y][x].get();
}

SDL_Renderer* Board::getRenderer()
{
    return m_Renderer;
};

void Board::updatePieceLocation(int oldX, int oldY, int newX, int newY)
{
    m_PieceInfoBoard[newY][newX].first = m_PieceInfoBoard[oldY][oldX].first;
    m_PieceInfoBoard[newY][newX].second = m_PieceInfoBoard[oldY][oldX].second;
    m_PieceInfoBoard[oldY][oldX].first = PieceType::EMPTY;
    m_PieceInfoBoard[oldY][oldX].second = ColorType::NONE;
}

bool Board::movePiece(int oldX, int oldY, int newX, int newY, bool isReal)
{
    Piece* piece = getPieceAt(oldX, oldY);
    if (piece == nullptr)
    {
        return false;
    }


    std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves(*this);
    if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_pair(newX, newY)) != possibleMoves.end())
    {
        m_TempPieces.push({std::move(m_PieceBoard[newY][newX]), piece->hasMoved});
        if (piece->getPiece() == PieceType::KING && abs(newX - oldX) == 2)
        {
            int rookX = (newX > oldX) ? 7 : 0; 
            Piece* rook = getPieceAt(rookX, oldY);
            int step = (newX > oldX) ? 1 : -1;
            m_PieceBoard[oldY][newX - step] = std::move(m_PieceBoard[oldY][rookX]);
            updatePieceLocation(oldX, oldY, newX, newY);
            rook->setPosX(newX - step);
            rook->setPosY(oldY);
            rook->hasMoved = true;
        }

        m_PieceBoard[newY][newX] = std::move(m_PieceBoard[oldY][oldX]);
        updatePieceLocation(oldX, oldY, newX, newY);
        piece->setPosX(newX);
        piece->setPosY(newY);
        piece->hasMoved = true;
        piece->setHasDoubleMoved(oldX, oldY, newX, newY);

        if (piece->getPiece() == PieceType::PAWN && ((piece->getColor() == ColorType::WHITE && newY == 0) || (piece->getColor() == ColorType::BLACK && newY == 7)))
        {
            m_PieceBoard[newY][newX] = std::make_unique<Piece>(piece->getColor(), PieceType::QUEEN, *this, newX, newY);
        }

        if(isReal)
        {
            if(isKingInCheckmate(ColorType::BLACK, *this) || isKingInCheckmate(ColorType::WHITE, *this))
            {
                m_PieceBoard.clear();
                initialize();
                Game::setPlayerToMove(ColorType::WHITE);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}


bool Board::tempMovePiece(int oldX, int oldY, int newX, int newY)
{
    Piece* piece = getPieceAt(oldX, oldY);
    if (piece == nullptr)
    {
        return false;
    }
        if(!( (newY>=0 && newY<8) && newX>=0 && newX<8 ))
        {
            return false;
        }
        m_TempPieces.push({std::move(m_PieceBoard[newY][newX]), piece->hasMoved});
    m_PieceBoard[newY][newX] = std::move(m_PieceBoard[oldY][oldX]);
        piece->setPosX(newX);
        piece->setPosY(newY);
        piece->hasMoved = true;
        piece->setHasDoubleMoved(oldX, oldY, newX, newY);
        return true;
}

bool Board::isMoveSafe(int oldX, int oldY, int newX, int newY, ColorType kingColor, bool isReal)
{
    if(isReal)
    {
        std::unique_ptr<Board> tempBoard = std::unique_ptr<Board>(this->clone());
        if (tempBoard->tempMovePiece(oldX, oldY, newX, newY))
        {
            if (tempBoard->isKingInCheck(kingColor, *tempBoard))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        bool isInCheck;
        tempMovePiece(oldX, oldY, newX, newY);
        if(isKingInCheck(kingColor, *this))
        {
            isInCheck = false;
        }
        else
        {
            isInCheck = true;
        }
        revertMove(oldX, oldY, newX, newY);
        return isInCheck;
    }
}

void Board::revertMove(int oldX, int oldY, int newX, int newY)
{
    if(!( (newY>=0 && newY<8) && newX>=0 && newX<8 ))
    {
        return;
    }
    m_PieceBoard[oldY][oldX] = std::move(m_PieceBoard[newY][newX]);
    Piece* piece = getPieceAt(oldX, oldY);
    if (piece != nullptr)
    {
        piece->setPosX(oldX);
        piece->setPosY(oldY);
        piece->hasMoved = m_TempPieces.top().second;
        piece->hasDoubleMoved = false;
    }
    m_PieceBoard[newY][newX] = std::move(m_TempPieces.top().first);
    m_TempPieces.pop();
}

void Board::displayPieces(){
    SDL_Rect rect;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            rect.x = x * TILE_SIZE;
            rect.y = y * TILE_SIZE;
            if (m_PieceBoard[y][x] != nullptr)
            {
                m_PieceBoard[y][x]->display(m_Renderer, rect.x, rect.y);
            }
        }
    }
}
void Board::display()
{
    SDL_Rect rect;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            rect.x = x * TILE_SIZE;
            rect.y = y * TILE_SIZE;

            if ((x + y) % 2 == 0)
            {
                SDL_SetRenderDrawColor(m_Renderer, 240, 217, 181, 255); // White
            }
            else
            {
                SDL_SetRenderDrawColor(m_Renderer, 181, 136, 99, 255); // Black
            }
            SDL_RenderFillRect(m_Renderer, &rect);
        }
    }
}

void Board::displayPossibleMoves(const std::vector<std::pair<int, int>>& possibleMoves)
{
    SDL_SetRenderDrawColor(m_Renderer, 128, 128, 128, 255); // Gray
    for (const auto& move : possibleMoves)
    {
        int x = move.first * TILE_SIZE + TILE_SIZE / 2;
        int y = move.second * TILE_SIZE + TILE_SIZE / 2;
        for (int radius = 0; radius < TILE_SIZE / 2; radius++)
        {
            SDL_RenderDrawCircle(m_Renderer, x, y, radius);
        }
    }
}

bool Board::isKingInCheck(ColorType kingColor, Board& board_)
{
    std::pair<int, int> kingPos = getKingPosition(kingColor, board_);
    ColorType opponentColor = (kingColor == ColorType::WHITE) ? ColorType::BLACK : ColorType::WHITE;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece* piece = getPieceAt(x, y);
            if (piece != nullptr && piece->getColor() == opponentColor)
            {
                std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves(*this, false);
                if (std::find(possibleMoves.begin(), possibleMoves.end(), kingPos) != possibleMoves.end())
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Board::isKingInCheckmate(ColorType kingColor, Board& board_)
{
    if (!isKingInCheck(kingColor, board_))
    {
        return false;
    }

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece* piece = board_.getPieceAt(x, y);
            if (piece != nullptr && piece->getColor() == kingColor)
            {
                std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves(board_, false);
                for (const auto& move : possibleMoves)
                {
                    std::unique_ptr<Board> tempBoard = std::unique_ptr<Board>(this->clone());
                    if (tempBoard->tempMovePiece(x, y, move.first, move.second))
                    {
                        if (!tempBoard->isKingInCheck(kingColor, *tempBoard))
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

std::pair<int, int> Board::getKingPosition(ColorType kingColor, Board& board_)
{
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece* piece = getPieceAt(x, y);
            if (piece != nullptr && piece->getPiece() == PieceType::KING && piece->getColor() == kingColor)
            {
                return {x, y};
            }
        }
    }

    return {-1, -1};
}


int Board::getScore() {
    int score = 0;
    const int pieceValues[6] = {10, 30, 30, 50, 90, 1000}; // Pawn, Knight, Bishop, Rook, Queen, King

    const float pawnTable[8][8] = {
            { 0, 0, 0, 0, 0, 0, 0, 0 },
            { 1, 1, 2, 3, 3, 2, 1, 1 },
            { 1, 1, 2, 3, 3, 2, 1, 1 },
            { 0, 0, 0, 6, 6, 0, 0, 0 },
            { 0, 0, 0, 6, 6, 0, 0, 0 },
            { 1, 1, 1, 3, 3, 1, 1, 1 },
            { 1, 1, 2, 3, 3, 2, 1, 1 },
            { 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    const float knightTable[8][8] = {
            {-5, 0, -3, -3, -3, -3, 0, -5},
            {-4, -2,  0,  0,  0,  0, -2, -4},
            {-3,  0,  1,  1,  1,  1,  0, -3},
            {-3,  0,  1,  2,  2,  1,  0, -3},
            {-3,  0,  1,  2,  2,  1,  0, -3},
            {-3,  0,  1,  1,  1,  1,  0, -3},
            {-4, -2,  0,  0,  0,  0, -2, -4},
            {-5, -4, -3, -3, -3, -3, -4, -5}
    };

    const float bishopTable[8][8] = {
            {-2, -1, -1, -1, -1, -1, -1, -2},
            {-1,  0,  0,  0,  0,  0,  0, -1},
            {-1,  0,  1,  1,  1,  1,  0, -1},
            {-1,  0,  1,  1,  1,  1,  0, -1},
            {-1,  0,  1,  1,  1,  1,  0, -1},
            {-1,  0,  1,  1,  1,  1,  0, -1},
            {-1,  0,  0,  0,  0,  0,  0, -1},
            {-2, -1, -1, -1, -1, -1, -1, -2}
    };

    const float rookTable[8][8] = {
            { 0,  0,  0,  0,  0,  0,  0,  0},
            { 1,  2,  2,  2,  2,  2,  2,  1},
            {-1,  0,  0,  0,  0,  0,  0, -1},
            {-1,  0,  0,  0,  0,  0,  0, -1},
            {-1,  0,  0,  0,  0,  0,  0, -1},
            {-1,  0,  0,  0,  0,  0,  0, -1},
            {-1,  0,  0,  0,  0,  0,  0, -1},
            { 0,  0,  0,  1,  1,  0,  0,  0}
    };

    const float queenTable[8][8] = {
            {-2, -1, -1, -0.5, -0.5, -1, -1, -2},
            {-1,  0,  0,  0,  0,  0,  0, -1},
            {-1,  0,  0.5,  0.5,  0.5,  0.5,  0, -1},
            {-0.5,  0,  0.5,  0.5,  0.5,  0.5,  0, -0.5},
            { 0,  0,  0.5,  0.5,  0.5,  0.5,  0, -0.5},
            {-1,  0.5,  0.5,  0.5,  0.5,  0.5,  0, -1},
            {-1,  0,  0.5,  0,  0,  0,  0, -1},
            {-2, -1, -1, -0.5, -0.5, -1, -1, -2}
    };

    const float kingTable[8][8] = {
            {2,  3,  1,  0,  0,  1,  3,  2},
            {2,  2,  0,  0,  0,  0,  2,  2},
            {-3, -4, -4, -5, -5, -4, -4, -3},
            {-3, -4, -4, -5, -5, -4, -4, -3},
            {-2, -3, -3, -4, -4, -3, -3, -2},
            {-1, -2, -2, -2, -2, -2, -2, -1},
            { 2,  2,  0,  0,  0,  0,  2,  2},
            { 2,  3,  1,  0,  0,  1,  3,  2}
    };

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = getPieceAt(x, y);
            if (piece != nullptr) {
                int pieceValue = 0;
                float positionalValue = 0;
                switch (piece->getPiece()) {
                    case PieceType::PAWN:
                        pieceValue = pieceValues[0];
                        positionalValue = pawnTable[y][x];
                        break;
                    case PieceType::KNIGHT:
                        pieceValue = pieceValues[1];
                        positionalValue = knightTable[y][x];
                        break;
                    case PieceType::BISHOP:
                        pieceValue = pieceValues[2];
                        positionalValue = bishopTable[y][x];
                        break;
                    case PieceType::ROOK:
                        pieceValue = pieceValues[3];
                        positionalValue = rookTable[y][x];
                        break;
                    case PieceType::QUEEN:
                        pieceValue = pieceValues[4];
                        positionalValue = queenTable[y][x];
                        break;
                    case PieceType::KING:
                        pieceValue = pieceValues[5];
                        positionalValue = kingTable[y][x];
                        break;
                    default:
                        break;
                }
                float pieceScore = pieceValue + positionalValue;
                if (piece->getColor() == ColorType::BLACK) {
                    score -= pieceScore;
                } else {
                    score += pieceScore;
                }
            }
        }
    }

    return score;
}
