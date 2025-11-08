#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Map {
public:
    int width;
    int height;
    std::vector<std::string> grid; // each string is a row
    int playerX;
    int playerY;

    Map();

    void LoadDefault();
    void RandomizeEvents(int eventCount, bool randomizeBoss);

    bool InBounds(int x, int y) const;
    bool IsWalkable(int x, int y) const;
    char GetTile(int x, int y) const;
    void SetTile(int x, int y, char c);

    // Moves player if walkable
    void TryMove(int dx, int dy);
};

#endif // MAP_H
