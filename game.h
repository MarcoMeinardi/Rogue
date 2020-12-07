#pragma once

#include "objects.h"
#include "gui.h"
#include "leaderboard.h"


/*
    moves:
    0: up
    1: right
    2: down
    3: left
*/

// game loop
void play (game* my_game);

void print_game (game* my_game);
void print_full_map (game* my_game);

int update_player (game* my_game, int direction);
int update_enemy (game* my_game, int index, int player_move);
int fight (game* my_game, int enemy_index, int is_moving);
void add_messagge (game* my_game, int message);

game* generate_game ();
void create_map (game* my_game, int y, int x);

game* load_map (int menu);
int save_map (game* my_game);

int** allocate_matrix (int h, int w);
void free_matrix (int** matrix, int height);
void delete_game (game* my_game);

// return a map with the distance from player to every point
int** get_distance_map (game* my_game, int start_y, int start_x);
void dfs (int** maze, int y, int x, int dist);

// return the best move to reach a given target from a given starting position  
int get_best_move (game* my_game, int start_y, int start_x, int goal_y, int goal_x);