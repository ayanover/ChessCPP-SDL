//
// Created by rexiv on 17.05.2024.
//

#ifndef CHESS_AI_H
#define CHESS_AI_H


#include "../Board.hpp"
#include "../Move.hpp"


std::vector<Move> generateMoves(Board& board, ColorType colorType);
void botMove(Board& board, int depth);
int minimax(Board& board, int depth, int alpha, int beta, bool isMaximizingPlayer);

#endif //CHESS_AI_H
