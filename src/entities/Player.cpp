#include "Player.h"
#include "UI/UI.h"
#include "AssetManager.h"
#include "renderer/Renderer.h"
#include "weapons/BulletWeapon.h"

#include <memory>
#include <raylib.h>
#include <utility>

Player::Player()
    : position{400.0f, 225.0f},
      velocity{},
      speed{250.0f},
      health{100},
      maxHealth{100},
      // depending on the sheet for the entity
      animation{10, 1, 0.12f}
{
    // push weapons into the array
    // 1st BulletWeapon
    // 2nd OrbitalWeapon
    weapons.emplace_back(std::make_unique<BulletWeapon>());

    weapons.back()->SetDamage(
        weapons.back()->GetBaseDamage() + damageBonus
    );
}

void Player::Update(float deltaTime, Vector2 aimDir)
{
    // update player movement
    // same as {0.0f, 0.0f}
    velocity = {};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) velocity.y -= 1;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) velocity.y += 1;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) velocity.x -= 1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) velocity.x += 1;

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
    animation.SetState(animationState, facingDirection);
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
    // get player texture
    const Texture2D& PlayerTexture = AssetManager::GetTexture("player_testing");

    // use renderer to draw texture, use animations, get position
    Renderer::DrawAnimatedTexture(
        PlayerTexture,
        animation,
        position,
        scale
    );

    // draw weapon
    for (auto& w : weapons)
    {
        w->Draw();
    }

    // add healthbar to player - beside them
    UI::DrawHealthBar(position, health, maxHealth);
}

const std::vector<std::unique_ptr<Weapon>>& Player::GetWeapons() const
{
    // get all waepons player can use
    return weapons;
}

void Player::AddWeapon(std::unique_ptr<Weapon> weapon)
{
    weapon->SetDamage(
        weapon->GetBaseDamage() + damageBonus
    );

    weapons.push_back(std::move(weapon));
}

// get player atrtibutes
Vector2 Player::GetPos() const { return position; }

void Player::SetPos(Vector2 newPos) { position = newPos; }

float Player::GetRadius() const { return Radius; }

int Player::GetHealth() const { return health; }

int Player::GetMaxHealth() const { return maxHealth; }

// make player take damage
void Player::TakeDamage(int amount)
{
    health -= amount;
    if (health < 0) health = 0;
}

int Player::GetDamage() const
{
    return baseDamage + damageBonus;
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
    xp += amount;

    while (xp >= xpToNextLevel)
    {
        xp -= xpToNextLevel;
        level++;

        xpToNextLevel = static_cast<int>(xpToNextLevel * 1.25f);

        if (onLevelUp)
            onLevelUp(level);
    }
}

int Player::GetLevel() const { return level; }
int Player::GetXP() const { return xp; }
int Player::GetXPToNextLevel() const { return xpToNextLevel; }

// level up system
void Player::SetLevelUpCallback(std::function<void(int)> callback)
{
    onLevelUp = std::move(callback);
}

void Player::IncreaseMaxHealth(int amount)
{
    maxHealth += amount;
    health += amount;
}

void Player::IncreaseDamage(int amount)
{
    damageBonus += amount;

    for (auto& weapon : weapons)
    {
        weapon->SetDamage(
            weapon->GetBaseDamage() + damageBonus
        );
    }
}
