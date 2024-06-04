#include <utility>
#include <algorithm>
#include "../../include/ai/ai.h"
#include "../../include/Game.hpp"

std::vector<Move> generateMoves(Board& board, ColorType colorType) {
    std::vector<Move> moves;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = board.getPieceAt(x, y);
            if (piece != nullptr && piece->getColor() == colorType) {
                for (const auto &move: piece->calculatePossibleMoves(board)) {
                    if (move.first >= 0 && move.first < 8 && move.second >= 0 && move.second < 8) {
                        moves.emplace_back(std::pair<int, int>(x, y), std::pair<int, int>(move.first, move.second));
                    }
                }
            }
        }
    }
    return moves;
}

// Alpha-beta pruning minimax function with move ordering
int minimax(Board& board, int depth, bool isMaximizingPlayer, int alpha, int beta) {
    if (depth == 0) {
        int score = board.getScore();
        return score;
    }

    std::vector<Move> moves = generateMoves(board, isMaximizingPlayer ? ColorType::WHITE : ColorType::BLACK);

    if (moves.empty()) {
        int score = board.getScore();
        return score;
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        for (const auto& move : moves) {
            board.tempMovePiece(move.from.first, move.from.second, move.to.first, move.to.second);
            int eval = minimax(board, depth - 1, false, alpha, beta);
            board.revertMove(move.from.first, move.from.second, move.to.first, move.to.second);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Beta cutoff
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto& move : moves) {
            board.tempMovePiece(move.from.first, move.from.second, move.to.first, move.to.second);
            int eval = minimax(board, depth - 1, true, alpha, beta);
            board.revertMove(move.from.first, move.from.second, move.to.first, move.to.second);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }
        return minEval;
    }
}

Move findBestMove(Board& board, int depth) {
    int bestValue = INT_MAX;
    Move bestMove(std::pair<int, int>(-1, -1),std::pair<int, int>(-1, -1)); // Initialize with an invalid move
    std::vector<Move> moves = generateMoves(board, ColorType::BLACK);

    for (const auto& move : moves) {
        board.tempMovePiece(move.from.first, move.from.second, move.to.first, move.to.second);
        int boardValue = minimax(board, depth, true, INT_MIN, INT_MAX);
        board.revertMove(move.from.first, move.from.second, move.to.first, move.to.second);

        if (boardValue < bestValue) {
            bestValue = boardValue;
            bestMove = move;
        }
    }
    return bestMove;
}


void botMove(Board& board, int depth) {
    Move bestMove = findBestMove(board, depth);
    board.movePiece(bestMove.from.first, bestMove.from.second, bestMove.to.first, bestMove.to.second);
    Game::setPlayerToMove(ColorType::WHITE);
}
