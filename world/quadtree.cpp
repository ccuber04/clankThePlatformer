#include "quadtree.h"

#include "game_object.h"

bool AABB::contains(Vec<float> point) const {
    Vec<float> displacement = point - center;
    return std::abs(displacement.x) < half_dimension.x && std::abs(displacement.y) < half_dimension.y;
}

bool AABB::intersects(AABB other) const {
    Vec<float> displacement = other.center - center;
    return std::abs(displacement.x) < (half_dimension.x + other.half_dimension.x)
        && std::abs(displacement.y) < (half_dimension.y + other.half_dimension.y);
}

Quadtree::Quadtree(AABB boundary)
    : boundary{boundary}{}
