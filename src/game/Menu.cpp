#include "../../include/Menu.hpp"
#include <iostream>

Menu::Menu(SDL_Renderer *renderer, int screenWidth, int screenHeight, VisualHelper &visualHelper)
        : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight), startGame(false), font(nullptr), cursor(nullptr), defaultCursor(nullptr),
          visualHelper(visualHelper) {
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont("../assets/Saira.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    menuItems.push_back("Start Game");
    menuItems.push_back("Exit");

    textRect.w = 300;
    textRect.h = 100;

    cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND); // Create hand cursor
    defaultCursor = SDL_GetDefaultCursor(); // Save the default cursor
}

Menu::~Menu() {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_FreeCursor(cursor);
}

bool Menu::gameShouldStart() const {
    return startGame;
}

void Menu::handleEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        for (int i = 0; i < menuItems.size(); ++i) {
            if (mouseX >= screenWidth / 2 - textRect.w / 2 &&
                mouseX <= screenWidth / 2 - textRect.w / 2 + textRect.w &&
                mouseY >= screenHeight / 2 - textRect.h / 2 + i * 60 &&
                mouseY <= screenHeight / 2 - textRect.h / 2 + i * 60 + textRect.h) {
                if (i == 0) {
                    startGame = true;
                } else if (i == 1) {
                    std::cout << "Exiting game..." << std::endl;
                    exit(0);
                }
            }
        }
    }

    if (event.type == SDL_MOUSEMOTION) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool overText = false;

        for (int i = 0; i < menuItems.size(); ++i) {
            if (mouseX >= screenWidth / 2 - textRect.w / 2 &&
                mouseX <= screenWidth / 2 - textRect.w / 2 + textRect.w &&
                mouseY >= screenHeight / 2 - textRect.h / 2 + i * 60 &&
                mouseY <= screenHeight / 2 - textRect.h / 2 + i * 60 + textRect.h) {
                //visualHelper.setCursor(VisualHelper::CursorType::HAND); // Set the hand cursor
                overText = true;
                break;
            }
        }

        if (!overText) {
            //visualHelper.setCursor(VisualHelper::CursorType::DEFAULT);
        }
    }
}

void Menu::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render menu items
    for (int i = 0; i < menuItems.size(); ++i) {
        int x = screenWidth / 2 - textRect.w / 2;
        int y = screenHeight / 2 - textRect.h / 2 + i * 60;
        visualHelper.drawRectangle(x, y, textRect.w, textRect.h, {0, 0, 0, 255}, menuItems[i], {255, 255, 255, 255},
                                   [this, i]() {
                                       if (i == 0) {
                                           startGame = true;
                                       } else if (i == 1) {
                                           std::cout << "Exiting game..." << std::endl;
                                           exit(0);
                                       }
                                   });
    }
 
    SDL_RenderPresent(renderer);
}
