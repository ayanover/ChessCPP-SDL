#include "SDL.h"
#include "SDL_ttf.h"
#include <vector>
#include <string>
#include "../src/game/enums/GameState.hpp"
#include "VisualHelper.hpp"

class Menu {
public:
    Menu(SDL_Renderer *renderer, int screenWidth, int screenHeight, VisualHelper &visualHelper);
    ~Menu();

    bool gameShouldStart() const;
    void handleEvents(SDL_Event& event);
    void render();

private:
    VisualHelper& visualHelper;
    void drawText(const std::string& text, int x, int y);

    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
    bool startGame;
    TTF_Font* font;
    SDL_Rect textRect;
    std::vector<std::string> menuItems;
    GameState gameState;

    SDL_Cursor* cursor; // Add cursor member
    SDL_Cursor* defaultCursor; // Add default cursor member
};
