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

#if TILEMAP_DEBUG
    std::cout << "===== TILEMAP DEBUG DUMP =====\n";
    std::cout << data.dump(2) << "\n";
    std::cout << "==============================\n";
#endif

    // ----------------------------
    // BASIC MAP INFO
    // ----------------------------
    mapWidth  = data.value("width", 0);
    mapHeight = data.value("height", 0);
    tileSize  = data.value("tilewidth", 0);

    if (mapWidth <= 0 || mapHeight <= 0 || tileSize <= 0)
    {
        TraceLog(LOG_ERROR, "Invalid map dimensions");
        return false;
    }

    // ----------------------------
    // TILESET
    // ----------------------------
    if (!data.contains("tilesets") || data["tilesets"].empty())
    {
        TraceLog(LOG_ERROR, "No tilesets found");
        return false;
    }

    std::string imagePath =
        data["tilesets"][0].value("image", "");

    if (imagePath.empty())
    {
        TraceLog(LOG_ERROR, "Tileset image path is empty");
        return false;
    }

    tileset = LoadTexture(imagePath.c_str());

    if (tileset.id == 0)
    {
        TraceLog(LOG_ERROR, "Failed to load tileset texture");
        return false;
    }

    tilesPerRow = tileset.width / tileSize;

    // ----------------------------
    // LOAD ALL TILE LAYERS
    // ----------------------------
    if (!data.contains("layers") || data["layers"].empty())
    {
        TraceLog(LOG_ERROR, "No layers found");
        return false;
    }

    layers.clear();
    layerNames.clear();

    for (const auto& layer : data["layers"])
    {
        if (!layer.contains("type"))
            continue;

        if (layer["type"] != "tilelayer")
            continue;

        if (!layer.contains("data"))
            continue;

        std::vector<int> layerTiles =
            layer["data"].get<std::vector<int>>();

        layers.push_back(std::move(layerTiles));
        layerNames.push_back(layer.value("name", "Unnamed"));
    }

    if (layers.empty())
    {
        TraceLog(LOG_ERROR, "No tile layers loaded");
        return false;
    }

    // ----------------------------
    // FIND COLLISION LAYER BY NAME (FIX)
    // ----------------------------
    collisionLayerIndex = -1;

    for (int i = 0; i < (int)layerNames.size(); i++)
    {
        if (layerNames[i] == "Collision")
        {
            collisionLayerIndex = i;
            break;
        }
    }

    // fallback safety
    if (collisionLayerIndex == -1)
    {
        collisionLayerIndex = (int)layers.size() - 1;
    }

#if TILEMAP_DEBUG
    std::cout << "[DEBUG] Collision layer index: "
              << collisionLayerIndex << "\n";
#endif

    return true;
}

void TileMap::Draw() const
{
    for (const auto& layer : layers)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                int index = y * mapWidth + x;

                if (index < 0 || index >= (int)layer.size())
                    continue;

                int tileID = layer[index];

                if (tileID == 0)
                    continue;

                tileID -= 1;

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

                DrawTextureRec(tileset, src, pos, WHITE);
            }
        }
    }
}

bool TileMap::IsSolid(int x, int y) const
{
    if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight)
        return true;

    if (layers.empty())
        return false;

    const auto& collisionLayer = layers[collisionLayerIndex];

    int index = y * mapWidth + x;

    if (index < 0 || index >= (int)collisionLayer.size())
        return true;

    return collisionLayer[index] != 0;
}

bool TileMap::IsSolidAtWorldPos(Vector2 worldPos) const
{
    if (tileSize <= 0)
        return false;

    int tileX = static_cast<int>(worldPos.x) / tileSize;
    int tileY = static_cast<int>(worldPos.y) / tileSize;

    if (worldPos.x < 0.0f && worldPos.x != static_cast<float>(tileX * tileSize))
        tileX -= 1;

    if (worldPos.y < 0.0f && worldPos.y != static_cast<float>(tileY * tileSize))
        tileY -= 1;

    return IsSolid(tileX, tileY);
}