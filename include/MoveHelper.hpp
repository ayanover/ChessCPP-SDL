//
// Created by rexiv on 23.05.2024.
//

#ifndef CHESS_MOVEHELPER_HPP
#define CHESS_MOVEHELPER_HPP

#include <vector>
#include <utility>
#include "Piece.hpp"

std::vector<std::pair<int, int>> calculatePawnMoves(const Piece& piece);
std::vector<std::pair<int, int>> calculateKnightMoves(const Piece& piece);
std::vector<std::pair<int, int>> calculateBishopMoves(const Piece& piece);
std::vector<std::pair<int, int>> calculateRookMoves(const Piece& piece);
std::vector<std::pair<int, int>> calculateQueenMoves(const Piece& piece);
std::vector<std::pair<int, int>> calculateKingMoves(const Piece& piece);


#endif //CHESS_MOVEHELPER_HPP
