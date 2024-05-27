#include "../../include/Board.hpp"
#include <algorithm>
#include "enums/ColorType.hpp"
#include "enums/PieceType.hpp"
#include "../../include/SDL_Helper.hpp"
#include "../../include/Game.hpp"

const int TILE_SIZE = 80;


Board::Board(SDL_Renderer* renderer) : renderer(renderer) {
    initialize();
}

Board::Board(const Board& other) : renderer(other.renderer) {
    board.resize(8);
    for (int y = 0; y < 8; ++y) {
        board[y].resize(8);
        for (int x = 0; x < 8; ++x) {
            if (other.board[y][x] != nullptr) {
                board[y][x] = std::make_unique<Piece>(*other.board[y][x]); // Deep copy of Piece
            } else {
                board[y][x] = nullptr;
            }
        }
    }
}



Board* Board::clone() const {
    return new Board(*this);
}

Board::~Board() {
}

void Board::initialize() {
    board.resize(8);
    for (auto& inner : board) {
        inner.resize(8);
    }

    for (int i = 0; i < 8; ++i) {
        board[6][i] = std::make_unique<Piece>(ColorType::WHITE, PieceType::PAWN, *this, i, 6);
        board[1][i] = std::make_unique<Piece>(ColorType::BLACK, PieceType::PAWN, *this, i, 1);
    }
    board[0][0] = std::make_unique<Piece>(ColorType::BLACK, PieceType::ROOK, *this, 0, 0);
    board[0][1] = std::make_unique<Piece>(ColorType::BLACK, PieceType::KNIGHT, *this, 1, 0);
    board[0][2] = std::make_unique<Piece>(ColorType::BLACK, PieceType::BISHOP, *this, 2, 0);
    board[0][3] = std::make_unique<Piece>(ColorType::BLACK, PieceType::QUEEN, *this, 3, 0);
    board[0][4] = std::make_unique<Piece>(ColorType::BLACK, PieceType::KING, *this, 4, 0);
    board[0][5] = std::make_unique<Piece>(ColorType::BLACK, PieceType::BISHOP, *this, 5, 0);
    board[0][6] = std::make_unique<Piece>(ColorType::BLACK, PieceType::KNIGHT, *this, 6, 0);
    board[0][7] = std::make_unique<Piece>(ColorType::BLACK, PieceType::ROOK, *this, 7, 0);

    board[7][0] = std::make_unique<Piece>(ColorType::WHITE, PieceType::ROOK, *this, 0, 7);
    board[7][1] = std::make_unique<Piece>(ColorType::WHITE, PieceType::KNIGHT, *this, 1, 7);
    board[7][2] = std::make_unique<Piece>(ColorType::WHITE, PieceType::BISHOP, *this, 2, 7);
    board[7][3] = std::make_unique<Piece>(ColorType::WHITE, PieceType::QUEEN, *this, 3, 7);
    board[7][4] = std::make_unique<Piece>(ColorType::WHITE, PieceType::KING, *this, 4, 7);
    board[7][5] = std::make_unique<Piece>(ColorType::WHITE, PieceType::BISHOP, *this, 5, 7);
    board[7][6] = std::make_unique<Piece>(ColorType::WHITE, PieceType::KNIGHT, *this, 6, 7);
    board[7][7] = std::make_unique<Piece>(ColorType::WHITE, PieceType::ROOK, *this, 7, 7);
}

Piece* Board::getPieceAt(int x, int y) {
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return nullptr;
    }
    return board[y][x].get();
}

SDL_Renderer* Board::getRenderer(){
    return renderer;
};

bool Board::movePiece(int oldX, int oldY, int newX, int newY, bool isReal) {
    Piece* piece = getPieceAt(oldX, oldY);
    if (piece == nullptr) {
        return false;
    }

    std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves(*this);
    if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_pair(newX, newY)) != possibleMoves.end()) {
        // Check if the move is a castling move
        if (piece->getPiece() == PieceType::KING && abs(newX - oldX) == 2) {
            int rookX = (newX > oldX) ? 7 : 0; // The rook is on the right for king-side castling and on the left for queen-side castling
            Piece* rook = getPieceAt(rookX, oldY);
            int step = (newX > oldX) ? 1 : -1;
            // Move the rook
            board[oldY][newX - step] = std::move(board[oldY][rookX]);
            rook->setPosX(newX - step);
            rook->setPosY(oldY);
            rook->hasMoved = true;
        }

        // Move the piece
        board[newY][newX] = std::move(board[oldY][oldX]);
        piece->setPosX(newX);
        piece->setPosY(newY);
        piece->hasMoved = true;
        piece->setHasDoubleMoved(oldX, oldY, newX, newY);

        if (piece->getPiece() == PieceType::PAWN && ((piece->getColor() == ColorType::WHITE && newY == 0) || (piece->getColor() == ColorType::BLACK && newY == 7))) {
            // Replace the pawn with a new queen. You can modify this to allow the player to choose which piece to promote to.
            board[newY][newX] = std::make_unique<Piece>(piece->getColor(), PieceType::QUEEN, *this, newX, newY);
        }

        if(isReal){
            if(isKingInCheckmate(ColorType::BLACK, *this) || isKingInCheckmate(ColorType::WHITE, *this)){
                board.clear();
                initialize();
                Game::playerToMove = ColorType::WHITE;
            }
        }
        return true;
    } else {
        return false;
    }
}


bool Board::tempMovePiece(int oldX, int oldY, int newX, int newY){
    Piece* piece = getPieceAt(oldX, oldY);
    if (piece == nullptr) {
        return false;
    }
    board[newY][newX] = std::move(board[oldY][oldX]);
    piece->setPosX(newX);
    piece->setPosY(newY);
    piece->hasMoved = true;
    piece->setHasDoubleMoved(oldX, oldY, newX, newY);
    return true;
}

bool Board::isMoveSafe(int oldX, int oldY, int newX, int newY, ColorType kingColor) {
    std::unique_ptr<Board> tempBoard = std::unique_ptr<Board>(this->clone());
    if (tempBoard->tempMovePiece(oldX, oldY, newX, newY)) {
        if (tempBoard->isKingInCheck(kingColor, *tempBoard)) {
            return false;
        }
    }
    return true; // The move is safe
}

bool Board::isGameOver() {
    return (isKingInCheckmate(ColorType::BLACK, *this) || isKingInCheckmate(ColorType::WHITE, *this));
}

void Board::display() {
    SDL_Rect rect;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            rect.x = x * TILE_SIZE;
            rect.y = y * TILE_SIZE;

            if ((x + y) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // White
            } else {
                SDL_SetRenderDrawColor(renderer, 87, 87, 87, 255); // Black
            }
            if(board[y][x] != nullptr){
                if(board[y][x]->getPiece() == PieceType::KING && isKingEndangered){
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                }
            }

            SDL_RenderFillRect(renderer, &rect);

            if (board[y][x] != nullptr) {
                board[y][x]->display(renderer, rect.x, rect.y);
            }
        }
    }
}

void Board::displayPossibleMoves(const std::vector<std::pair<int, int>>& possibleMoves) {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray
    for (const auto& move : possibleMoves) {
        int x = move.first * TILE_SIZE + TILE_SIZE / 2;
        int y = move.second * TILE_SIZE + TILE_SIZE / 2;
        for (int radius = 0; radius < TILE_SIZE / 2; radius++) {
            SDL_RenderDrawCircle(renderer, x, y, radius);
        }
    }
}

bool Board::isKingInCheck(ColorType kingColor, Board& board_) {
    std::pair<int, int> kingPos = getKingPosition(kingColor, board_);
    ColorType opponentColor = (kingColor == ColorType::WHITE) ? ColorType::BLACK : ColorType::WHITE;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = getPieceAt(x, y);
            if (piece != nullptr && piece->getColor() == opponentColor) {
                std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves(board_, false);
                Board& tempBoardRef = board_;
                if (std::find(possibleMoves.begin(), possibleMoves.end(), kingPos) != possibleMoves.end()) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Board::isKingInCheckmate(ColorType kingColor, Board& board_) {
    if (!isKingInCheck(kingColor, board_)) {
        return false; // The king is not in check
    }

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = board_.getPieceAt(x, y);
            if (piece != nullptr && piece->getColor() == kingColor) {
                std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves(board_, false);
                for (const auto& move : possibleMoves) {
                    std::unique_ptr<Board> tempBoard = std::unique_ptr<Board>(this->clone());
                    if (tempBoard->tempMovePiece(x, y, move.first, move.second)) {
                        Board& tempBoardRef = *tempBoard;
                        if (!tempBoard->isKingInCheck(kingColor, *tempBoard)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

std::pair<int, int> Board::getKingPosition(ColorType kingColor, Board& board_) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = getPieceAt(x, y);
            if (piece != nullptr && piece->getPiece() == PieceType::KING && piece->getColor() == kingColor) {
                return {x, y};
            }
        }
    }

    return {-1, -1};
}

std::vector<Move> Board::generateMoves() {
    std::vector<Move> moves;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece *piece = getPieceAt(x, y);
            if (piece != nullptr && piece->getColor() == ColorType::BLACK) {
                for (const auto &move: piece->calculatePossibleMoves(*this)) {
                    if (move.first >= 0 && move.first < 8 && move.second >= 0 && move.second < 8) {
                        moves.emplace_back(std::pair<int, int>(x, y), std::pair<int, int>(move.first, move.second));
                    }
                }
            }
        }
    }
    return moves;
}

int Board::getScore() {
    int score = 0;
    const int pieceValues[6] = {1, 3, 3, 5, 9, 100}; // Pawn, Knight, Bishop, Rook, Queen, King
    const int pawnTable[8][8] = {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {-5, -5, -5, -5, -5, -5, -5, -5},
            {-1, -1, -2, -3, -3, -2, -1, -1},
            {-1, -1, -2, -3, -3, -2, -1, -1},
            {-1, -1, -2, -3, -3, -2, -1, -1},
            {-1, -1, -2, -3, -3, -2, -1, -1},
            {-1, -1, -2, -3, -3, -2, -1, -1},
            {0, 0, 0, 0, 0, 0, 0, 0}
    };

    const int knightTable[8][8] = {
            {5, 4, 3, 3, 3, 3, 4, 5},
            {4, 2, 0, -1, -1, 0, 2, 4},
            {3, 0, -1, -2, -2, -1, 0, 3},
            {3, -1, -2, -3, -3, -2, -1, 3},
            {3, 0, -2, -3, -3, -2, 0, 3},
            {3, -1, -1, -2, -2, -1, -1, 3},
            {4, 2, 0, 0, 0, 0, 2, 4},
            {5, 4, 3, 3, 3, 3, 4, 5}
    };

    const int bishopTable[8][8] = {
            {2, 1, 1, 1, 1, 1, 1, 2},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, -1, -1, -1, -1, 0, 1},
            {1, -1, -1, -2, -2, -1, -1, 1},
            {1, 0, -2, -2, -2, -2, 0, 1},
            {1, -2, -2, -2, -2, -2, -2, 1},
            {1, -1, 0, 0, 0, 0, -1, 1},
            {2, 1, 1, 1, 1, 1, 1, 2}
    };

    const int rookTable[8][8] = {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, -1, -1, -1, -1, -1, -1, 0},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {0, 0, 0, -1, -1, 0, 0, 0}
    };


    const int queenTable[8][8] = {
            {2, 1, 1, 1, 1, 1, 1, 2},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {2, 1, 1, 1, 1, 1, 1, 2}
    };

    const int kingTable[8][8] = {
            {-3, -4, -4, -5, -5, -4, -4, -3},
            {-3, -4, -4, -5, -5, -4, -4, -3},
            {-3, -4, -4, -5, -5, -4, -4, -3},
            {-3, -4, -4, -5, -5, -4, -4, -3},
            {-2, -3, -3, -4, -4, -3, -3, -2},
            {-1, -2, -2, -2, -2, -2, -2, -1},
            {2,  2,  0,  0,  0,  0,  2,  2},
            {2,  3,  1,  0,  0,  1,  3,  2}
    };

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = getPieceAt(x, y);
            if (piece != nullptr) {
                int pieceValue = 0;
                int positionalValue = 0;
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

                if (piece->getColor() == ColorType::BLACK) {
                    score -= pieceValue;
                    score -= positionalValue;
                } else {
                    score += pieceValue;
                    score += positionalValue;
                }
            }
        }
    }

    return score;
}
