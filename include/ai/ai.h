//
// Created by rexiv on 17.05.2024.
//

#ifndef CHESS_AI_H
#define CHESS_AI_H


#include "../Board.hpp"

void botMove(Board& board);
std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> minimax(Board& board, int depth, bool isMaximizingPlayer);


#endif //CHESS_AI_H
