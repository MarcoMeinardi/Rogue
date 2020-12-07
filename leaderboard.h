#pragma once

#include "objects.h"
#include "game.h"

#define MAX_PLAYERS 10

void print_leaderboard ();
void submit_score (int score);
int update_leaderboard (int score, char* player_name);
void reset_leaderboard ();