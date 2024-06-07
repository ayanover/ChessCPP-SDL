#include <iostream>
#include <SDL_image.h>
#include "../../include/Game.hpp"
#include "../../include/ai/ai.h"
#include "../../include/RendererManager.hpp"
#include "../../include/Menu.hpp"

Game::Game(): m_clickedPiece(nullptr)
{
    initializeWindow();
    initializeRenderer();
    m_Board = new Board(m_Renderer);
    Menu menu(m_Renderer, *this);
    menu.display();
    setPlayerToMove(ColorType::WHITE);
    setPlayerColor(ColorType::WHITE);
}

ColorType Game::m_PlayerToMove = ColorType::WHITE;
ColorType Game::m_PlayerColor = ColorType::WHITE;
void Game::initializeWindow()
{
    SDL_Init(SDL_INIT_VIDEO);
    m_Window = SDL_CreateWindow("Chess Game",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          SCREEN_WIDTH, SCREEN_HEIGHT,
                          SDL_WINDOW_SHOWN);


    SDL_Surface* icon = IMG_Load("../assets/Pieces/white-king.png");
    if (icon == nullptr)
    {
        std::cerr << "Unable to load icon! IMG_Error: " << IMG_GetError() << std::endl;
        IMG_Quit();
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
        exit(1);
    }
    SDL_SetWindowIcon(m_Window, icon);
    SDL_FreeSurface(icon);
}

void Game::initializeRenderer()
{
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    RendererManager::getInstance().setRenderer(m_Renderer);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
}


void Game::runGame()
{
    gameLoop();
}

void Game::gameLoop()
{
    bool running = true;
    SDL_Event e;
    while (running)
    {
        if(m_PlayerToMove != m_PlayerColor)
        {
            botMove(*m_Board, 3);
        }
        while (SDL_PollEvent(&e) != 0)
        {
            handleEvent(e, running);
        }
        SDL_SetRenderDrawColor(m_Renderer, 50, 50, 50, 255);
        SDL_RenderClear(m_Renderer);

        m_Board->display();
        m_Board->displayPossibleMoves(m_PossibleMoves);
        m_Board->displayPieces();

        SDL_RenderPresent(m_Renderer);
    }
}

void Game::handleEvent(SDL_Event& e, bool& running)
{
    switch (e.type)
    {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            int x, y;
            SDL_GetMouseState(&x, &y);
            if(m_IsMoveInitialized)
            {
                if(m_clickedPiece->getPosX() == x / 80 && m_clickedPiece->getPosY() == y / 80)
                {
                    m_IsMoveInitialized = false;
                    m_PossibleMoves.clear();
                }
                else if(m_Board->movePiece(m_clickedPiece->getPosX(), m_clickedPiece->getPosY(), x / 80, y / 80, true))
                {
                    m_IsMoveInitialized = false;
                    m_PossibleMoves.clear();
                    if(m_PlayerToMove == ColorType::WHITE)
                    {
                        m_PlayerToMove = ColorType::BLACK;
                    }
                    else
                    {
                        m_PlayerToMove = ColorType::WHITE;
                    }
                }
                else
                {
                    m_IsMoveInitialized = false;
                    m_PossibleMoves.clear();
                    m_clickedPiece = m_Board->getPieceAt(x / 80, y / 80);
                    if (m_clickedPiece != nullptr)
                    {
                        m_PossibleMoves = m_clickedPiece->calculatePossibleMoves(*m_Board, true, true);
                        if(!m_PossibleMoves.empty())
                        {
                            m_IsMoveInitialized = true;
                        }
                    }
                }
            }
            else
            {
                m_clickedPiece = m_Board->getPieceAt(x / 80, y / 80);
                if (m_clickedPiece != nullptr)
                {
                    m_PossibleMoves = m_clickedPiece->calculatePossibleMoves(*m_Board, true, true);
                    if(!m_PossibleMoves.empty())
                    {
                        m_IsMoveInitialized = true;
                    }
                }
            }
    }
    if (e.type == SDL_QUIT)
    {
        running = false;
    }
}


