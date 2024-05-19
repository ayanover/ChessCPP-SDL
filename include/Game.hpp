#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Menu.hpp"

class Game {
public:
    Game(SDL_Renderer* renderer);
    void start();

private:
    void gameLoop();
    SDL_Renderer* renderer;
    Board board;
    bool isGameRunning;
    VisualHelper& visualHelper;
};

#endif // GAME_HPP
