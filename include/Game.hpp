#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Menu.hpp"

class Game {
public:
    Game(SDL_Renderer* renderer);
    void start();
    static ColorType PlayerColor;
    static ColorType playerToMove;

private:
    bool isMoveInitialized = false;
    Piece *clickedPiece;
    std::vector<std::pair<int, int>> possibleMoves;
    void gameLoop();
    SDL_Renderer* renderer;
    Board board;

    bool isGameRunning;

};

#endif // GAME_HPP
