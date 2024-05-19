//
// Created by rexiv on 18.05.2024.
//
#ifndef CHESS_VISUALHELPER_HPP
#define CHESS_VISUALHELPER_HPP

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>

class VisualHelper {
public:
    static VisualHelper& getInstance() {
        static VisualHelper instance;
        return instance;
    }

    VisualHelper(const VisualHelper&) = delete;
    VisualHelper& operator=(const VisualHelper&) = delete;

    bool initialize(const std::string& fontPath, int fontSize, SDL_Window* window, SDL_Renderer* renderer);
    void cleanup();

    void drawRectangle(int x, int y, int width, int height, SDL_Color color, const std::string& text, SDL_Color textColor, std::function<void()> onClick);

private:
    VisualHelper() : font(nullptr), renderer(nullptr), window(nullptr) {}

    TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Window* window;
};

extern VisualHelper& visualHelper;

#endif //CHESS_VISUALHELPER_HPP
