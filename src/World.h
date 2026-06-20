#pragma once

#include <vector>
#include <raylib.h>

#include "entities/Player.h"
#include "entities/Enemy.h"
#include "entities/Bullet.h"
#include "Spawner.h"

// world is bigger than window view
struct WorldBounds
{
    float width;
    float height;
};

class World
{
public:
    World();

    // update world entities -movement etc
    void Update(float dt);
    // draw world entities etc
    void Draw();
    // reset the world
    void Reset();

    // check if player dead
    bool IsPlayerDead() const;

    // get the current camera - its attacked to the player
    Camera2D& GetCamera();
    // get the player entity
    const Player& GetPlayer() const;
    // get players curr health
    int GetPlayerHealth() const;
    // get players max health potential
    int GetPlayerMaxHealth() const;

    void HandleEnemySeparation();

    Spawner& GetSpawner();

private:
    // init the world bounds
    WorldBounds bounds;

    // init the camera
    Camera2D camera;

    // init the player
    Player player;

    // init an array of enemies
    std::vector<Enemy> enemies;
    // init an array of bullets
    std::vector<Bullet> bullets;

    // init the spawner
    Spawner spawner;

    // handle firing
    float fireRate = 10.0f;
    float fireTimer = 0.0f;

    // if two entities collide
    void HandleCollisions();
    // if dead remove logic
    void RemoveDeadEnemies();
    // if bullet of screen remove
    void RemoveOffScreenBullets();
};
