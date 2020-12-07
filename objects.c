#include "objects.h"

player init_player () {
    player new_player;
    new_player.y = 1;
    new_player.x = 1;
    new_player.lives = MAX_LIVES;
    new_player.has_sword = 0;
    new_player.food_units = 0;
    new_player.hit_probability = PLAYER_BASE_HIT_PROBABILITY;

    return new_player;
}

enemy init_enemy () {
    enemy new_enemy;
    new_enemy.y = 1;
    new_enemy.x = 1;
    new_enemy.alive = 1;
    new_enemy.has_fought = 0;
    new_enemy.last_seen_x = -1;
    new_enemy.last_seen_y = -1;
    new_enemy.hit_probability = ENEMY_BASE_HIT_PROBABILITY;

    return new_enemy;
}