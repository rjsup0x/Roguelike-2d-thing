#pragma once

enum class AnimationState
{
    Idle,
    Walk,
    Attack,
    Hurt,
    Death
};

enum class Direction
{
    Down = 0,
    Left = 1,
    Right = 2,
    Up = 3
};
