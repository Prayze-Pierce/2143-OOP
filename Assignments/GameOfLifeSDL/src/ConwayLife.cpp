#include "ConwayLife.hpp"
#include <random>

ConwayLife::ConwayLife(int rows, int cols)
    : rows(rows), cols(cols),
      grid(rows, std::vector<Cell>(cols, 0)),
      nextGrid(rows, std::vector<Cell>(cols, 0))
{
}

int ConwayLife::countNeighbors(int r, int c) const
{
    int count = 0;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (grid[nr][nc]) {
                    ++count;
                }
            }
        }
    }
    return count;
}

void ConwayLife::step()
{
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int n = countNeighbors(r, c);
            bool alive = grid[r][c] != 0;

            bool nextAlive = false;
            if (alive) {
                nextAlive = (n == 2 || n == 3);
            } else {
                nextAlive = (n == 3);
            }

            nextGrid[r][c] = nextAlive ? 1 : 0;
        }
    }

    grid.swap(nextGrid);
}

void ConwayLife::clear()
{
    for (auto &row : grid) {
        std::fill(row.begin(), row.end(), 0);
    }
}

void ConwayLife::randomize(double aliveChance)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            grid[r][c] = (dist(gen) < aliveChance) ? 1 : 0;
        }
    }
}

