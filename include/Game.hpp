#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"

class Game {
public:
    explicit Game();
    void runGame();
public:
    static void setPlayerColor(ColorType playerColor) {m_PlayerColor = playerColor;}
    static ColorType getPlayerColor() {return  m_PlayerColor;}
    static void setPlayerToMove(ColorType playerToMove) {m_PlayerToMove = playerToMove;}
    static ColorType getPlayerToMove() {return m_PlayerToMove;}

private:
    void gameLoop();
    void initializeWindow();
    void initializeRenderer();
    void handleEvent(SDL_Event &e, bool &running);
private:
    SDL_Window *m_Window{};
    SDL_Renderer* m_Renderer{};
    Board *m_Board;

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 640;

    static ColorType m_PlayerColor;
    static ColorType m_PlayerToMove;
    bool m_IsMoveInitialized = false;
    Piece *m_clickedPiece;
    std::vector<std::pair<int, int>> m_PossibleMoves;
};

#endif // GAME_HPP
