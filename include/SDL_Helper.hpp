//
// Created by rexiv on 23.05.2024.
//

#ifndef CHESS_SDL_HELPER_HPP
#define CHESS_SDL_HELPER_HPP

#include <valarray>
#include "SDL.h"
#include "SDL_ttf.h"

void renderText(const char* text, int x, int y, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int mWidth = textSurface->w;
    int mHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    SDL_RenderCopy(renderer, mTexture, NULL, &renderQuad);
    SDL_DestroyTexture(mTexture);
}

void SDL_RenderDrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    auto plot = [&](int32_t x, int32_t y, Uint8 alpha)
    {
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, alpha); // Assuming white color for the circle
        SDL_RenderDrawPoint(renderer, x, y);
    };

    for (int32_t y = -radius; y <= radius; y++)
    {
        for (int32_t x = -radius; x <= radius; x++)
        {
            float distance = std::sqrt(static_cast<float>(x * x + y * y));
            if (distance <= radius)
            {
                Uint8 alpha = static_cast<Uint8>(255 * (1.0f - (distance - std::floor(distance))));
                plot(centreX + x, centreY + y, alpha);
            }
        }
    }
}

#endif //CHESS_SDL_HELPER_HPP
