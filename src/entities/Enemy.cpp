#include "Enemy.h"
#include "AssetManager.h"
#include "renderer/Renderer.h"
#include "UI/UI.h"

#include <raymath.h>


Enemy::Enemy(Vector2 startPos)
    : position(startPos),
      animation(kFrameCount, kRowCount, kFrameTime)
{
}

void Enemy::Update(float dt, Vector2 playerPos)
{
    // --- shared systems (run for every enemy type, every frame) ---

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

    std::erase_if(damageNumbers,
                  [](const DamageNumber& d) { return d.timer <= 0.0f; });

    // movement integration (knockback etc.)
    position = Vector2Add(position, Vector2Scale(velocity, dt));
    velocity = Vector2Scale(velocity, 0.90f);

    // --- subclass-specific AI ---
    // Previously each subclass (e.g. BatEnemy) had to call Enemy::Update()
    // itself and then separately check `if (freezeTimer > 0.0f) return;`
    // before running its own AI. That meant every new enemy type had to
    // remember to re-implement that guard correctly. Now it's handled once,
    // here, so a frozen enemy always skips AI consistently.
    if (freezeTimer > 0.0f)
        return;

    UpdateAI(dt, playerPos);
}

void Enemy::Draw() const
{
    const Texture2D& EnemyTexture = AssetManager::GetTexture("enemy");

    const Color tint = (hitFlashTimer >= 0.0f) ? Fade(WHITE, 1.0f) : Fade(RED, 1.0f);;

    Renderer::DrawAnimatedTexture(
        EnemyTexture,
        animation,
        position,
        1.5f,
        0.0f,
        tint,
        facingDirection == Direction::Left
    );

    UI::DrawHealthBar(position, health, maxHealth);

    for (const auto& d : damageNumbers)
    {
        DrawText(
            TextFormat("%d", d.value),
            static_cast<int>(d.pos.x),
            static_cast<int>(d.pos.y),
            18,
            Fade(RED, d.alpha)
        );
    }
}

Vector2 Enemy::GetPos() const { return position; }
void Enemy::SetPos(Vector2 newPos) { position = newPos; }

float Enemy::GetRadius() { return Radius; }

bool Enemy::isDead() const { return health <= 0; }

int Enemy::GetHealth() const { return health; }

void Enemy::SetStats(int hp, float spd)
{
    health = hp;
    maxHealth = hp;
    speed = spd;
}

void Enemy::TakeDamage(const int amount, const Vector2 hitDir)
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
