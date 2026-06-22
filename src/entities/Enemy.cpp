#include "Enemy.h"

#include "AssetManager.h"
#include "renderer/Renderer.h"
#include "UI/UI.h"


Enemy::Enemy(Vector2 startPos)
    : position(startPos),
      velocity{},
      speed(100.0f),
      health(50),
      maxHealth(50),
      animation(8, 1, 0.12f)
{
}

void Enemy::Update(float dt, Vector2 playerPos)
{
    // base update just runs shared systems
    if (hitFlashTimer > 0.0f)
        hitFlashTimer -= dt;

    if (freezeTimer > 0.0f)
        freezeTimer -= dt;

    // damage numbers
    for (auto& d : damageNumbers)
    {
        d.timer -= dt;
        d.pos.y -= 30.0f * dt;
        d.alpha = d.timer / 0.6f;
    }

    damageNumbers.erase(
        std::remove_if(damageNumbers.begin(), damageNumbers.end(),
            [](DamageNumber& d) { return d.timer <= 0.0f; }),
        damageNumbers.end()
    );

    // movement integration (knockback etc)
    position = Vector2Add(position, Vector2Scale(velocity, dt));
    velocity = Vector2Scale(velocity, 0.90f);
}

void Enemy::Draw() const
{
    const Texture2D& tex = AssetManager::GetTexture("enemy");

    Color tint = (hitFlashTimer > 0.0f) ? RED : WHITE;

    Renderer::DrawAnimatedTexture(
        tex,
        animation,
        position,
        1.0f,
        0.0f,
        tint,
        facingDirection == Direction::Left
    );

    UI::DrawHealthBar(position, health, maxHealth);

    for (const auto& d : damageNumbers)
    {
        DrawText(
            TextFormat("%d", d.value),
            (int)d.pos.x,
            (int)d.pos.y,
            18,
            Fade(RED, d.alpha)
        );
    }
}

Vector2 Enemy::GetPos() const { return position; }
void Enemy::SetPos(Vector2 newPos) { position = newPos; }

float Enemy::GetRadius() const { return Radius; }

bool Enemy::isDead() const { return health <= 0; }

int Enemy::GetHealth() const { return health; }

void Enemy::SetStats(int hp, float spd)
{
    health = hp;
    maxHealth = hp;
    speed = spd;
}

void Enemy::TakeDamage(int amount, Vector2 hitDir)
{
    health -= amount;
    if (health < 0) health = 0;

    velocity = Vector2Add(
        velocity,
        Vector2Scale(hitDir, 250.0f)
    );

    freezeTimer = 0.08f;
    hitFlashTimer = 0.12f;

    damageNumbers.push_back({ amount, position, 0.6f, 1.0f });
}
