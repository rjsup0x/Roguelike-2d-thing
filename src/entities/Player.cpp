#include "Player.h"
#include "UI/UI.h"
#include "AssetManager.h"
#include "weapons/BulletWeapon.h"

#include <raylib.h>

#include <iostream>

Player::Player()
    : position(400.0f, 225.0f),
      velocity({0, 0}),
      speed(250.0f),
      health(100),
      maxHealth(100)
{
    // push weapons into the array
    // 1st BulletWeapon
    weapons.push_back(std::make_unique<BulletWeapon>());
}

void Player::Update(float deltaTime, Vector2 aimDir)
{
    // update player movement
    velocity = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W)) velocity.y -= 1;
    if (IsKeyDown(KEY_S)) velocity.y += 1;
    if (IsKeyDown(KEY_A)) velocity.x -= 1;
    if (IsKeyDown(KEY_D)) velocity.x += 1;

    // normaize the movement - when moving on angle stop it going faster
    if (Vector2Length(velocity) > 0.0f)
    {
        velocity = Vector2Normalize(velocity);
        position = Vector2Add(position, Vector2Scale(velocity, speed * deltaTime));
    }

    // for all weapons
    for (auto &w : weapons)
    {
        // update each weapons deltatime, position, and aim direction
        w->Update(deltaTime, position, aimDir);
    }
}

void Player::Draw() const
{
    // add the texture to the player as size
    Vector2 size = {
        (float)AssetManager::PlayerTex.width,
        (float)AssetManager::PlayerTex.height
    };

    // draw that texture and its attributes
    DrawTextureV(
        AssetManager::PlayerTex,
        {
            position.x - size.x / 2.0f,
            position.y - size.y / 2.0f
        },
        WHITE
    );

    // draw weapon
    for (auto& w : weapons)
    {
        w->Draw();
    }

    // add healthbar to player - beside them
    UI::DrawHealthBar(position, health, maxHealth);
}

std::vector<std::unique_ptr<Weapon>>& Player::GetWeapons()
{
    // get all waepons player can use
    return weapons;
}

// get player atrtibutes
Vector2 Player::GetPos() const { return position; }

void Player::SetPos(Vector2 newPos) { position = newPos; }

float Player::GetRadius() const { return 16.0f; }

int Player::GetHealth() const { return health; }

int Player::GetMaxHealth() const { return maxHealth; }

// make player take damage
void Player::TakeDamage(int amount)
{
    health -= amount;
    if (health < 0) health = 0;
}

bool Player::isDead() const
{
    return health <= 0;
}

Vector2 Player::GetVelocity() const
{
    return velocity;
}

// xp functions
void Player::AddXP(int amount)
{
    // how much xp to add
    xp += amount;

    // if xp is enough to level up
    if (xp >= xpToNextLevel)
    {
        // refresh xp
        xp -= xpToNextLevel;
        // go up a level + 1
        level++;
        // make xp to next level more than it previously was
        xpToNextLevel = (int)(xpToNextLevel * 1.25f); // scaling
    }
}

int Player::GetLevel() const { return level; }
int Player::GetXP() const { return xp; }
int Player::GetXPToNextLevel() const { return xpToNextLevel; }
