//
// Created by rexiv on 17.05.2024.
//
#include <iostream>
#include "../../include/Piece.hpp"
#include "../../include/Game.hpp"
#include <SDL_image.h>


Piece::Piece(ColorType colorType, PieceType pieceType, Board& parentBoard, int x, int y)
        : color(colorType), piece(pieceType), parentBoard(parentBoard), renderer(parentBoard.getRenderer()){
    posX = x;
    posY = y;
    std::string path = getPieceTexturePath();
    texture = nullptr;
    initializeTexture(path);
}

Piece::Piece(const Piece& other)
        : color(other.color), piece(other.piece), parentBoard(other.parentBoard), renderer(other.renderer),
          posX(other.posX), posY(other.posY), hasMoved(other.hasMoved) {
    initializeTexture(getPieceTexturePath());
}
Piece::~Piece(){
    if(texture != nullptr){
        SDL_DestroyTexture(texture);
    }
}
int Piece::getPosX() const {
    return posX;
}

int Piece::getPosY() const {
    return posY;
}

void Piece::setPosX(int x) {
    posX = x;
}

void Piece::setPosY(int y) {
    posY = y;
}

ColorType Piece::getColor() const {
    return color;
}

PieceType Piece::getPiece() const {
    return piece;
}

void Piece::initializeTexture(const std::string &path) {
    IMG_Init(IMG_INIT_PNG); // Initialize PNG loading
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == nullptr) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_FreeSurface(loadedSurface);
}


void Piece::display(SDL_Renderer* renderer, int x, int y) {
    if (texture) {
        SDL_Rect dstrect = { x, y, 80, 80 }; // Example destination rectangle
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    }
}

std::string Piece::getPieceTexturePath() {
    std::string path = "../assets/Pieces/";


    if(color == ColorType::BLACK) {
        path += "black-";
    } else {
        path += "white-";
    }
    switch(piece) {
        case PieceType::PAWN:
            path +="pawn";
            break;
        case PieceType::KNIGHT:
            path +="knight";
            break;
        case PieceType::BISHOP:
            path +="bishop";
            break;
        case PieceType::ROOK:
            path +="rook";
            break;
        case PieceType::QUEEN:
            path +="queen";
            break;
        case PieceType::KING:
            path +="king";
            break;
        default: return "";
    }

    path+= ".png";

    return path;
}

void Piece::setHasDoubleMoved(int oldX, int oldY, int newX, int newY) {
    if (piece == PieceType::PAWN && abs(newY - oldY) == 2) {
        hasDoubleMoved = true;
    } else {
        hasDoubleMoved = false;
    }
}

std::vector<std::pair<int, int>> Piece::calculatePossibleMoves(Board& board ,bool isRealMove) {
    if(color != Game::playerToMove && isRealMove){
        //return {};
    }
    switch (piece){
        case PieceType::PAWN:
            return calculatePawnMoves(*this, board, isRealMove);
        case PieceType::KNIGHT:
            return calculateKnightMoves(*this, board, isRealMove);
        case PieceType::BISHOP:
            return calculateBishopMoves(*this, board, isRealMove);
        case PieceType::ROOK:
            return calculateRookMoves(*this, board, isRealMove);
        case PieceType::QUEEN:
            return calculateQueenMoves(*this, board, isRealMove);
        case PieceType::KING:
            return calculateKingMoves(*this, board, isRealMove);
        default:
            return {};
    }
}