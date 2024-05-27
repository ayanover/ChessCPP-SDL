//
// Created by rexiv on 17.05.2024.
//

#include <utility>
#include <limits>
#include <algorithm>
#include "../../include/ai/ai.h"
#include "../../include/Game.hpp"


std::vector<Move> generateMoves(Board& board) {
    std::vector<Move> moves;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece *piece = board.getPieceAt(x, y);
            if (piece != nullptr && piece->getColor() == ColorType::BLACK) {
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
int minimax(Board& board, int depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0 || board.isGameOver()) {
        return board.getScore();
    }

    std::vector<Move> moves = generateMoves(board);
    if (isMaximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        for (const Move &move: moves) {
                board.movePiece(move.from.first, move.from.second, move.to.first, move.to.second);
                int eval = minimax(board, depth - 1, alpha, beta, false);
                board.movePiece(move.to.first, move.to.second, move.from.first, move.from.second);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    break;
                }
            }
            return maxEval;
        } else {
            int minEval = std::numeric_limits<int>::max();
            for (const Move &move: moves) {
                board.movePiece(move.from.first, move.from.second, move.to.first, move.to.second);
                int eval = minimax(board, depth - 1, alpha, beta, true);
                board.movePiece(move.from.first, move.from.second, move.to.first, move.to.second);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break;
                }
            }
            return minEval;
        }
    }

    Move findBestMove(Board &board, int depth) {
        int bestValue = std::numeric_limits<int>::min();
        Move bestMove(std::pair<int, int>(-1, -1),std::pair<int, int>(-1, -1)); // Initialize with an invalid move

        std::unique_ptr<Board> tempBoard = std::unique_ptr<Board>(board.clone());
        std::vector<Move> moves = tempBoard->generateMoves();
        for (const Move &move: moves) {
            tempBoard->movePiece(move.from.first, move.from.second, move.to.first, move.to.second);
            int moveValue = minimax(*tempBoard, depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(),
                                    false);
            tempBoard->movePiece(move.to.first, move.to.second, move.from.first, move.from.second);
            if (moveValue > bestValue) {
                bestValue = moveValue;
                bestMove = move;
            }
        }
        return bestMove;
    }

// Iterative deepening search
void botMove(Board& board, int depth) {
    Move bestMove = findBestMove(board, depth);
    board.movePiece(bestMove.from.first, bestMove.from.second, bestMove.to.first, bestMove.to.second);
    Game::playerToMove = ColorType::WHITE;
}
