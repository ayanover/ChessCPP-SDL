//
// Created by rexiv on 25.05.2024.
//

#pragma once
#include <utility>

struct Move {
    std::pair<int, int> from;
    std::pair<int, int> to;

    // Default constructor
    Move() : from(-1, -1), to(-1, -1) {}

    // Constructor with parameters - pairs are cheap to copy, no need for std::move
    Move(std::pair<int, int> f, std::pair<int, int> t) : from(f), to(t) {}

    // Comparison operators for easier testing and sorting
    bool operator==(const Move& other) const {
        return from == other.from && to == other.to;
    }

    bool operator!=(const Move& other) const {
        return !(*this == other);
    }
};
