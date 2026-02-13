#pragma once

#include "vec.h"

class Physics {
public:
    Physics()=default;
    Physics(Vec<float> position, Vec<float> velocity, Vec<float> acceleration);
    Physics(Vec<float> position, Vec<float> velocity, Vec<float> acceleration, float gravity, float damping,
        float walk_acceleration, float jump_velocity, float terminal_velocity);

    Vec<float> position{0, 0};
    Vec<float> velocity{0, 0};
    Vec<float> acceleration{0, 0};

    float gravity{-30.0f};
    float damping{0.88f};
    float walk_acceleration{50.0f};
    float jump_velocity{15.0f};
    float terminal_velocity{30.0f};
};