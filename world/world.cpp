#include "world.h"

#include <algorithm>

#include "game_object.h"
#include "keyboard_input.h"
#include "states.h"
#include "vec.h"
#include "physics.h"

World::World(int width, int height)
    : tilemap{width, height} {}

void World::add_platform(float x, float y, float width, float height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tilemap(x+j, y+i) = Tile{};
        }
    }
}

bool World::collides(const Vec<float>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x, y).blocking;
}

GameObject* World::create_player() {
    // Create the fsm
    Transitions transitions = {
        {{StateType::Standing, Transition::Jump}, StateType::InAir},
        {{StateType::Standing, Transition::Move}, StateType::Running},
        {{StateType::Standing, Transition::Crouch}, StateType::Crouching},
        {{StateType::InAir, Transition::Stop}, StateType::Standing},
        {{StateType::InAir, Transition::Move}, StateType::Running},
        {{StateType::InAir, Transition::Jump}, StateType::DoubleJump},
        {{StateType::Running, Transition::Stop}, StateType::Standing},
        {{StateType::Running, Transition::Jump}, StateType::InAir},
        {{StateType::Crouching, Transition::Stop}, StateType::Standing},
        {{StateType::DoubleJump, Transition::Stop}, StateType::Standing}
    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Running, new Running()},
        {StateType::Crouching, new Crouching()},
        {StateType::DoubleJump, new DoubleJump()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};
    KeyboardInput* input = new KeyboardInput();

    player = std::make_unique<GameObject>(Vec<float>{1, 1}, *this, fsm, input);
    return player.get();
}

void World::move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity) {
    Vec<float> br{position.x + size.x, position.y};
    Vec<float> tl{position.x, position.y + size.y};
    Vec<float> tr{position.x + size.x, position.y + size.y};

    // test for collisions on the bottom or top first
    if (collides(position) && collides(br)) {
        position.y = ceil(position.y);
        velocity.y = 0;
    }
    else if (collides(tl) && collides(tr)) {
        position.y = floor(position.y);
        velocity.y = 0;
    }

    // then test for collisions on the left and right sides
    if (collides(position) && collides(tl)) {
        position.x = ceil(position.x);
        velocity.x = 0;
    }
    else if (collides(br) && collides(tr)) {
        position.x = floor(position.x);
        velocity.x = 0;
    }

    // now test each corner
    if (collides(position)) {
        float dx = ceil(position.x) - position.x;
        float dy = ceil(position.y) - position.y;
        if (dx > dy) {
            position.y = ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = ceil(position.x);
            velocity.x = 0;
        }
    }
    else if (collides(br)) {
        float dx = br.x - floor(br.x);
        float dy = ceil(br.y) - br.y;
        if (dx > dy) {
            position.y = ceil(position.y);
            velocity.y = 0;
        }
        else {
            position.x = floor(position.x);
            velocity.x = 0;
        }
    }
    else if (collides(tr)) {
        float dx = tr.x - floor(tr.x);
        float dy = tr.y - floor(tr.y);
        if (dx > dy) {
            position.y = floor(position.y);
            velocity.y = 0;
        }
        else {
            position.x = floor(position.x);
            velocity.x = 0;
        }
    }
    else if (collides(tl)) {
        float dx = ceil(tl.x) - tl.x;
        float dy = tl.y - floor(tl.y);
        if (dx > dy) {
            position.y = floor(position.y);
            velocity.y = 0;
        }
        else {
            position.x = ceil(position.x);
            velocity.x = 0;
        }
    }
}

void World::update(float dt) {
    // currently only updating player
    player->update(*this, dt);
    auto position = player->physics.position;
    auto velocity = player->physics.velocity;
    auto acceleration = player->physics.acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity * dt;
    velocity += 0.5f * acceleration * dt;
    velocity.x *= player->physics.damping;

    velocity.x = std::clamp(velocity.x, -player->physics.terminal_velocity, player->physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -player->physics.terminal_velocity, player->physics.terminal_velocity);

    // check for collisions in the world - x direction
    Vec<float> future_position{position.x, player->physics.position.y};
    Vec<float> future_velocity{velocity.x, 0};
    move_to(future_position, player->size, future_velocity);

    // now y direction after (maybe) moving in x
    future_velocity.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, player->size, future_velocity);

    // update the player position and velocity
    player->physics.position = future_position;
    player->physics.velocity = future_velocity;
}
