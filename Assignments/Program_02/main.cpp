#include <SDL.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "json.hpp"
using json = nlohmann::json;

int main(int argc, char* argv[]) {

    // -----------------------------
    // Load JSON Patterns
    // -----------------------------
    std::ifstream f("patterns.json");
    if (!f.is_open()) {
        std::cerr << "Could not open patterns.json\n";
        return 1;
    }

    json data = json::parse(f);

    // Default pattern
    std::string patternName = "glider";

    // Bonus: allow command-line pattern selection
    if (argc > 1)
        patternName = argv[1];

    if (!data.contains(patternName)) {
        std::cerr << "Pattern not found.\n";
        return 1;
    }

    auto pattern = data[patternName]["coords"];

    // -----------------------------
    // Config
    // -----------------------------
    const int cellSize = 10;
    const int windowWidth = 500;
    const int windowHeight = 500;

    // -----------------------------
    // Init SDL
    // -----------------------------
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Pattern Renderer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // -----------------------------
    // Random Cell Color
    // -----------------------------
    std::srand(std::time(nullptr));
    int r = std::rand() % 256;
    int g = std::rand() % 256;
    int b = std::rand() % 256;

    // -----------------------------
    // Center Pattern
    // -----------------------------
    // First find min/max of the pattern
    int minX = 9999, minY = 9999;
    int maxX = -9999, maxY = -9999;

    for (auto& cell : pattern) {
        int x = cell[0];
        int y = cell[1];
        minX = std::min(minX, x);
        minY = std::min(minY, y);
        maxX = std::max(maxX, x);
        maxY = std::max(maxY, y);
    }

    int patternWidth = (maxX - minX + 1) * cellSize;
    int patternHeight = (maxY - minY + 1) * cellSize;

    int offsetX = (windowWidth - patternWidth) / 2;
    int offsetY = (windowHeight - patternHeight) / 2;

    // -----------------------------
    // Main Loop
    // -----------------------------
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
        }

        // Clear background
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        // Draw the pattern
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);

        for (auto& cell : pattern) {
            int px = offsetX + (cell[0] - minX) * cellSize;
            int py = offsetY + (cell[1] - minY) * cellSize;

            SDL_Rect rect = { px, py, cellSize, cellSize };
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

