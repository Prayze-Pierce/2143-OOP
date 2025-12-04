#pragma once
#include "grid.hpp"

class CellularAutomaton {
public:
    virtual ~CellularAutomaton() = default;

    virtual void step() = 0;
    virtual Grid &getGrid() = 0;
    virtual const Grid &getGrid() const = 0;
};

