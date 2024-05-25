//
// Created by rexiv on 17.05.2024.
//

#include <utility>
#include <limits>
#include <iostream>
#include "../../include/ai/ai.h"
#include "../../include/Game.hpp"

std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> minimax(Board& board, int depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0) {
        return {board.getScore(), {{-1, -1}, {-1, -1}}};
    }

    std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> bestMove = {
            isMaximizingPlayer ? -std::numeric_limits<int>::max() : std::numeric_limits<int>::max(),
            {{-1, -1}, {-1, -1}}
    };

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = board.getPieceAt(x, y);
            if (piece != nullptr && piece->getColor() == ColorType::BLACK) {
                for (const auto& move : piece->calculatePossibleMoves(board)) {
                    if (move.first >= 0 && move.first < 8 && move.second >= 0 && move.second < 8) {
                        std::unique_ptr<Board> newBoard = std::unique_ptr<Board>(board.clone());
                        newBoard->movePiece(x, y, move.first, move.second);
                        auto currentMove = minimax(*newBoard, depth - 1, alpha, beta, !isMaximizingPlayer);
                        currentMove.second = {{x, y}, {move.first, move.second}};

                        if (isMaximizingPlayer) {
                            if (currentMove.first > bestMove.first) {
                                bestMove = currentMove;
                            }
                            alpha = std::max(alpha, bestMove.first);
                        } else {
                            if (currentMove.first < bestMove.first) {
                                bestMove = currentMove;
                            }
                            beta = std::min(beta, bestMove.first);
                        }

                        if (beta <= alpha) {
                            return bestMove;
                        }
                    }
                }
            }
        }
    }

    return bestMove;
}

void botMove(Board& board) {
    auto bestMove = minimax(board, 2, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), true);
    if (bestMove.second.first.first != -1 && bestMove.second.first.second != -1) {
        int startX = bestMove.second.first.first;
        int startY = bestMove.second.first.second;
        int endX = bestMove.second.second.first;
        int endY = bestMove.second.second.second;

        board.movePiece(startX, startY, endX, endY);
        Game::playerToMove = ColorType::WHITE;
    } else {
        std::cout << "No valid move found." << std::endl;
    }
}
