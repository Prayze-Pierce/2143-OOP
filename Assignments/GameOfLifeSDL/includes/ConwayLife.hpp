#pragma once
#include "CellularAutomaton.hpp"

class ConwayLife : public CellularAutomaton {
public:
    ConwayLife(int rows, int cols);

    void step() override;

    Grid &getGrid() override { return grid; }
    const Grid &getGrid() const override { return grid; }

    void clear();
    void randomize(double aliveChance = 0.2);

private:
    int rows;
    int cols;
    Grid grid;
    Grid nextGrid;

    int countNeighbors(int r, int c) const;
};
