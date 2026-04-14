#include "events.h"

#include "world.h"

void NextLevel::perform(World& world, GameObject& obj) {
    if (&obj != world.player) return;
    world.end_level = true;
}
