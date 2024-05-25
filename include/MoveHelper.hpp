//
// Created by rexiv on 23.05.2024.
//

#ifndef CHESS_MOVEHELPER_HPP
#define CHESS_MOVEHELPER_HPP

#include <vector>
#include <utility>
#include "Piece.hpp"
#include "Board.hpp"
class Board;
class Piece;
std::vector<std::pair<int, int>> calculatePawnMoves(const Piece& piece, Board& board, bool isRealMove = true);
std::vector<std::pair<int, int>> calculateKnightMoves(const Piece& piece, Board& board, bool isRealMove = true);
std::vector<std::pair<int, int>> calculateBishopMoves(const Piece& piece, Board& board, bool isRealMove = true);
std::vector<std::pair<int, int>> calculateRookMoves(const Piece& piece, Board& board, bool isRealMove = true);
std::vector<std::pair<int, int>> calculateQueenMoves(const Piece& piece, Board& board, bool isRealMove = true);
std::vector<std::pair<int, int>> calculateKingMoves(const Piece& piece, Board& board, bool isRealMove = true);


#endif //CHESS_MOVEHELPER_HPP
