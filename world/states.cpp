#include "states.h"

#include "action.h"
#include "game_object.h"
#include "world.h"

#include <iostream>

// Helper function
bool on_platform(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;
    Vec<float> left_foot{obj.physics.position.x + epsilon, obj.physics.position.y - epsilon};
    Vec<float> right_foot{obj.physics.position.x + obj.size.x - epsilon, obj.physics.position.y - epsilon};
    return world.collides(left_foot) || world.collides(right_foot);
}

// Standing
void Standing::on_enter(World&, GameObject& obj) {
    obj.color = Color{255, 255, 255, 255};
    obj.physics.acceleration.x = 0;
    obj.set_sprite("idle");
    std::cout << "Standing\n";
}

void Standing::update(World& world, GameObject& obj, double) {
    if (!on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
    }
}

Action* Standing::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::Jump && on_platform(world, obj)) {
        std::cout << "Standing->InAir\n";
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    }
    if (action_type == ActionType::Crouch) {
        obj.fsm->transition(Transition::Crouch, world, obj);
        return new Crouch();
    }
    return nullptr;
}

// InAir
void InAir::on_enter(World&, GameObject& obj) {
    elapsed = cooldown;
    obj.color = Color{0, 0, 255, 255};
    obj.set_sprite("jumping");
    std::cout << "InAir\n";
}

void InAir::update(World& world, GameObject& obj, double dt) {
    elapsed -= dt;
    if (on_platform(world, obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

Action* InAir::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    }
    if (action_type == ActionType::Jump) {
        std::cout << "InAir->DoubleJump\n";
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    return nullptr;
}

// Running
void Running::on_enter(World&, GameObject& obj) {
    obj.color = Color{255, 255, 0};
    obj.set_sprite("walking");
    std::cout << "Running\n";
}

void Running::update(World& world, GameObject& obj, double) {
    if (!on_platform(world, obj)) {
        obj.fsm->transition(Transition::Jump, world, obj);
    }
}

Action* Running::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    if (action_type == ActionType::Jump && on_platform(world, obj)) {
        std::cout << "Running->InAir\n";
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    }
    if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    }
    if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    if (action_type == ActionType::Crouch) {
        obj.fsm->transition(Transition::Crouch, world, obj);
        return new Crouch();
    }
    return nullptr;
}

// Crouching
void Crouching::on_enter(World&, GameObject& obj) {
    obj.color = Color{255, 0, 255, 255};
    obj.set_sprite("crouch");
    std::cout << "Crouching\n";
}

void Crouching::on_exit(World&, GameObject& obj) {
    obj.set_sprite("uncrouch");
    obj.size.y *= 2;
}

Action* Crouching::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
    return nullptr;
}

// DoubleJump
void DoubleJump::on_enter(World&, GameObject& obj) {
    obj.color = Color{10, 200, 255, 255};
    obj.set_sprite("jumping");
    std::cout << "DoubleJump\n";
}

void DoubleJump::update(World& world, GameObject& obj, double dt) {
    if (on_platform(world, obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

Action* DoubleJump::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    }
    if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    }
    return nullptr;
}
