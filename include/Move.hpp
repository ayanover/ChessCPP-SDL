//
// Created by rexiv on 25.05.2024.
//

#pragma once
#include <utility>

struct Move {
    std::pair<int,int> from;
    std::pair<int,int> to;

    Move(std::pair<int,int> f, std::pair<int,int> t) : from(std::move(f)), to(std::move(t)) {}
};
