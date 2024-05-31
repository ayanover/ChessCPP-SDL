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
    pieceBoard.resize(8);
    for (int y = 0; y < 8; ++y) {
        board[y].resize(8);
        pieceBoard[y].resize(8);
        for (int x = 0; x < 8; ++x) {
            pieceBoard[y][x] = other.pieceBoard[y][x];
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
    pieceBoard.resize(8);
    for (auto& inner : board) {
        inner.resize(8);
    }
    for (auto& inner : pieceBoard) {
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

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece *piece = getPieceAt(x, y);
            if(piece == nullptr){
                pieceBoard[x][y].first = PieceType::EMPTY;
                pieceBoard[x][y].second = ColorType::NONE;
            }
            else{
                pieceBoard[x][y].first = piece->getPiece();
                pieceBoard[x][y].second = piece->getColor();
            }
        }
    }
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

void Board::updatePieceLocation(int oldX, int oldY, int newX, int newY){
    pieceBoard[newY][newX].first = pieceBoard[oldY][oldX].first;
    pieceBoard[newY][newX].second = pieceBoard[oldY][oldX].second;
    pieceBoard[oldY][oldX].first = PieceType::EMPTY;
    pieceBoard[oldY][oldX].second = ColorType::NONE;
}

bool Board::movePiece(int oldX, int oldY, int newX, int newY, bool isReal) {
    Piece* piece = getPieceAt(oldX, oldY);
    if (piece == nullptr) {
        return false;
    }


    std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves(*this);
    if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_pair(newX, newY)) != possibleMoves.end()) {
        tempPieces.push({std::move(board[newY][newX]), piece->hasMoved});
        // Check if the move is a castling move
        if (piece->getPiece() == PieceType::KING && abs(newX - oldX) == 2) {
            int rookX = (newX > oldX) ? 7 : 0; // The rook is on the right for king-side castling and on the left for queen-side castling
            Piece* rook = getPieceAt(rookX, oldY);
            int step = (newX > oldX) ? 1 : -1;
            // Move the rook
            board[oldY][newX - step] = std::move(board[oldY][rookX]);
            updatePieceLocation(oldX, oldY, newX, newY);
            rook->setPosX(newX - step);
            rook->setPosY(oldY);
            rook->hasMoved = true;
        }

        // Move the piece
        board[newY][newX] = std::move(board[oldY][oldX]);
        updatePieceLocation(oldX, oldY, newX, newY);
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
        if(!( (newY>=0 && newY<8) && newX>=0 && newX<8 )){
            return false;
        }
        tempPieces.push({std::move(board[newY][newX]), piece->hasMoved});
        board[newY][newX] = std::move(board[oldY][oldX]);
        piece->setPosX(newX);
        piece->setPosY(newY);
        piece->hasMoved = true;
        piece->setHasDoubleMoved(oldX, oldY, newX, newY);
        return true;
}

bool Board::isMoveSafe(int oldX, int oldY, int newX, int newY, ColorType kingColor, bool isReal) {

    if(isReal){
        std::unique_ptr<Board> tempBoard = std::unique_ptr<Board>(this->clone());
        if (tempBoard->tempMovePiece(oldX, oldY, newX, newY)) {
            if (tempBoard->isKingInCheck(kingColor, *tempBoard)) {
                return false;
            }
        }
        return true;
    }
    else{
        bool isInCheck;
        tempMovePiece(oldX, oldY, newX, newY);
        if(isKingInCheck(kingColor, *this)){
            isInCheck = false;
        } else{
            isInCheck = true;
        }
        revertMove(oldX, oldY, newX, newY);
        return isInCheck;
    }
}

void Board::revertMove(int oldX, int oldY, int newX, int newY){
    if(!( (newY>=0 && newY<8) && newX>=0 && newX<8 )){
        return;
    }
    board[oldY][oldX] = std::move(board[newY][newX]);
    Piece* piece = getPieceAt(oldX, oldY);
    if (piece != nullptr) {
        piece->setPosX(oldX);
        piece->setPosY(oldY);
        piece->hasMoved = tempPieces.top().second;
        piece->hasDoubleMoved = false;
    }
    board[newY][newX] = std::move(tempPieces.top().first);
    tempPieces.pop();
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
                std::vector<std::pair<int, int>> possibleMoves = piece->calculatePossibleMoves(*this, false);
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
    const int pieceValues[6] = {10, 30, 30, 50, 90, 1000}; // Pawn, Knight, Bishop, Rook, Queen, King
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = getPieceAt(x, y);
            if(piece != nullptr){
                int pieceValue = 0;
                switch (piece->getPiece()) {
                    case PieceType::PAWN:
                        pieceValue = pieceValues[0];
                        break;
                    case PieceType::KNIGHT:
                        pieceValue = pieceValues[1];
                        break;
                    case PieceType::BISHOP:
                        pieceValue = pieceValues[2];
                        break;
                    case PieceType::ROOK:
                        pieceValue = pieceValues[3];
                        break;
                    case PieceType::QUEEN:
                        pieceValue = pieceValues[4];
                        break;
                    case PieceType::KING:
                        pieceValue = pieceValues[5];
                        break;
                    default:
                        break;
                }
                if (piece->getColor() == ColorType::BLACK) {
                    score -= pieceValue;
                } else {
                    score += pieceValue;
                }
            }
        }
    }

    return score;
}
