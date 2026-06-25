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

    std::cout << "[DEBUG] Map size: "
              << mapWidth << "x"
              << mapHeight << "\n";

    std::cout << "[DEBUG] Tile size: "
              << tileSize << "\n";

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

    std::cout << "[DEBUG] Raw tileset image path: "
              << imagePath << "\n";

    if (imagePath.empty())
    {
        TraceLog(LOG_ERROR, "Tileset image path is empty");
        return false;
    }

    if (!FileExists(imagePath.c_str()))
    {
        TraceLog(LOG_ERROR,
                 "Texture file NOT FOUND: %s",
                 imagePath.c_str());
    }

    tileset = LoadTexture(imagePath.c_str());

    std::cout << "[DEBUG] Texture ID: "
              << tileset.id << "\n";

    std::cout << "[DEBUG] Texture size: "
              << tileset.width << "x"
              << tileset.height << "\n";

    if (tileset.id == 0)
    {
        TraceLog(LOG_ERROR,
                 "Failed to load tileset texture");
        return false;
    }

    tilesPerRow = tileset.width / tileSize;

    std::cout << "[DEBUG] Tiles per row: "
              << tilesPerRow << "\n";

    // ----------------------------
    // LOAD ALL TILE LAYERS
    // ----------------------------
    if (!data.contains("layers") || data["layers"].empty())
    {
        TraceLog(LOG_ERROR, "No layers found");
        return false;
    }

    layers.clear();

    int tileLayerCount = 0;

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

        std::cout
            << "[DEBUG] Loaded layer: "
            << layer.value("name", "Unnamed")
            << " | tiles="
            << layerTiles.size()
            << "\n";

        layers.push_back(std::move(layerTiles));

        tileLayerCount++;
    }

    std::cout
        << "[DEBUG] Total tile layers loaded: "
        << tileLayerCount
        << "\n";

    if (layers.empty())
    {
        TraceLog(LOG_ERROR,
                 "No tile layers loaded");
        return false;
    }

    std::cout
        << "[DEBUG] Expected tiles per layer: "
        << (mapWidth * mapHeight)
        << "\n";

    for (size_t i = 0; i < layers.size(); i++)
    {
        std::cout
            << "[DEBUG] Layer "
            << i
            << " tile count: "
            << layers[i].size()
            << "\n";
    }

    return true;
}

void TileMap::Draw() const
{
    static bool printedOnce = false;

    if (!printedOnce)
    {
        std::cout << "\n===== DRAW DEBUG =====\n";
        std::cout << "Map Width: " << mapWidth << "\n";
        std::cout << "Map Height: " << mapHeight << "\n";
        std::cout << "Tile Size: " << tileSize << "\n";
        std::cout << "Layer Count: " << layers.size() << "\n";
        std::cout << "Texture ID: " << tileset.id << "\n";
        std::cout << "Texture Size: "
                  << tileset.width << "x"
                  << tileset.height << "\n";
        std::cout << "Tiles Per Row: "
                  << tilesPerRow << "\n";
        std::cout << "======================\n";

        printedOnce = true;
    }

    static int debugCount = 0;

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

                if (debugCount < 10)
                {
                    std::cout
                        << "Tile #" << debugCount
                        << " | tileID=" << tileID
                        << " | src=(" << srcX << "," << srcY << ")"
                        << " | pos=(" << pos.x << "," << pos.y << ")"
                        << "\n";

                    debugCount++;
                }

                DrawTextureRec(
                    tileset,
                    src,
                    pos,
                    WHITE
                );
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

    int index = y * mapWidth + x;

    if (index < 0 || index >= (int)layers[0].size())
        return true;

    // use top layer (walls) for now
    if (layers.size() > 1)
        return layers[2][index] != 0;

    return layers[0][index] != 0;
}