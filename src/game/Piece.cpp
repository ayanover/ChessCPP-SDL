//
// Created by rexiv on 17.05.2024.
//
#include <iostream>
#include "../../include/Piece.hpp"
#include "../../include/MoveHelper.hpp"
#include "../../include/Game.hpp"
#include <SDL_image.h>


Piece::Piece(ColorType colorType, PieceType pieceType, SDL_Renderer* renderer, int x, int y)
        : color(colorType), piece(pieceType), renderer(renderer) {
    posX = x;
    posY = y;
    std::string path = getPieceTexturePath();
    texture = nullptr;
    initializeTexture(path);
}

Piece::Piece(Piece& other) : color(other.color), piece(other.piece), renderer(other.renderer), posX(other.posX), posY(other.posY), hasMoved(other.hasMoved) {
    initializeTexture(other.getPieceTexturePath());
}
Piece::~Piece(){
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
    texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
}


void Piece::display(SDL_Renderer* renderer, int x, int y) {
    if (texture) {
        SDL_Rect dstrect = { x, y, 75, 75 }; // Example destination rectangle
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    }
}

std::string Piece::getPieceTexturePath() {
    std::string path = "C:/Users/rexiv/CLionProjects/Chess/assets/Pieces/";
    switch(piece) {
        case PieceType::PAWN:
            path +="Pawn";
            break;
        case PieceType::KNIGHT:
            path +="Knight";
            break;
        case PieceType::BISHOP:
            path +="Bishop";
            break;
        case PieceType::ROOK:
            path +="Rook";
            break;
        case PieceType::QUEEN:
            path +="Queen";
            break;
        case PieceType::KING:
            path +="King";
            break;
        default: return "";
    }

    if(color == ColorType::BLACK) {
        path += "_B";
    }

    path+= ".png";

    return path;
}

std::vector<std::pair<int, int>> Piece::calculatePossibleMoves(){

    switch (piece){
        case PieceType::PAWN:
            return calculatePawnMoves(*this);
        case PieceType::KNIGHT:
            return calculateKnightMoves(*this);
        case PieceType::BISHOP:
            return calculateBishopMoves(*this);
        case PieceType::ROOK:
            return calculateRookMoves(*this);
        case PieceType::QUEEN:
            return calculateQueenMoves(*this);
        case PieceType::KING:
            return calculateKingMoves(*this);
        default:
            return {};
    }
}