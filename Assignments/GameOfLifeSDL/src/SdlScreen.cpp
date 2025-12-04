#include "SdlScreen.hpp"
#include <stdexcept>

SdlScreen::SdlScreen(int windowWidth, int windowHeight, int cellSize)
    : windowWidth(windowWidth), windowHeight(windowHeight), cellSize(cellSize)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
    }

    window = SDL_CreateWindow(
        "Conway's Game of Life (SDL)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
    }
}

SdlScreen::~SdlScreen()
{
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    SDL_Quit();
}

void SdlScreen::render(const Grid &grid)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    const int rows = static_cast<int>(grid.size());
    const int cols = rows > 0 ? static_cast<int>(grid[0].size()) : 0;

    SDL_Rect cellRect;
    cellRect.w = cellSize;
    cellRect.h = cellSize;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (grid[y][x]) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // alive
            } else {
                SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);   // dead
            }
            cellRect.x = x * cellSize;
            cellRect.y = y * cellSize;
            SDL_RenderFillRect(renderer, &cellRect);
        }
    }

    // grid lines
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    for (int x = 0; x <= cols; ++x) {
        int px = x * cellSize;
        SDL_RenderDrawLine(renderer, px, 0, px, rows * cellSize);
    }
    for (int y = 0; y <= rows; ++y) {
        int py = y * cellSize;
        SDL_RenderDrawLine(renderer, 0, py, cols * cellSize, py);
    }

    SDL_RenderPresent(renderer);
}

void SdlScreen::pause(int ms)
{
    SDL_Delay(ms);
}

