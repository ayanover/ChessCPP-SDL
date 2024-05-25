// RendererManager.hpp
#pragma once
#include <SDL.h>

class RendererManager {
public:
    static RendererManager& getInstance() {
        static RendererManager instance; // Guaranteed to be destroyed.
        return instance;
    }

    RendererManager(RendererManager const&) = delete;
    void operator=(RendererManager const&) = delete;

    void setRenderer(SDL_Renderer* renderer) {
        this->renderer = renderer;
    }

    SDL_Renderer* getRenderer() const {
        return renderer;
    }

private:
    RendererManager() {}
    SDL_Renderer* renderer;
};