#pragma once

#include <utility>

#include "physics.h"
#include "graphics.h"
#include "vec.h"

class Color;

class World;

class GameObject {
public:
    GameObject(const Vec<float>& position, const Vec<int>& size, World& world);
    ~GameObject();

    void input(World& world);
    void update(World& world, double dt);

    std::pair<Vec<float>, Color> get_sprite() const;

    // Player data
    Physics physics;
    Vec<int> size;

};