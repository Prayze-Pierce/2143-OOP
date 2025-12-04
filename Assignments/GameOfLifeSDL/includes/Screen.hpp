#pragma once
#include "grid.hpp"

class Screen {
public:
    virtual ~Screen() = default;
    virtual void render(const Grid &grid) = 0;
    virtual void pause(int ms) = 0;
};
