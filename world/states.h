#pragma once

#include "fsm.h"

class Standing : public State {
public:
    void on_enter(World& world, GameObject& obj) override;
    Action* input(World& world, GameObject& obj, ActionType action_type) override;
};

class InAir : public State {
public:
    void on_enter(World& world, GameObject& obj) override;
    void update(World& world, GameObject& obj, double dt) override;

    const double cooldown = 1;
    double elapsed = 0;
};