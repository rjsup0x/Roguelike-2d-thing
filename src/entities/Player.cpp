#include "Player.h"
#include "UI/UI.h"
#include "AssetManager.h"

#include <raylib.h>


Player::Player()
    : position(400.0f, 225.0f),
      velocity({0, 0}),
      speed(250.0f),
      health(100),
      maxHealth(100)
{

}

void Player::Update(float deltaTime)
{
    // update player movement
    velocity = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W)) velocity.y -= 1;
    if (IsKeyDown(KEY_S)) velocity.y += 1;
    if (IsKeyDown(KEY_A)) velocity.x -= 1;
    if (IsKeyDown(KEY_D)) velocity.x += 1;

    // normalize it for verticle movement
    if (Vector2Length(velocity) > 0.0f)
    {
        velocity = Vector2Normalize(velocity);

        position.x += velocity.x * speed * deltaTime;
        position.y += velocity.y * speed * deltaTime;
    }
}

void Player::Draw() const
{
    Vector2 size = {
        (float)AssetManager::PlayerTex.width,
        (float)AssetManager::PlayerTex.height
    };

    DrawTextureV(
        AssetManager::PlayerTex,
        {
            position.x - size.x / 2.0f,
            position.y - size.y / 2.0f
        },
        WHITE
    );

    UI::DrawHealthBar(position, health, maxHealth);
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
