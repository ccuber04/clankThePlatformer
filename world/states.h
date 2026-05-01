#pragma once

#include "fsm.h"

class Standing : public State {
public:
    void on_enter(World& world, GameObject& obj) override;
    void update(World& world, GameObject& obj , double dt) override;
    Action* input(World& world, GameObject& obj, ActionType action_type) override;
};

class InAir : public State {
public:
    void on_enter(World& world, GameObject& obj) override;
    void update(World& world, GameObject& obj, double dt) override;
    Action* input(World& world, GameObject& obj, ActionType action_type) override;

    const double cooldown = 0.1;
    double elapsed = 0;
};

class Running : public State {
public:
    void on_enter(World& world, GameObject& obj) override;
    void update(World& world, GameObject& obj, double dt) override;
    Action* input(World& world, GameObject& obj, ActionType action_type) override;
};

class Patrolling : public Running {
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override;
    Action* input(World&, GameObject&, ActionType) override;

    double elapsed{0.0};
    double cooldown{0.0};
};

class Crouching : public State {
public:
    void on_enter(World& world, GameObject& obj) override;
    void on_exit(World&, GameObject&) override;
    Action* input(World& world, GameObject& obj, ActionType action_type) override;
};

class DoubleJump : public State {
public:
    void on_enter(World& world, GameObject& obj) override;
    void update(World& world, GameObject& obj, double dt) override;
    Action* input(World& world, GameObject& obj, ActionType action_type) override;
};

class AttackAllEnemies : public State {
public:
    void on_enter(World& world, GameObject& obj) override;
    void update(World& world, GameObject& obj, double dt) override;

    double elapsed = 0;
    double cooldown = 2;
};