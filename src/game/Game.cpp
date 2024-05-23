#include "../../include/Game.hpp"
#include "../../include/VisualHelper.hpp"

const int SCREEN_WIDTH = 740;
const int SCREEN_HEIGHT = 740;

Game::Game(SDL_Renderer* renderer)
        : board(renderer),
        renderer(renderer),
        isGameRunning(false) {}



void Game::start() {
    gameLoop();
}

void Game::gameLoop() {
    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);

        board.display();

        SDL_RenderPresent(renderer);
    }
}
