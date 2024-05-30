#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "../include/Game.hpp"
#include "../include/RendererManager.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Chess Game",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    RendererManager::getInstance().setRenderer(renderer);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    SDL_Surface* icon = IMG_Load("../assets/Pieces/white-king.png");
    if (icon == nullptr) {
        std::cerr << "Unable to load icon! IMG_Error: " << IMG_GetError() << std::endl;
        IMG_Quit();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set the icon
    SDL_SetWindowIcon(window, icon);

    // Free the icon surface
    SDL_FreeSurface(icon);

    Game game(renderer);
    game.start();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

