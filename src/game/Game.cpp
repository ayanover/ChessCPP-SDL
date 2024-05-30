#include "../../include/Game.hpp"
#include "../../include/VisualHelper.hpp"
#include "../../include/ai/ai.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

Game::Game(SDL_Renderer* renderer)
        :board( new Board(renderer)),
        renderer(renderer),
        isGameRunning(false) {;;
}

ColorType Game::PlayerColor = ColorType::WHITE;
ColorType Game::playerToMove = ColorType::WHITE;

void Game::start() {
    gameLoop();
}

void Game::gameLoop() {
    bool running = true;
    SDL_Event e;

    while (running) {
        if(playerToMove != PlayerColor){
            botMove(*board, 2);
        }
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    if(isMoveInitialized){
                        if(clickedPiece->getPosX() == x / 80 && clickedPiece->getPosY()==y/80){
                            isMoveInitialized = false;
                            possibleMoves.clear();
                        }
                        else if(board->movePiece(clickedPiece->getPosX(), clickedPiece->getPosY(),x / 80, y / 80, true)){
                            isMoveInitialized = false;
                            possibleMoves.clear();
                            if(playerToMove == ColorType::WHITE){
                                 playerToMove = ColorType::BLACK;
                            }
                            else{
                                playerToMove = ColorType::WHITE;
                            }
                        }
                        else{
                            isMoveInitialized = false;
                            possibleMoves.clear();
                            clickedPiece = board->getPieceAt(x / 80, y / 80);
                            if (clickedPiece != nullptr) {
                                possibleMoves = clickedPiece->calculatePossibleMoves(*board);
                                if(!possibleMoves.empty()){
                                    isMoveInitialized = true;
                                }
                            }
                        }
                    } else {
                        clickedPiece = board->getPieceAt(x / 80, y / 80);
                        if (clickedPiece != nullptr) {
                            possibleMoves = clickedPiece->calculatePossibleMoves(*board);
                            if(!possibleMoves.empty()){
                                isMoveInitialized = true;
                            }
                        }
                    }
            }
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);

        board->display();
        board->displayPossibleMoves(possibleMoves);

        SDL_RenderPresent(renderer);
    }
}



