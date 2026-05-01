#pragma once

#include <utility>

#include <map>
#include "vec.h"
#include "fsm.h"
#include "graphics.h"
#include "physics.h"
#include "animated_sprite.h"

class World;
class Input;
class AABB;

using Sprites = std::map<std::string, AnimatedSprite>;

class GameObject {
public:
    GameObject(std::string name, FSM* fsm, Input* input, const Color& color={255, 255, 255, 255});
    virtual ~GameObject();

    virtual void update(World& world, double dt);

    std::pair<Vec<float>, Color> get_sprite() const;
    void set_sprite(const std::string& next_sprite);

    AABB get_bounding_box();

    void take_damage(int attack_damage);
    bool flash_sprite() const;

    // Player data
    std::string obj_name;
    Physics physics;
    Vec<float> size;
    FSM* fsm;
    Input* input;
    Color color;
    Sprites sprites;
    Sprite sprite;
    std::string sprite_name;

    // combat stuff
    int health;
    int max_health;
    int damage;
    bool is_alive{true};

    double iframe{0.0};
};