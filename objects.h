#pragma once

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// main menu
#define NEW_GAME 0
#define LOAD_GAME 1
#define LEADERBOARD 2
#define QUIT 3


// map costrains
#define MIN_MAP_DIM 20
#define MAX_MAP_DIM 30
#define MIN_GOAL_DISTANCE 70
#define MIN_ENEMY_DISTANCE 15


// objects
#define FIELD_OF_VIEW 3
#define ENEMY_FIELD_OF_VIEW 5

#define MAX_LIVES 3
#define MAX_FOOD_QUANTITY 3
#define MAX_ENEMIES_QUANTITY 5
#define MIN_ENEMIES_QUANTITY 3

#define WALL_NUMBER 0
#define EMPTY_NUMBER 1
#define PLAYER_NUMBER 2
#define GOAL_NUMBER 3

#define FOOD_NUMBER 4
#define SWORD_NUMBER 5
#define ENEMY_NUMBER 6

#define PLAYER '+'
#define GOAL 'X'
#define WALL 4194401
#define GRASS '#'
#define EMPTY ' '
#define UNSEEN '?'
#define STEP '.'

#define FOOD '@'
#define SWORD 't'
#define ENEMY 'H'

// update function return values
#define NOT_MOVED -1
#define STANDARD_MOVE 0
#define WIN 1
#define DEAD 2
#define FIGHT 3
#define EAT 4

// fight probabilities
#define PLAYER_BASE_HIT_PROBABILITY 20
#define ENEMY_BASE_HIT_PROBABILITY 25
#define HIT_PROBABILITY_CHANGE 5

// fight function return values
#define NOTHING 0
#define PLAYER_HIT 1
#define ENEMY_HIT 2
#define BOTH_HIT 3
#define KILLED_PLAYER 4

// scoring system
#define KILL_SCORE 30
#define FOOD_SCORE 5
#define LIFE_SCORE -5
#define LEVEL_SCORE 50
#define SWORD_SCORE 30

// fight messagges
#define PLAYER_MISS_ENEMY 0
#define PLAYER_HIT_ENEMY 1
#define ENEMY_MISS_PLAYER 2
#define ENEMY_HIT_PLAYER 3
#define PLAYER_EAT 4
#define PICK_UP_FOOD 5
#define PICK_UP_SWORD 6



// terminal sizes
int row_, col;

typedef struct {
    int y, x;
    int lives;
    int has_sword;
    int food_units;
    int hit_probability;
} player;

typedef struct {
    int y, x;
    int alive;
    int has_fought;
    int last_seen_y, last_seen_x;
    int hit_probability;
} enemy;

typedef struct {
    int w, h;
    int** map;
    
    int score;
    int level;
    player pl;
    int** walked_on;
    int goal_y, goal_x;
    int** visible;

    int n_enemies;
    enemy enemies[MAX_ENEMIES_QUANTITY];

    int messagges[5];
    int n_messagges;
} game;


player init_player ();
enemy init_enemy ();