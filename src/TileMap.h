#pragma once
#include <vector>
#include <string>
#include <raylib.h>

class TileMap
{
public:
    bool LoadFromFile(const std::string& jsonPath);
    void Draw() const;

    bool IsSolid(int x, int y) const;

    int GetTileSize() const { return tileSize; }

private:
    Texture2D tileset{};

    int tileSize{0};
    int mapWidth{0};
    int mapHeight{0};
    int tilesPerRow{0};

    std::vector<int> tiles;
};