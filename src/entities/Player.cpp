#include "Player.h"
#include "UI/UI.h"
#include "AssetManager.h"
#include "renderer/Renderer.h"
#include "weapons/BulletWeapon.h"
#include "weapons/OrbitalWeapon.h"

#include <memory>
#include <raylib.h>

Player::Player()
    : position(400.0f, 225.0f),
      velocity({0, 0}),
      speed(250.0f),
      health(100),
      maxHealth(100),
      // depending on the sheet for the entity
      animation(10, 1, 0.12f)
{
    // push weapons into the array
    // 1st BulletWeapon
    // 2nd OrbitalWeapon
    weapons.push_back(std::make_unique<BulletWeapon>());
    weapons.push_back(std::make_unique<OrbitalWeapon>());
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

    // update animation
    if (Vector2Length(velocity) > 0.0f)
    {
        animationState = AnimationState::Walk;

        if (fabs(velocity.x) > fabs(velocity.y))
        {
            // is facing right of left
            if (velocity.x > 0)
                facingDirection = Direction::Right;
            else
                facingDirection = Direction::Left;
        }
        else
        {
            // is facing up or down
            if (velocity.y > 0)
                facingDirection = Direction::Down;
            else
                facingDirection = Direction::Up;
        }
    }
    else
    {
        // otherwise idle
        animationState = AnimationState::Idle;
    }

    // update anim state
    int row = 0;

    switch (animationState)
    {
        case AnimationState::Idle:

            switch (facingDirection)
            {
                case Direction::Down:  row = 0; break;
                case Direction::Left:  row = 1; break;
                case Direction::Right: row = 2; break;
                case Direction::Up:    row = 3; break;
            }

            break;

        case AnimationState::Walk:

            switch (facingDirection)
            {
                case Direction::Down:  row = 4; break;
                case Direction::Left:  row = 5; break;
                case Direction::Right: row = 6; break;
                case Direction::Up:    row = 7; break;
            }

            break;

        case AnimationState::Attack:

            switch (facingDirection)
            {
                case Direction::Down:  row = 8; break;
                case Direction::Left:  row = 9; break;
                case Direction::Right: row = 10; break;
                case Direction::Up:    row = 11; break;
            }

            break;

        case AnimationState::Hurt:

            switch (facingDirection)
            {
                case Direction::Down:  row = 12; break;
                case Direction::Left:  row = 13; break;
                case Direction::Right: row = 14; break;
                case Direction::Up:    row = 15; break;
            }

            break;

        case AnimationState::Death:

            row = 16;
            break;
    }

    animation.SetRow(row);
    animation.Update(deltaTime);

    // for all weapons
    for (auto &w : weapons)
    {
        // update each weapons deltatime, position, and aim direction
        w->Update(deltaTime, position, aimDir);
    }
}

void Player::Draw() const
{
    // use renderer to draw texture, use animations, get position
    Renderer::DrawAnimatedTexture(
        AssetManager::PlayerTex,
        animation,
        position,
        1.0f
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
