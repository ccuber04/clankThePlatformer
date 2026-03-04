#pragma once

#include <utility>

#include "physics.h"
#include "graphics.h"
#include "vec.h"
#include "fsm.h"
#include "input.h"

class Color;
class World;

class GameObject {
public:
    GameObject(const Vec<float>& position, const Vec<float>& size, World& world, FSM* fsm, Input* input, const Color& color={255, 255, 255, 255});
    ~GameObject();

    void update(World& world, double dt);

    std::pair<Vec<float>, Color> get_sprite() const;

    // Player data
    Physics physics;
    Vec<float> size;
    FSM* fsm;
    Input* input;
    Color color;
    Sprite sprite;
};