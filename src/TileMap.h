#pragma once
#include <vector>
#include <string>
#include <raylib.h>
#include <unordered_set>

// Set to 1 to enable verbose load/draw debug logging to stdout.
#ifndef TILEMAP_DEBUG
#define TILEMAP_DEBUG 0
#endif

class TileMap
{
public:
    bool LoadFromFile(const std::string& jsonPath);
    void Draw() const;

    [[nodiscard]] bool IsSolid(int x, int y) const;

    [[nodiscard]] bool IsSolidAtWorldPos(Vector2 worldPos) const;

    [[nodiscard]] int GetTileSize() const { return tileSize; }

private:
    Texture2D tileset{};

    int tileSize{0};
    int mapWidth{0};
    int mapHeight{0};
    int tilesPerRow{0};

    int collisionLayerIndex{0};

    std::vector<std::vector<int>> layers;
    std::vector<std::string> layerNames;
    std::unordered_set<int> solidTiles;
};