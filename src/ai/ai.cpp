#include <utility>
#include <algorithm>
#include <climits>
#include "../../include/ai/ai.h"
#include "../../include/Game.hpp"

std::vector<Move> generateMoves(Board& board, ColorType colorType) {
    std::vector<Move> moves;
    moves.reserve(40); // Average number of legal moves in chess

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = board.getPieceAt(x, y);
            if (piece != nullptr && piece->getColor() == colorType) {
                const std::vector<std::pair<int, int>>& possibleMoves = piece->calculatePossibleMoves(board);
                for (const auto &move : possibleMoves) {
                    // Bounds check (should already be valid from calculatePossibleMoves)
                    if (move.first >= 0 && move.first < 8 && move.second >= 0 && move.second < 8) {
                        moves.emplace_back(std::make_pair(x, y), move);
                    }
                }
            }
        }
    }
    return moves;
}

int minimax(Board& board, int depth, bool isMaximizingPlayer, int alpha, int beta) {
    if (depth == 0) {
        return board.getScore();
    }

    const ColorType currentColor = isMaximizingPlayer ? ColorType::WHITE : ColorType::BLACK;
    std::vector<Move> moves = generateMoves(board, currentColor);

    if (moves.empty()) {
        return board.getScore();
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        for (const auto& move : moves) {
            board.tempMovePiece(move.from.first, move.from.second, move.to.first, move.to.second);
            int eval = minimax(board, depth - 1, false, alpha, beta);
            board.revertMove(move.from.first, move.from.second, move.to.first, move.to.second);

            if (eval > maxEval) {
                maxEval = eval;
            }
            if (eval > alpha) {
                alpha = eval;
            }
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

            if (eval < minEval) {
                minEval = eval;
            }
            if (eval < beta) {
                beta = eval;
            }
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }
        return minEval;
    }
}

Move findBestMove(Board& board, int depth) {
    int bestValue = INT_MAX;
    Move bestMove; // Use default constructor
    std::vector<Move> moves = generateMoves(board, ColorType::BLACK);

    int alpha = INT_MIN;
    int beta = INT_MAX;

    for (const auto& move : moves) {
        board.tempMovePiece(move.from.first, move.from.second, move.to.first, move.to.second);
        int boardValue = minimax(board, depth, true, alpha, beta);
        board.revertMove(move.from.first, move.from.second, move.to.first, move.to.second);

        if (boardValue < bestValue) {
            bestValue = boardValue;
            bestMove = move;
            beta = boardValue; // Update beta for better pruning
        }
    }
    return bestMove;
}


void botMove(Board& board, int depth) {
    Move bestMove = findBestMove(board, depth);
    board.movePiece(bestMove.from.first, bestMove.from.second, bestMove.to.first, bestMove.to.second);
    Game::setPlayerToMove(ColorType::WHITE);
}
