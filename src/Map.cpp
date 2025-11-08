#include "Map.h"
#include <algorithm>
#include <random>
#include <ctime>

Map::Map() : width(0), height(0), playerX(0), playerY(0) {}

void Map::LoadDefault() {
    // Default level 1 map; player symbol rendered separately
    const char* rows[] = {
        "############################",
        "#^^^^^^^^^^^^^^^^^^^^^^^^^^#",
        "#^^~~~~~~~^^^^^^~~~~~~~^^^#",
        "#^^~~~~~~~^^^^^^~~~~~~~^^^#",
        "#^^~~~~~~~~~~~~~~~~~~~~~~^#",
        "#^^~~~~E~~~~~~~~~~~~~~~^^^#",
        "#^^~~~~~~~~~~~~~~~~~~~~~~^#",
        "#^^~~~~~~~^^^^^^~~~~~~~^^^#",
        "#^^~~~~~~~^^^^^^~~~~~~~^^^#",
        "#^^~~~~~~~~~~~~B~~~~~~~~^^#",
        "#^^^^^^^^^^^^^^^^^^^^^^^^^#",
        "############################"
    };
    grid.assign(rows, rows + sizeof(rows)/sizeof(rows[0]));
    height = (int)grid.size();
    width = (int)grid[0].size();

    // Place player at a default open tile near top-left interior
    playerX = 2; playerY = 2;
}

bool Map::InBounds(int x, int y) const {
    return y >= 0 && y < height && x >= 0 && x < width;
}

char Map::GetTile(int x, int y) const {
    if (!InBounds(x, y)) return '#';
    return grid[y][x];
}

void Map::SetTile(int x, int y, char c) {
    if (!InBounds(x, y)) return;
    grid[y][x] = c;
}

bool Map::IsWalkable(int x, int y) const {
    char t = GetTile(x, y);
    return t != '#';
}

void Map::TryMove(int dx, int dy) {
    int nx = playerX + dx;
    int ny = playerY + dy;
    if (IsWalkable(nx, ny)) {
        playerX = nx;
        playerY = ny;
    }
}

void Map::RandomizeEvents(int eventCount, bool randomizeBoss) {
    // Collect candidate floor tiles (not walls '#', not edges)
    std::vector<std::pair<int,int>> candidates;
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            char c = grid[y][x];
            if (c != '#') {
                candidates.emplace_back(x, y);
            }
        }
    }
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::shuffle(candidates.begin(), candidates.end(), rng);

    // Clear existing E and maybe B
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == 'E' || (randomizeBoss && grid[y][x] == 'B')) {
                grid[y][x] = '^';
            }
        }
    }

    // Place events
    int placedE = 0;
    for (auto &p : candidates) {
        if (placedE >= eventCount) break;
        // Avoid placing on player spawn
        if (p.first == playerX && p.second == playerY) continue;
        if (GetTile(p.first, p.second) != '#') {
            SetTile(p.first, p.second, 'E');
            placedE++;
        }
    }

    // Place boss (one)
    if (randomizeBoss && !candidates.empty()) {
        for (auto &p : candidates) {
            if (GetTile(p.first, p.second) != '#') {
                SetTile(p.first, p.second, 'B');
                break;
            }
        }
    }
}
