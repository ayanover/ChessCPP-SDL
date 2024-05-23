#include <algorithm>
#include "../../include/Board.hpp"
#include "enums/ColorType.hpp"
#include "enums/PieceType.hpp"
#include "../../include/SDL_Helper.hpp"

const int TILE_SIZE = 80;

Board::Board(SDL_Renderer* renderer) : renderer(renderer) {
    initialize();
}

Board::Board(const Board& other) : renderer(other.renderer) {
    initializeCopy();
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* otherPiece = other.board_[y][x].get();
            if (otherPiece != nullptr) {
                board_[y][x] = std::make_unique<Piece>(*otherPiece);  // Deep copy of Piece
            } else {
                board_[y][x] = nullptr;
            }
        }
    }
}

std::vector<std::vector<std::unique_ptr<Piece>>> Board::board;

Piece* Board::getPieceAt(int x, int y) {
    if(x < 0 || x >= 8 || y < 0 || y >= 8){
        return nullptr;
    }
    return board[y][x].get();
}

Piece* Board::getPiece(int x, int y) {
    if(x < 0 || x >= 8 || y < 0 || y >= 8){
        return nullptr;
    }
    return board_[y][x].get();
}

void Board::initializeCopy(){
    board.resize(8);
    for (auto& inner : board) {
        inner.resize(8);
    }
    board_.resize(8);
    for (auto& inner : board_) {
        inner.resize(8);
    }
}

void Board::initialize() {
    board.resize(8);
    for (auto& inner : board) {
        inner.resize(8);
    }
    board_.resize(8);
    for (auto& inner : board_) {
        inner.resize(8);
    }

    for (int i = 0; i < 8; ++i) {
        board[6][i] = std::make_unique<Piece>(ColorType::WHITE, PieceType::PAWN, renderer, i, 6);
        board[1][i] = std::make_unique<Piece>(ColorType::BLACK, PieceType::PAWN, renderer, i, 1);
    }
    board[0][0] = std::make_unique<Piece>(ColorType::BLACK, PieceType::ROOK, renderer, 0, 0);
    board[0][1] = std::make_unique<Piece>(ColorType::BLACK, PieceType::KNIGHT, renderer, 1, 0);
    board[0][2] = std::make_unique<Piece>(ColorType::BLACK, PieceType::BISHOP, renderer, 2, 0);
    board[0][3] = std::make_unique<Piece>(ColorType::BLACK, PieceType::QUEEN, renderer, 3, 0);
    board[0][4] = std::make_unique<Piece>(ColorType::BLACK, PieceType::KING, renderer, 4, 0);
    board[0][5] = std::make_unique<Piece>(ColorType::BLACK, PieceType::BISHOP, renderer, 5, 0);
    board[0][6] = std::make_unique<Piece>(ColorType::BLACK, PieceType::KNIGHT, renderer, 6, 0);
    board[0][7] = std::make_unique<Piece>(ColorType::BLACK, PieceType::ROOK, renderer, 7, 0);

    board[7][0] = std::make_unique<Piece>(ColorType::WHITE, PieceType::ROOK, renderer, 0, 7);
    board[7][1] = std::make_unique<Piece>(ColorType::WHITE, PieceType::KNIGHT, renderer, 1, 7);
    board[7][2] = std::make_unique<Piece>(ColorType::WHITE, PieceType::BISHOP, renderer, 2, 7);
    board[7][3] = std::make_unique<Piece>(ColorType::WHITE, PieceType::QUEEN, renderer, 3, 7);
    board[7][4] = std::make_unique<Piece>(ColorType::WHITE, PieceType::KING, renderer, 4, 7);
    board[7][5] = std::make_unique<Piece>(ColorType::WHITE, PieceType::BISHOP, renderer, 5, 7);
    board[7][6] = std::make_unique<Piece>(ColorType::WHITE, PieceType::KNIGHT, renderer, 6, 7);
    board[7][7] = std::make_unique<Piece>(ColorType::WHITE, PieceType::ROOK, renderer, 7, 7);
}

Board::~Board() {
    for (auto & row : board) {
        for (auto & piece : row) {
        }
    }
}

bool Board::isKingUnderAttack(ColorType kingColor) {
    std::pair<int, int> kingPosition;
    // Find the king's position
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = getPieceAt(i, j);
            if (piece != nullptr && piece->getPiece() == PieceType::KING && piece->getColor() == kingColor) {
                kingPosition = {i, j};
                break;
            }
        }
    }

    // Check if any piece can move to the king's position
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = getPieceAt(i, j);
            if (piece != nullptr && piece->getColor() != kingColor) {
                std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves();
                if (std::find(possibleMoves.begin(), possibleMoves.end(), kingPosition) != possibleMoves.end()) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Board::movePiece(int oldX, int oldY, int newX, int newY) {
    Piece* piece = getPieceAt(oldX, oldY);
    std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves();

    // Check if the new position is in the list of possible moves
    if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_pair(newX, newY)) != possibleMoves.end()) {
        board[newY][newX] = std::move(board[oldY][oldX]);
        board[oldY][oldX] = nullptr;
        piece->setPosX(newX);
        piece->setPosY(newY);
        piece->hasMoved = true;

        board_.resize(board.size());
        for (size_t i = 0; i < board.size(); ++i) {
            board_[i].resize(board[i].size());
            for (size_t j = 0; j < board[i].size(); ++j) {
                if (board[i][j]) {
                    board_[i][j] = std::make_unique<Piece>(*board[i][j]);
                } else {
                    board_[i][j] = nullptr;
                }
            }
        }

        return true;
    } else {
        return false;
    }
}
bool Board::movePiece_(int oldX, int oldY, int newX, int newY) {
    Piece* piece = getPiece(oldX, oldY);
    std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves();

    // Check if the new position is in the list of possible moves
    if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_pair(newX, newY)) != possibleMoves.end()) {
        board_[newY][newX] = std::move(board_[oldY][oldX]);
        piece->setPosX(newX);
        piece->setPosY(newY);
        piece->hasMoved = true;
        return true;
    } else {
        return false;
    }
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

bool Board::isGameOver() {
    // Check if either king is under attack
    if (!isKingUnderAttack(ColorType::WHITE) && !isKingUnderAttack(ColorType::BLACK)) {
        return false;
    }

    // Check if there are no valid moves left for either king
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = getPieceAt(x, y);
            if (piece != nullptr && piece->getPiece() == PieceType::KING) {
                if (!piece->calculatePossibleMoves().empty()) {
                    return false;
                }
            }
        }
    }

    // If neither king is under attack and there are no valid moves left for either king, the game is over
    return true;
}

int Board::getScore() {
    int score = 0;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = Board::getPieceAt(x, y);
            if (piece != nullptr) {
                switch (piece->getPiece()) {
                    case PieceType::PAWN:
                        score += piece->getColor() == ColorType::WHITE ? 1 : -1;
                        break;
                    case PieceType::KNIGHT:
                    case PieceType::BISHOP:
                        score += piece->getColor() == ColorType::WHITE ? 3 : -3;
                        break;
                    case PieceType::ROOK:
                        score += piece->getColor() == ColorType::WHITE ? 5 : -5;
                        break;
                    case PieceType::QUEEN:
                        score += piece->getColor() == ColorType::WHITE ? 9 : -9;
                        break;
                    case PieceType::KING:
                        score += piece->getColor() == ColorType::WHITE ? 100 : -100;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return score;
}
