//
// Created by rexiv on 18.05.2024.
//

#include "../../include/VisualHelper.hpp"
#include <iostream>

bool VisualHelper::initialize(const std::string& fontPath, int fontSize, SDL_Window* window, SDL_Renderer* renderer) {
    if (TTF_Init() == -1) {
        SDL_Log("Failed to initialize TTF: %s", TTF_GetError());
        return false;
    }

    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return false;
    }

    this->window = window;
    this->renderer = renderer;

    return true;
}

void VisualHelper::cleanup() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    TTF_Quit();
}

// Implementation of the drawRectangle method
void VisualHelper::drawRectangle(int x, int y, int width, int height, SDL_Color color, const std::string& text, SDL_Color textColor, std::function<void()> onClick) {
    // Draw rectangle
    SDL_Rect rect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);

    // Render text
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = { x + (width - textSurface->w) / 2, y + (height - textSurface->h) / 2, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    // Handle click event if onClick function is provided
    if (onClick && window) {
        int mouseX, mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
                onClick();
            }
        }
    }
}