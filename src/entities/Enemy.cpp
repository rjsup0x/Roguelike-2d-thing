// #include <iostream>
#include <raylib.h>

#include "Enemy.h"
#include "UI/UI.h"
#include "AssetManager.h"
#include "animation/AnimationState.h"
#include "renderer/Renderer.h"

Enemy::Enemy(Vector2 startPos)
    : position{startPos},
      velocity{},
      speed{100.0f},
      maxHealth{50},
      health{50},
      animation{8, 1, 0.12f}
{

}

void Enemy::Update(float deltaTime, Vector2 playerPos)
{
    // -------------------------
    // timers
    // -------------------------
    if (hitFlashTimer > 0.0f)
        hitFlashTimer -= deltaTime;

    if (freezeTimer > 0.0f)
        freezeTimer -= deltaTime;

    // -------------------------
    // update damage numbers
    // -------------------------
    for (auto &d : damageNumbers)
    {
        d.timer -= deltaTime;
        d.pos.y -= 30.0f * deltaTime;
        d.alpha = d.timer / 0.6f;
    }

    damageNumbers.erase(
        std::remove_if(damageNumbers.begin(), damageNumbers.end(),
        [](DamageNumber &d) { return d.timer <= 0.0f; }),
        damageNumbers.end()
    );

    // -------------------------
    // ANIMATION + FACING
    // -------------------------
    Vector2 toPlayerForFacing = Vector2Subtract(playerPos, position);

    if (Vector2Length(toPlayerForFacing) > 0.0f)
    {
        animationState = AnimationState::Walk;

        toPlayerForFacing = Vector2Normalize(toPlayerForFacing);

        if (fabs(toPlayerForFacing.x) > fabs(toPlayerForFacing.y))
        {
            if (toPlayerForFacing.x > 0.0f)
                facingDirection = Direction::Right;
            else
                facingDirection = Direction::Left;
        }
        else
        {
            if (toPlayerForFacing.y > 0.0f)
                facingDirection = Direction::Down;
            else
                facingDirection = Direction::Up;
        }
    }
    else
    {
        animationState = AnimationState::Idle;
    }

    // update anim state
    animation.SetState(animationState, facingDirection);
    animation.Update(deltaTime);

    // -------------------------
    // KNOCKBACK MOVEMENT
    // -------------------------
    position = Vector2Add(position, Vector2Scale(velocity, deltaTime));

    // apply friction (important or they slide forever)
    velocity = Vector2Scale(velocity, 0.90f);

    // -------------------------
    // freeze = no AI movement
    // -------------------------
    if (freezeTimer > 0.0f)
        return;

    // -------------------------
    // AI MOVE TOWARD PLAYER
    // -------------------------
    Vector2 direction = Vector2Subtract(playerPos, position);

    if (Vector2Length(direction) > 0.0f)
    {
        direction = Vector2Normalize(direction);

        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;
    }

    // -------------------------
    // UPDATE FACING
    // -------------------------
    // Vector2 toPlayer = Vector2Subtract(playerPos, position);

    // if (Vector2Length(toPlayer) > 0.0f)
    // {
    //     toPlayer = Vector2Normalize(toPlayer);

    //     // only flip direction (do NOT use for movement)
    //     facingLeft = (toPlayer.x < 0.0f);
    // }

    // -------------------------
    // HIT FLASH TIMER
    // -------------------------
    if (hitFlashTimer > 0.0f)
        hitFlashTimer -= deltaTime;
}

void Enemy::Draw() const
{
    // for getting hit
    Color tint = WHITE;
    float roatation{};

    // enemy texture
    const Texture2D& EnemyTexture = AssetManager::GetTexture("enemy");


    Renderer::DrawAnimatedTexture(
        EnemyTexture,
        animation,
        position,
        scale,
        rotation,
        tint,
        facingDirection == Direction::Left
    );

    // if player hit flash red
    if (hitFlashTimer > 0.0f)
        tint = RED;

    // add healthbar beside enemies
    UI::DrawHealthBar(position, health, maxHealth);

    // for all damage number in the array
    for (const auto &d : damageNumbers)
    {
        // draw them above the enemy when hot
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

void Enemy::TakeDamage(int amount, Vector2 hitDirection)
{
    health -= amount;
        if (health < 0) health = 0;

    // knockback
    float knockback{250.0f};

    velocity = Vector2Add(
        velocity,
        Vector2Scale(hitDirection, knockback)
    );

    // freeze enemy briefly
    freezeTimer = 0.08f;

    // flash red
    hitFlashTimer = 0.12f;



    // spawn damage number
    damageNumbers.push_back(
        DamageNumber{
            amount,
            position,
            0.6f,
            1.0f
        }
    );
}

bool Enemy::isDead() const
{
    return health <= 0;
}

int Enemy::GetHealth() const { return health; }

void Enemy::SetStats(int hp, float spd)
{
    health = hp;
    maxHealth = hp;
    speed = spd;
}
