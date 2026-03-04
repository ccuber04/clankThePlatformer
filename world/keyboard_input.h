#pragma once

#include "input.h"
#include "SDL3/SDL_events.h"

class KeyboardInput : public Input {
public:
    void collect_discrete_event(SDL_Event* event) override;
    void get_input() override;
    void handle_input(World& world, GameObject& obj) override;
};
