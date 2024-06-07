//
// Created by rexiv on 06.06.2024.
//

#ifndef CHESS_MENU_HPP
#define CHESS_MENU_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "Game.hpp"

class Menu {
public:
    Menu(SDL_Renderer* renderer, Game& game);
    ~Menu();
    bool loadMedia();
    void display();
private:
    Game& m_Game;
    SDL_Renderer* m_Renderer;
    TTF_Font* m_Font;
    SDL_Rect m_StartButton;
    SDL_Rect m_OptionsButton;
    SDL_Rect m_QuitButton;

    SDL_Texture* initializeTexture(const std::string &path);
    void renderText(const char* text, int x, int y, SDL_Color color);
};

#endif
