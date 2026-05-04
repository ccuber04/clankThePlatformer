#pragma once

#include <vector>

#include "animated_sprite.h"
#include "sprite.h"

struct Background {
    std::string filename;
    float scale;
    float distance;
    Sprite sprite;
};

class Tile {
public:
    void update(double dt);

    Sprite sprite;
    AnimatedSprite animated_sprite;
    bool animation_random_start{true};
    bool blocking{false};
    std::string id;
    std::string event_name;
};

class Tilemap {
public:
    Tilemap(int width, int height);

    void update(double dt);

    // access tiles
    const Tile& operator()(int x, int y) const;
    Tile& operator()(int x, int y);

    const int width;
    const int height;

private:
    std::vector<Tile> tiles;

    void check_bounds(int x, int y) const;
};