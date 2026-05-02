#include "game.h"

#include  "keyboard_input.h"
#include "states.h"
#include "asset_manager.h"
#include "input.h"
#include "ai_input.h"
#include "projectile.h"

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height}, camera{graphics, 64}, dt{1.0/60.0}, lag{0.0}, performance_frequency{SDL_GetPerformanceFrequency()}, prev_counter{SDL_GetPerformanceCounter()} {
    // load events
    get_events();

    // Give player its assets then put it in the correct state
    create_player();

    // load first level
    load_level();
}

Game::~Game() {
    delete world;
    for (auto [_, event] : events) {
        delete event;
    }
}

void Game::handle_event(SDL_Event* event) {
    switch (mode) {
        case GameMode::Playing:
            auto action = player->input->collect_discrete_event(event);
            if (action) {
                action->perform(*world, *player);
                delete action;
            }
            break;
    }
}

void Game::input() {
    switch (mode) {
        case GameMode::Playing:
            player->input->get_input();
            camera.handle_input();
    }
}
void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (float)performance_frequency;
    prev_counter = now;
    while (lag >= dt) {
        switch (mode) {
            case GameMode::Playing:
                for (auto& obj : world->game_objects) {
                    obj->input->handle_input(*world, *obj);
                }
                world->update(dt);

                // put the camera slightly ahead of the player
                float L = length(player->physics.velocity);
                Vec displacement = 8.0f * player->physics.velocity / (1.0f + L);
                camera.update(player->physics.position + displacement, dt);

                // check for level end
                if (world->end_level) {
                    load_level();
                }

                // check for game over
                if (world->end_game) {
                    mode = GameMode::GameOver;
                }
                break;
        }
        lag -= dt;
    }
}

void Game::render() {
    // clear
    graphics.clear();

    // draw the background
    camera.render(world->backgrounds);

    // draw the world
    camera.render(world->tilemap);

    // enemies
    for (auto& obj : world->game_objects) {
        camera.render(*obj);
    }

    for (auto& projectile : world->projectiles) {
        camera.render(*projectile);
    }

    if (mode == GameMode::GameOver) {
        camera.render_game_over();
    }

    // update
    graphics.update();
}

void Game::get_events() {
    events["next_level"] = new NextLevel();
}

void Game::create_player() {
    // Create the fsm
    Transitions transitions = {
        {{StateType::Standing, Transition::Jump}, StateType::InAir},
        {{StateType::Standing, Transition::Move}, StateType::Running},
        {{StateType::Standing, Transition::AttackAll}, StateType::AttackAll},
        {{StateType::Standing, Transition::Crouch}, StateType::Crouching},
        {{StateType::InAir, Transition::Stop}, StateType::Standing},
        {{StateType::InAir, Transition::Move}, StateType::Running},
        {{StateType::InAir, Transition::Jump}, StateType::DoubleJump},
        {{StateType::Running, Transition::Stop}, StateType::Standing},
        {{StateType::Running, Transition::Jump}, StateType::InAir},
        {{StateType::Crouching, Transition::Stop}, StateType::Standing},
        {{StateType::DoubleJump, Transition::Stop}, StateType::Standing},
        {{StateType::AttackAll, Transition::Stop}, StateType::Standing}
    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Running, new Running()},
        {StateType::Crouching, new Crouching()},
        {StateType::DoubleJump, new DoubleJump()},
        {StateType::AttackAll, new AttackAllEnemies()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};
    KeyboardInput* input = new KeyboardInput();

    player = std::make_unique<GameObject>("player", fsm, input);
    AssetManager::get_game_object_details("player", graphics, *player);
}

void Game::load_level() {
    std::string level_name = "level_" + std::to_string(++current_level);
    Level level{level_name};
    AssetManager::get_level_details(graphics, level);

    // create the world
    delete world;
    world = new World(level, audio, player.get(), events);

    // assets for objs
    for (auto& obj : world->game_objects) {
        if (obj == world->player) {
            continue;
        }
        AssetManager::get_game_object_details(obj->obj_name + "-enemy", graphics, *obj, true);
        update_enemies(*obj);
    }

    // get the available items
    AssetManager::get_available_items("items", graphics, *world);

    GameObject* obj = world->available_items["fireball"]();

    player->physics.position = {static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)};
    player->fsm->current_state->on_enter(*world, *player);
    camera.set_location(player->physics.position);
    audio.play_sounds("background", true);
}

void Game::update_enemies(GameObject& obj) {
    Transitions transitions;
    States states;

    if (obj.obj_name == "mushroom" || obj.obj_name == "mysticflyer") {
        transitions = {
            {{StateType::Standing, Transition::Move}, StateType::Patrolling},
            {{StateType::Patrolling, Transition::Stop}, StateType::Standing}
        };

        states = {
            {StateType::Standing, new Standing()},
            {StateType::Patrolling, new Patrolling()}
        };
    }

    FSM* fsm = new FSM{transitions, states, StateType::Patrolling};
    obj.fsm = fsm;

    Input* input = new AiInput();
    input->next_action_type = ActionType::MoveLeft;
    obj.input = input;
}
