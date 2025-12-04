#include <SDL.h>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "json.hpp"
#include "ArgsToJson.hpp"
#include "ConwayLife.hpp"
#include "SdlScreen.hpp"
#include "grid.hpp"

using json = nlohmann::json;

json LoadPatterns(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Failed to open pattern file: " + filename);
    }
    json j;
    in >> j;
    return j;
}

void ApplyPatternAt(Grid &grid,
                    const json &patterns,
                    const std::string &patternName,
                    int gx, int gy)
{
    if (!patterns.contains("shapes") || !patterns["shapes"].contains(patternName)) {
        std::cerr << "Pattern not found: " << patternName << "\n";
        return;
    }

    const json &shape = patterns["shapes"][patternName];
    if (!shape.contains("cells") || !shape["cells"].is_array()) {
        std::cerr << "Pattern '" << patternName << "' has no valid 'cells' array\n";
        return;
    }

    int rows = static_cast<int>(grid.size());
    int cols = rows > 0 ? static_cast<int>(grid[0].size()) : 0;

    for (const auto &cell : shape["cells"]) {
        int dx = cell.value("x", 0);
        int dy = cell.value("y", 0);

        int x = gx + dx;
        int y = gy + dy;

        if (x >= 0 && x < cols && y >= 0 && y < rows) {
            grid[y][x] = 1;
        }
    }
}

int main(int argc, char *argv[])
{
    try {
        json defaults = {
            {"window_width",  900},
            {"window_height", 900},
            {"cellSize",      12},
            {"frameDelayMs",  50}
        };

        json cli = ArgsToJson(argc, argv);
        json params = defaults;
        params.update(cli);

        int windowWidth  = params.value("window_width",  900);
        int windowHeight = params.value("window_height", 900);
        int cellSize     = params.value("cellSize",      12);
        int frameDelayMs = params.value("frameDelayMs",  50);

        if (cellSize <= 0) {
            throw std::runtime_error("cellSize must be > 0");
        }

        int rows = windowHeight / cellSize;
        int cols = windowWidth  / cellSize;

        if (rows <= 0 || cols <= 0) {
            throw std::runtime_error("Window too small for given cellSize.");
        }

        ConwayLife gol(rows, cols);
        gol.clear();

        SdlScreen screen(windowWidth, windowHeight, cellSize);

        json patterns = LoadPatterns("assets/shapes.json");

        bool running = true;
        bool paused  = true;
        Uint32 lastStepTime = SDL_GetTicks();
        int generation = 0;

        while (running) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN: {
                    SDL_Keycode key = e.key.keysym.sym;
                    if (key == SDLK_q || key == SDLK_ESCAPE) {
                        running = false;
                    } else if (key == SDLK_SPACE) {
                        paused = !paused;
                    } else if (key == SDLK_n) {
                        if (paused) {
                            gol.step();
                            generation++;
                        }
                    } else if (key == SDLK_r) {
                        gol.randomize();
                        generation = 0;
                    } else if (key == SDLK_c) {
                        gol.clear();
                        generation = 0;
                    } else if (key == SDLK_g) {
                        int mx, my;
                        SDL_GetMouseState(&mx, &my);
                        int gx = mx / cellSize;
                        int gy = my / cellSize;

                        Grid &grid = gol.getGrid();
                        ApplyPatternAt(grid, patterns, "glider", gx, gy);
                    } else if (key == SDLK_b) {
                        int mx, my;
                        SDL_GetMouseState(&mx, &my);
                        int gx = mx / cellSize;
                        int gy = my / cellSize;

                        Grid &grid = gol.getGrid();
                        ApplyPatternAt(grid, patterns, "blinker", gx, gy);
                    } else if (key == SDLK_o) {
                        int mx, my;
                        SDL_GetMouseState(&mx, &my);
                        int gx = mx / cellSize;
                        int gy = my / cellSize;

                        Grid &grid = gol.getGrid();
                        ApplyPatternAt(grid, patterns, "block", gx, gy);
                    }
                    break;
                }

                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        int mx = e.button.x;
                        int my = e.button.y;
                        int gx = mx / cellSize;
                        int gy = my / cellSize;

                        if (gx >= 0 && gx < cols && gy >= 0 && gy < rows) {
                            Grid &grid = gol.getGrid();
                            grid[gy][gx] = grid[gy][gx] ? 0 : 1;
                        }
                    }
                    break;

                default:
                    break;
                }
            }

            Uint32 now = SDL_GetTicks();
            if (!paused && now - lastStepTime >= static_cast<Uint32>(frameDelayMs)) {
                gol.step();
                generation++;
                lastStepTime = now;
            }

            screen.render(gol.getGrid());
            screen.pause(5);
        }

        return 0;
    } catch (const std::exception &ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }
}

