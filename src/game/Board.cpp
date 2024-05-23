#include "../../include/Board.hpp"
#include "enums/ColorType.hpp"
#include "enums/PieceType.hpp"

const int TILE_SIZE = 80;

Board::Board(SDL_Renderer* renderer) : renderer(renderer) {
    initialize();
}

void Board::initialize() {
    board.resize(8, std::vector<Piece*>(8, nullptr));

    for (int i = 0; i < 8; ++i) {
        board[1][i] = new Piece(ColorType::WHITE, PieceType::PAWN);
        board[6][i] = new Piece(ColorType::BLACK, PieceType::PAWN);
    }
}

Board::~Board() {
    for (auto & row : board) {
        for (auto & piece : row) {
            delete piece;
            SDL_DestroyTexture(whiteTileTexture);
            SDL_DestroyTexture(blackTileTexture);
        }
    }
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

