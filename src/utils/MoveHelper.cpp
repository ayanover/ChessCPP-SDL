//
// Created by rexiv on 23.05.2024.
//
#include "../../include/MoveHelper.hpp"
#include "../../include/Game.hpp"


std::vector<std::pair<int, int>> calculatePawnMoves(const Piece& piece, Board& board, bool isRealMove) {
    std::vector<std::pair<int, int>> moves;
    int direction = (piece.getColor() == ColorType::WHITE) ? -1 : 1;
    int x = piece.getPosX();
    int y = piece.getPosY();

    // Pawns can move forward one square, if that square is unoccupied.
    if(y + direction >= 0 && y + direction < 8 ){
        if(board.getPieceAt(x, y + direction) == nullptr){
            if (isRealMove) {
                if(board.isMoveSafe(x, y, x, y + direction, piece.getColor(), false)){
                    moves.push_back({x, y + direction});
                }
            } else {
                moves.push_back({x, y + direction});
            }
        }
    }

    // Pawns can move forward two squares if they haven't moved yet and both squares in front of them are unoccupied.
    if(!piece.hasMoved && board.getPieceAt(x, y + direction) == nullptr && board.getPieceAt(x, y + 2*direction) == nullptr){
        if (isRealMove) {
            if(board.isMoveSafe(x, y, x, y + direction * 2, piece.getColor(), false)){
                moves.push_back({x, y + direction * 2});
            }
        } else {
            moves.push_back({x, y + direction * 2});
        }
    }

    // Pawns can capture an enemy piece diagonally forward.
    if(x - 1 >= 0 && board.getPieceAt(x - 1, y + direction) != nullptr && board.getPieceAt(x - 1, y + direction)->getColor() != piece.getColor()){
        if (isRealMove) {
            if(board.isMoveSafe(x, y, x - 1, y + direction, piece.getColor(), false)){
                moves.push_back({x - 1, y + direction});
            }
        } else {
            moves.push_back({x - 1, y + direction});
        }
    }
    if(x + 1 < 8 && board.getPieceAt(x + 1, y + direction) != nullptr && board.getPieceAt(x + 1, y + direction)->getColor() != piece.getColor()){
        if (isRealMove) {
            if(board.isMoveSafe(x, y, x + 1, y + direction, piece.getColor(), false)){
                moves.push_back({x + 1, y + direction});
            }
        } else {
            moves.push_back({x + 1, y + direction});
        }
    }

    // TODO: Add logic to handle en passant.
    if (y == (piece.getColor() == ColorType::WHITE ? 3 : 4)) { // The pawn is on its fifth rank
        std::vector<int> adjacentFiles = {x - 1, x + 1};
        for (int adjFile : adjacentFiles) {
            if (adjFile >= 0 && adjFile < 8) { // The file is on the m_Board
                Piece* adjPiece = board.getPieceAt(adjFile, y);
                if (adjPiece != nullptr && adjPiece->getColor() != piece.getColor() && adjPiece->getPiece() == PieceType::PAWN && adjPiece->hasDoubleMoved == true) {
                    if (isRealMove) {
                        if(board.isMoveSafe(x, y, x, y + direction, piece.getColor(), false)){
                            moves.push_back({x, y + direction});
                        }
                    } else {
                        moves.push_back({x, y + direction});
                    }
                }
            }
        }
    }
    return moves;
}

std::vector<std::pair<int, int>> calculateKnightMoves(const Piece& piece, Board& board, bool isRealMove) {
    std::vector<std::pair<int, int>> moves;
    int x = piece.getPosX();
    int y = piece.getPosY();

    std::vector<std::pair<int, int>> offsets = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    for (const auto& offset : offsets) {
        int newX = x + offset.first;
        int newY = y + offset.second;
        if(newX >= 0 && newX < 8 && newY >= 0 && newY < 8){
            Piece* destinationPiece = board.getPieceAt(newX, newY);
            if (destinationPiece == nullptr || destinationPiece->getColor() != piece.getColor()) {
                if (isRealMove) {
                    if(board.isMoveSafe(x, y, newX, newY, piece.getColor(), false)){
                        moves.push_back({newX, newY});
                    }
                } else {
                    moves.push_back({newX, newY});
                }
            }
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> calculateBishopMoves(const Piece& piece, Board& board, bool isRealMove) {
    std::vector<std::pair<int, int>> moves;
    int x = piece.getPosX();
    int y = piece.getPosY();

    std::vector<std::pair<int, int>> directions = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    for (const auto& direction : directions) {
        for (int i = 1; i < 8; ++i) {
            int newX = x + i * direction.first;
            int newY = y + i * direction.second;
            if(newX >= 0 && newX < 8 && newY >= 0 && newY < 8){
                if(board.getPieceAt(newX, newY) != nullptr){
                    if(board.getPieceAt(newX, newY)->getColor() != piece.getColor()){
                        if (isRealMove) {
                            if(board.isMoveSafe(x, y, newX, newY, piece.getColor(), false)){
                                moves.push_back({newX, newY});
                            }
                        } else {
                            moves.push_back({newX, newY});
                        }
                    }
                    break;
                }
                moves.push_back({newX, newY});
            }
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> calculateRookMoves(const Piece& piece, Board& board, bool isRealMove) {
    std::vector<std::pair<int, int>> moves;
    int x = piece.getPosX();
    int y = piece.getPosY();

    std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (const auto& direction : directions) {
        for (int i = 1; i < 8; ++i) {
            int newX = x + i * direction.first;
            int newY = y + i * direction.second;
            if(newX >= 0 && newX < 8 && newY >= 0 && newY < 8){
                if(board.getPieceAt(newX, newY) != nullptr){
                    if(board.getPieceAt(newX, newY)->getColor() != piece.getColor()){
                        if (isRealMove) {
                            if(board.isMoveSafe(x, y, newX, newY, piece.getColor(), false)){
                                moves.push_back({newX, newY});
                            }
                        } else {
                            moves.push_back({newX, newY});
                        }
                    }
                    break;
                }
                if (isRealMove) {
                    if(board.isMoveSafe(x, y, newX, newY, piece.getColor(), false)){
                        moves.push_back({newX, newY});
                    }
                } else {
                    moves.push_back({newX, newY});
                }
            }
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> calculateQueenMoves(const Piece& piece, Board& board, bool isRealMove) {
    std::vector<std::pair<int, int>> rookMoves = calculateRookMoves(piece, board, isRealMove);
    std::vector<std::pair<int, int>> bishopMoves = calculateBishopMoves(piece, board, isRealMove);
    rookMoves.insert(rookMoves.end(), bishopMoves.begin(), bishopMoves.end());
    return rookMoves;
}


std::vector<std::pair<int, int>> calculateKingMoves(const Piece& piece, Board& board, bool isRealMove) {
    std::vector<std::pair<int, int>> moves;
    int x = piece.getPosX();
    int y = piece.getPosY();

    std::vector<std::pair<int, int>> offsets = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    for (const auto& offset : offsets) {
        int newX = x + offset.first;
        int newY = y + offset.second;
        if(newX >= 0 && newX < 8 && newY >= 0 && newY < 8){
            Piece* destinationPiece = board.getPieceAt(newX, newY);
            if (destinationPiece == nullptr || destinationPiece->getColor() != piece.getColor()) {
                if (isRealMove) {
                    if(board.isMoveSafe(x, y, newX, newY, piece.getColor(), false)){
                        moves.push_back({newX, newY});
                    }
                } else {
                    moves.push_back({newX, newY});
                }
            }
        }
    }

    // Castling
    if (!piece.hasMoved) {
        // King-side castling
        if (board.getPieceAt(x + 3, y) != nullptr && !board.getPieceAt(x + 3, y)->hasMoved) {
            if (board.getPieceAt(x + 1, y) == nullptr && board.getPieceAt(x + 2, y) == nullptr) {
                if (isRealMove) {
                    if(!board.isKingInCheck(piece.getColor(), board) && board.isMoveSafe(x, y, x + 1, y, piece.getColor(), false)){
                        moves.push_back({x + 2, y});
                    }
                } else {
                    moves.push_back({x + 2, y});
                }
            }
        }

        // Queen-side castling
        if (board.getPieceAt(x - 4, y) != nullptr && !board.getPieceAt(x - 4, y)->hasMoved) {
            if (board.getPieceAt(x - 1, y) == nullptr && board.getPieceAt(x - 2, y) == nullptr && board.getPieceAt(x - 3, y) == nullptr) {
                if (isRealMove) {
                    if(!board.isKingInCheck(piece.getColor(), board) && board.isMoveSafe(x, y, x - 2, y, piece.getColor(), false)){
                        moves.push_back({x - 2, y});
                    }
                } else {
                    moves.push_back({x - 2, y});
                }
            }
        }
    }

    return moves;
}