#include "TileMap.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <raylib.h>

bool TileMap::LoadFromFile(const std::string& jsonPath)
{
    std::ifstream file(jsonPath);
    if (!file.is_open())
    {
        TraceLog(LOG_ERROR, "Failed to open map file: %s", jsonPath.c_str());
        return false;
    }

    nlohmann::json data;
    file >> data;

    std::cout << "===== TILEMAP DEBUG DUMP =====\n";
    std::cout << data.dump(2) << "\n";
    std::cout << "==============================\n";

    // ----------------------------
    // BASIC MAP INFO
    // ----------------------------
    mapWidth  = data.value("width", 0);
    mapHeight = data.value("height", 0);
    tileSize  = data.value("tilewidth", 0);

    std::cout << "[DEBUG] Map size: " << mapWidth << "x" << mapHeight << "\n";
    std::cout << "[DEBUG] Tile size: " << tileSize << "\n";

    if (mapWidth <= 0 || mapHeight <= 0 || tileSize <= 0)
    {
        TraceLog(LOG_ERROR, "Invalid map dimensions");
        return false;
    }

    // ----------------------------
    // TILESET PATH (from JSON)
    // ----------------------------
    if (!data.contains("tilesets") || data["tilesets"].empty())
    {
        TraceLog(LOG_ERROR, "No tilesets found");
        return false;
    }

    std::string imagePath = data["tilesets"][0].value("image", "");

    std::cout << "[DEBUG] Raw tileset image path: " << imagePath << "\n";

    if (imagePath.empty())
    {
        TraceLog(LOG_ERROR, "Tileset image path is empty");
        return false;
    }

    // ----------------------------
    // LOAD TEXTURE
    // ----------------------------
    if (!FileExists(imagePath.c_str()))
    {
        TraceLog(LOG_ERROR, "Texture file NOT FOUND: %s", imagePath.c_str());
    }

    tileset = LoadTexture(imagePath.c_str());

    std::cout << "[DEBUG] Texture ID: " << tileset.id << "\n";
    std::cout << "[DEBUG] Texture size: " << tileset.width << "x" << tileset.height << "\n";

    if (tileset.id == 0)
    {
        TraceLog(LOG_ERROR, "Failed to load tileset texture");
        return false;
    }

    tilesPerRow = tileset.width / tileSize;

    std::cout << "[DEBUG] Tiles per row: " << tilesPerRow << "\n";

    // ----------------------------
    // LAYERS
    // ----------------------------
    if (!data.contains("layers") || data["layers"].empty())
    {
        TraceLog(LOG_ERROR, "No layers found");
        return false;
    }

    const auto& layer = data["layers"][0];

    if (!layer.contains("data"))
    {
        TraceLog(LOG_ERROR, "Layer missing data field");
        return false;
    }

    tiles = layer["data"].get<std::vector<int>>();

    std::cout << "[DEBUG] Tile count: " << tiles.size() << "\n";
    std::cout << "[DEBUG] Expected: " << mapWidth * mapHeight << "\n";

    if (tiles.empty())
    {
        TraceLog(LOG_ERROR, "Tile data empty");
        return false;
    }

    if (tiles.size() != (size_t)(mapWidth * mapHeight))
    {
        TraceLog(LOG_WARNING, "Tile data size mismatch (Tiled export issue?)");
    }

    return true;
}

void TileMap::Draw() const
{
    // check if tileset loading
    // if (tileset.id == 0)
    // {
    //     TraceLog(LOG_ERROR, "Draw called but tileset not loaded!");
    //     return;
    // }

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            int index = y * mapWidth + x;

            if (index < 0 || index >= (int)tiles.size())
                continue;

            int tileID = tiles[index];

            if (tileID == 0) continue;

            tileID -= 1; // Tiled is 1-based

            int srcX = (tileID % tilesPerRow) * tileSize;
            int srcY = (tileID / tilesPerRow) * tileSize;

            Rectangle src = {
                (float)srcX,
                (float)srcY,
                (float)tileSize,
                (float)tileSize
            };

            Vector2 pos = {
                (float)(x * tileSize),
                (float)(y * tileSize)
            };

            if (tileID != 0)
            {
                std::cout << "Drawing tile at: " << x << "," << y << "\n";
                break;
            }

            DrawTextureRec(tileset, src, pos, WHITE);
        }
    }
}

bool TileMap::IsSolid(int x, int y) const
{
    if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight)
        return true;

    int index = y * mapWidth + x;

    if (index < 0 || index >= (int)tiles.size())
        return true;

    return tiles[index] != 0;
}