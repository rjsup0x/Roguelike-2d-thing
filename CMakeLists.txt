cmake_minimum_required(VERSION 3.20)

project(roli)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

add_executable(roli
    src/main.cpp
    src/Game.cpp
    src/Spawner.cpp
    src/World.cpp
    src/AssetManager.cpp

    src/entities/Player.cpp
    src/entities/Enemy.cpp
    src/entities/Bullet.cpp

    src/weapons/BulletWeapon.cpp
    src/weapons/OrbitalWeapon.cpp

    src/xp/XPOrb.cpp

    src/UI/UI.cpp
)

target_include_directories(roli PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(roli PRIVATE raylib)
