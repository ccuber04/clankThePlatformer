#include "game_object.h"

GameObject::GameObject(const Vec<float>& position, const Vec<int>& size, World& world)
    : size{size} {
    physics.position = position;
    physics.acceleration.y = physics.gravity;
}

GameObject::~GameObject() {}

void GameObject::input(World& world) {
    const bool *key_states = SDL_GetKeyboardState(NULL);

    physics.acceleration.x = 0;
    // if (key_states[SDL_SCANCODE_W]) {
    //     velocity.y += -8;
    // }
    // if (key_states[SDL_SCANCODE_S]) {
    //     velocity.y += 8;
    // }
    if (key_states[SDL_SCANCODE_A]) {
        // velocity.x += -8;
        physics.acceleration.x -= physics.walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_D]) {
        // velocity.x += 8;
        physics.acceleration.x += physics.walk_acceleration;
    }
    if (key_states[SDL_SCANCODE_SPACE]) {
        physics.velocity.y = physics.jump_velocity;
    }
    if (key_states[SDL_SCANCODE_LSHIFT])
    {
        physics.velocity.x *= 2;
    }
}

void GameObject::update(World& world, double dt) {}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, {20, 200, 255, 255}};
}