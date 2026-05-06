#include "tilemap.h"

#include "random.h"

#include <stdexcept>
#include <sstream>

void Tile::update(double dt) {
    if (sprite.number_of_frames == 1) return;

    animated_sprite.update(dt);
    sprite = animated_sprite.get_sprite();
}

void Tile::randomize_animation_start() {
    if (sprite.number_of_frames == 1) return;

    auto first_location = sprite.location;
    std::vector<Sprite> sprite_frames;

    for (int i = 0; i < sprite.number_of_frames; ++i) {
        sprite.location = {first_location.x + i * sprite.size.x, first_location.y};
        sprite_frames.push_back(sprite);
    }

    int starting_frame = randint(0, sprite.number_of_frames - 1);
    animated_sprite = AnimatedSprite{sprite_frames, sprite.dt_per_frame, starting_frame};
    sprite = animated_sprite.get_sprite();
}

Tilemap::Tilemap(int width, int height)
    : width{width}, height{height}, tiles(width*height) {
    if (width < 1) {
        throw std::runtime_error("width must be positive");
    }
    if (height < 1) {
        throw std::runtime_error("height must be positive");
    }
}

void Tilemap::update(double dt) {
    for (auto& tile : tiles) {
        tile.update(dt);
    }
}

void Tilemap::check_bounds(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ") is not within bounds (";
        ss << width << ", " << height << ")";
        throw std::runtime_error(ss.str());
    }
}

const Tile& Tilemap::operator()(int x, int y) const {
    check_bounds(x, y);
    return tiles.at(x + y*width);
}

Tile& Tilemap::operator()(int x, int y) {
    check_bounds(x, y);
    return tiles.at(x + y*width);
}
