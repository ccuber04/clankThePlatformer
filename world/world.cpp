#include "world.h"

#include <algorithm>

#include "game_object.h"
#include "keyboard_input.h"
#include "level.h"
#include "states.h"
#include "vec.h"
#include "physics.h"
#include "audio.h"

World::World(const Level& level, Audio& audio, GameObject* player, Events events)
    : tilemap{level.width, level.height}, audio{&audio}, player{player}, events{events} {
    load_level(level);
}

World::~World() {
    for (auto obj : game_objects) {
        if (obj == player) continue;
        delete obj;
    }
}

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

void World::move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity) {
    float epsilon = 0.0f;
    Vec<float> br{position.x + size.x - epsilon, position.y};
    Vec<float> tl{position.x, position.y + size.y - epsilon};
    Vec<float> tr{position.x + size.x - epsilon, position.y + size.y - epsilon};

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
    for (auto& obj : game_objects) {
        obj->update(*this, dt);
        auto position = obj->physics.position;
        auto velocity = obj->physics.velocity;
        auto acceleration = obj->physics.acceleration;

        velocity += 0.5f * acceleration * dt;
        position += velocity * dt;
        velocity += 0.5f * acceleration * dt;
        velocity.x *= obj->physics.damping;

        velocity.x = std::clamp(velocity.x, -obj->physics.terminal_velocity, obj->physics.terminal_velocity);
        velocity.y = std::clamp(velocity.y, -obj->physics.terminal_velocity, obj->physics.terminal_velocity);

        // check for collisions in the world - x direction
        Vec<float> future_position{position.x, obj->physics.position.y};
        Vec<float> future_velocity{velocity.x, 0};
        move_to(future_position, obj->size, future_velocity);

        // now y direction after (maybe) moving in x
        future_velocity.y = velocity.y;
        future_position.y = position.y;
        move_to(future_position, obj->size, future_velocity);

        // update the obj position and velocity
        obj->physics.position = future_position;
        obj->physics.velocity = future_velocity;

        touch_tiles(*obj);
    }
}


void World::load_level(const Level& level) {
    for (const auto& [pos, tile_id] : level.tile_locations) {
        tilemap(pos.x, pos.y) = level.tile_types.at(tile_id);
    }
    audio->load_sounds({}); // load specified sound

    // get all enemies
    for (const auto& [pos, enemy_name] : level.enemy_locations) {
        auto enemy = new GameObject{enemy_name, nullptr, nullptr};
        enemy->physics.position = pos;
        game_objects.push_back(enemy);
    }

    game_objects.push_back(player);
}

void World::touch_tiles(GameObject& obj) {
    float epsilon = 0.001f;

    const std::vector<Vec<float>> tiles {
                {obj.physics.position.x - epsilon, obj.physics.position.y},
                {obj.physics.position.x, obj.physics.position.y + obj.size.y + epsilon},
                {obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y},
                {obj.physics.position.x, obj.physics.position.y - epsilon}
    };

    for (const auto& p : tiles) {
        int x = static_cast<int>(std::floor(p.x));
        int y = static_cast<int>(std::floor(p.y));
        Tile& tile = tilemap(x, y);
        if (!tile.event_name.empty()) {
            auto itr = events.find(tile.event_name);
            if (itr == events.end()) {
                throw std::runtime_error("Cannot find event: " + tile.event_name);
            }
            itr->second->perform(*this, obj);
        }
    }
}
