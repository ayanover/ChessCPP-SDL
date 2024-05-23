//
// Created by rexiv on 17.05.2024.
//
#include <iostream>
#include "../../include/Piece.hpp"
#include <SDL_image.h>

Piece::Piece(ColorType colorType, PieceType pieceType) : color(colorType), piece(pieceType) {

    std::string path = getPieceTexturePath(pieceType);
    SDL_Texture* texture = nullptr;
    initializeTexture(renderer, path, texture);
    textures[pieceType] = texture;
}

Piece::~Piece(){
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
}

ColorType Piece::getColor() const {
    return color;
}

PieceType Piece::getPiece() const {
    return piece;
}

void Piece::initializeTexture(SDL_Renderer *renderer, const std::string &path, SDL_Texture *&texture) {
    texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
}


void Piece::display(SDL_Renderer* renderer, PieceType type, int x, int y) {
    SDL_Texture* texture = textures[type];
    if (texture) {
        SDL_Rect dstrect = { x, y, 64, 64 }; // Example destination rectangle
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    }
}

std::string Piece::getPieceTexturePath(PieceType type) {
    switch(type) {
        case PieceType::PAWN: return "assets/pawn.png";
        case PieceType::KNIGHT: return "assets/knight.png";
        case PieceType::BISHOP: return "assets/bishop.png";
        case PieceType::ROOK: return "assets/rook.png";
        case PieceType::QUEEN: return "assets/queen.png";
        case PieceType::KING: return "assets/king.png";
        default: return "";
    }
}