#pragma once

#include "game_object.h"
#include "tilemap.h"
#include "vec.h"
#include "events.h"
#include "quadtree.h"

class GameObject;
class Level;
class Audio;

class World {
public:
    World(const Level& level, Audio& audio, GameObject* player, Events events);
    ~World();

    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>& position) const;
    GameObject* create_player(const Level& level);
    void move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity);
    void update(float dt);
    void load_level(const Level& level);
    void touch_tiles(GameObject& obj);
    void build_quadtree();

    Tilemap tilemap;
    bool end_level{false};
    bool end_game{false};

    GameObject* player;
    Audio* audio;
    std::vector<GameObject*> game_objects;
    Events events;

    QuadTree quadtree;
};