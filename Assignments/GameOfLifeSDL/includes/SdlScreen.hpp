#pragma once

#include <SDL.h>
#include "Screen.hpp"
#include "grid.hpp"

class SdlScreen : public Screen {
public:
    SdlScreen(int windowWidth, int windowHeight, int cellSize);
    ~SdlScreen() override;

    void render(const Grid &grid) override;
    void pause(int ms) override;

    int getCellSize() const { return cellSize; }

private:
    int windowWidth;
    int windowHeight;
    int cellSize;

    SDL_Window   *window   = nullptr;
    SDL_Renderer *renderer = nullptr;
};

