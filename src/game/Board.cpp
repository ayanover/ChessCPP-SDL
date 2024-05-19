#include "../../include/Board.hpp"
#include "../../include/pieces/Pawn.hpp"

const int TILE_SIZE = 100;

Board::Board(SDL_Renderer* renderer) : renderer(renderer) {
    initialize();
}

void Board::initialize() {
    // Resize the board to 8x8 and initialize with nullptrs
    board.resize(8, std::vector<Piece*>(8, nullptr));

    // Example of placing pieces on the board
    for (int i = 0; i < 8; ++i) {
        board[1][i] = new Pawn(Piece::WHITE);
        board[6][i] = new Pawn(Piece::BLACK);
    }

    // Place other pieces, e.g., Rooks, Knights, Bishops, Queen, and King
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
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
            }
            SDL_RenderFillRect(renderer, &rect);

            if (board[y][x] != nullptr) {
                board[y][x]->display(renderer, rect.x, rect.y, TILE_SIZE, TILE_SIZE);
            }
        }
    }
}

