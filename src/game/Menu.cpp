//
// Created by rexiv on 06.06.2024.
//

#include "../../include/Menu.hpp"
#include <iostream>

const int BUTTON_WIDTH = 230;
const int BUTTON_HEIGHT = 50;

Menu::Menu(SDL_Renderer* renderer, Game& game)
        : m_Renderer(renderer), m_Font(nullptr), m_Game(game),
          m_StartButton{200, 200, BUTTON_WIDTH, BUTTON_HEIGHT},
          m_OptionsButton{200, 260, BUTTON_WIDTH, BUTTON_HEIGHT},
          m_QuitButton{200, 320, BUTTON_WIDTH, BUTTON_HEIGHT} {
    loadMedia();
}

Menu::~Menu() {
    TTF_CloseFont(m_Font);
    m_Font = nullptr;
}

bool Menu::loadMedia() {
    TTF_Init();
    m_Font = TTF_OpenFont("../assets/Saira.ttf", 28);
    if (m_Font == NULL) {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    TTF_SetFontStyle(m_Font, TTF_STYLE_BOLD);
    return true;
}

void Menu::renderText(const char* text, int x, int y, SDL_Color color, short width, short height) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(m_Font, text, color);
    SDL_Texture* mTexture = SDL_CreateTextureFromSurface(m_Renderer, textSurface);
    int mWidth = width == 0? textSurface->w : width;
    int mHeight = height == 0? textSurface->h : height;
    SDL_FreeSurface(textSurface);

    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    SDL_RenderCopy(m_Renderer, mTexture, NULL, &renderQuad);
    SDL_DestroyTexture(mTexture);
}

void Menu::display() {
    bool quit = false;
    SDL_Event e;
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Color hoverColor = {255, 100, 100, 255};

    bool startButtonHover = false;
    bool optionsButtonHover = false;
    bool quitButtonHover = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > m_StartButton.x && x < m_StartButton.x + m_StartButton.w &&
                    y > m_StartButton.y && y < m_StartButton.y + m_StartButton.h) {
                    m_Game.runGame();
                    quit = true;
                    std::cout << "Start Game button clicked!" << std::endl;
                } else if (x > m_OptionsButton.x && x < m_OptionsButton.x + m_OptionsButton.w &&
                           y > m_OptionsButton.y && y < m_OptionsButton.y + m_OptionsButton.h) {
                    std::cout << "Options button clicked!" << std::endl;
                } else if (x > m_QuitButton.x && x < m_QuitButton.x + m_QuitButton.w &&
                           y > m_QuitButton.y && y < m_QuitButton.y + m_QuitButton.h) {
                    quit = true;
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                startButtonHover = (x > m_StartButton.x && x < m_StartButton.x + m_StartButton.w &&
                                    y > m_StartButton.y && y < m_StartButton.y + m_StartButton.h);
                optionsButtonHover = (x > m_OptionsButton.x && x < m_OptionsButton.x + m_OptionsButton.w &&
                                      y > m_OptionsButton.y && y < m_OptionsButton.y + m_OptionsButton.h);
                quitButtonHover = (x > m_QuitButton.x && x < m_QuitButton.x + m_QuitButton.w &&
                                   y > m_QuitButton.y && y < m_QuitButton.y + m_QuitButton.h);
            }
        }

        SDL_SetRenderDrawColor(m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_Renderer);

        SDL_Rect dstrect = {270, 40, 80, 80};
        SDL_RenderCopy(m_Renderer, initializeTexture("../assets/Pieces/white-king.png"), NULL, &dstrect);
        renderText("ChessSDL-CPP", 200, 130, textColor);
        renderText("by Aver", 200, 160, textColor, 70, 25);

        renderText("Start Game", m_StartButton.x + 30, m_StartButton.y + 5, startButtonHover ? hoverColor : textColor);
        renderText("Options", m_OptionsButton.x + 30, m_OptionsButton.y + 5, optionsButtonHover ? hoverColor : textColor);
        renderText("Quit", m_QuitButton.x + 30, m_QuitButton.y + 5, quitButtonHover ? hoverColor : textColor);

        SDL_SetRenderDrawColor(m_Renderer, startButtonHover ? 255 : 0, startButtonHover ? 0 : 0, 0, 255);
        SDL_RenderDrawRect(m_Renderer, &m_StartButton);

        SDL_SetRenderDrawColor(m_Renderer, optionsButtonHover ? 255 : 0, optionsButtonHover ? 0 : 0, 0, 255);
        SDL_RenderDrawRect(m_Renderer, &m_OptionsButton);

        SDL_SetRenderDrawColor(m_Renderer, quitButtonHover ? 255 : 0, quitButtonHover ? 0 : 0, 0, 255);
        SDL_RenderDrawRect(m_Renderer, &m_QuitButton);

        SDL_RenderPresent(m_Renderer);
    }
}


SDL_Texture* Menu::initializeTexture(const std::string &path) {
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, loadedSurface);
    if (texture == nullptr) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_FreeSurface(loadedSurface);
    return texture;
}


