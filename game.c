#include "game.h"



// used to get the neightbours of a point
const int dy[4] = {-1, 0, 1, 0}, dx[4] = {0, 1, 0, -1};

void play (game* my_game) {
    int state = 0;
    int in_menu;

    while (1) {
        print_game (my_game);
        print_stats (my_game);
        print_messagges (my_game->messagges, my_game->n_messagges);
        my_game->n_messagges = 0;


        if (state == WIN) {
            int save_score = my_game->score;
            int next_level = my_game->level + 1;
            if (next_level == LAST_LEVEL) {
                int final_game_result = final_game (my_game->score);
                if (final_game_result == 0) {
                    return;
                } else {
                    my_game->score = final_game_result;
                    save_score = my_game->score;
                }
                delete_game (my_game);
                my_game = generate_game ();
                my_game->score = save_score;
                my_game->level = next_level;
                state = 0;
                continue;
            } else {
                delete_game (my_game);
                mvprintw (FIELD_OF_VIEW * 2 + 4, col / 2 - 7, "LEVEL COMPLETED");
                mvprintw (FIELD_OF_VIEW * 2 + 6, col / 2 - 6, "-> next level");
                getch();
                my_game = generate_game ();
                my_game->score = save_score;
                my_game->level = next_level;
                state = 0;
                continue;
            }
        } else if (state == DEAD) {
            mvprintw (FIELD_OF_VIEW * 2 + 4, col / 2 - 5, "you died");
            
            submit_score (my_game->score);
            delete_game (my_game);

            mvprintw (FIELD_OF_VIEW * 2 + 6, col / 2 - 5, "play again?");
            if (yes_no (FIELD_OF_VIEW * 2 + 7, col / 2 - 5) == 0) {
                my_game = generate_game ();
                state = 0;
                continue;
            } else {
                return;
            }
        }

        game_instructions ();
        state = NOT_MOVED;

        switch (getch()) {
        case KEY_UP:
            state = update_player (my_game, 0);
            break;
        case KEY_RIGHT:
            state = update_player (my_game, 1);
            break;
        case KEY_DOWN:
            state = update_player (my_game, 2);
            break;
        case KEY_LEFT:
            state = update_player (my_game, 3);
            break;
        case 'e':
            state = update_player (my_game, 4);
            break;
        case 'm':
            print_full_map (my_game);
            break;
        case 'p':
            in_menu = 1;
            while (in_menu) {
                in_menu = 0;
                switch (game_menu()) {
                case 0:
                    break;
                case 1:
                    save_map (my_game);
                    in_menu = 1;
                    break;
                case 2:
                    mvprintw (FIELD_OF_VIEW * 2 + 10, col / 2 - FIELD_OF_VIEW - 1, "any unsaved progress will be lost, are you sure?");
                    if (yes_no (FIELD_OF_VIEW * 2 + 11, col / 2 - FIELD_OF_VIEW - 1) == 0) {
                        move (FIELD_OF_VIEW * 2 + 10, 0);
                        clrtoeol ();
                        move (FIELD_OF_VIEW * 2 + 11, 0);
                        clrtoeol ();
                        game* new_game = load_map (0);
                        if (new_game != NULL) {
                            delete_game (my_game);
                            my_game = new_game;
                        } else {
                            in_menu = 1;
                        }
                    } else {
                        in_menu = 1;
                    }
                    break;
                case 3:
                    mvprintw (FIELD_OF_VIEW * 2 + 10, col / 2 - FIELD_OF_VIEW - 1, "any unsaved progress will be lost, are you sure?");
                    if (yes_no (FIELD_OF_VIEW * 2 + 11, col / 2 - FIELD_OF_VIEW - 1) == 0) {
                        delete_game (my_game);
                        my_game = generate_game ();
                    } else {
                        in_menu = 1;
                        move (FIELD_OF_VIEW * 2 + 10, 0);
                        clrtoeol();
                        move (FIELD_OF_VIEW * 2 + 11, 0);
                        clrtoeol();
                    }
                    break;
                case 4:
                    mvprintw (FIELD_OF_VIEW * 2 + 10, col / 2 - FIELD_OF_VIEW - 1, "any unsaved progress will be lost, are you sure?");
                    if (yes_no (FIELD_OF_VIEW * 2 + 11, col / 2 - FIELD_OF_VIEW - 1) == 0) {
                        delete_game (my_game);
                        return;
                    } else {
                        in_menu = 1;
                        move (FIELD_OF_VIEW * 2 + 10, 0);
                        clrtoeol();
                        move (FIELD_OF_VIEW * 2 + 11, 0);
                        clrtoeol();
                    }
                    break;
                }
            }
            break;
        default:
            break;
        }

        // update enemies
        if (state == STANDARD_MOVE || state == EAT || state == FIGHT) {
            for (int i = 0; i < my_game->n_enemies; i++) {
                int enemy_action = update_enemy (my_game, i, state);
                if (enemy_action == KILLED_PLAYER) {
                    state = DEAD;
                    break;
                }
            }
        }
    }
}

// --------------------------------------------------------------------------------------------------------------

void print_game (game* my_game) {
    clear();

    for (int i = my_game->pl.y - FIELD_OF_VIEW, y = 2; i <= my_game->pl.y + FIELD_OF_VIEW; i++, y++) {
        for (int j = my_game->pl.x - FIELD_OF_VIEW, x = col / 2 - FIELD_OF_VIEW; j <= my_game->pl.x + FIELD_OF_VIEW; j++, x++) {
            if (i < 0 || i >= my_game->h || j < 0 || j >= my_game->w) {
                mvaddch (y, x, GRASS);
            } else {
                my_game->visible[i][j] = 1;
                if (my_game->map[i][j] == PLAYER_NUMBER) {
                    mvaddch (y, x, PLAYER);
                } else if (my_game->map[i][j] == GOAL_NUMBER) {
                    mvaddch (y, x, GOAL);
                } else if (my_game->map[i][j] == WALL_NUMBER) {
                    mvaddch (y, x, WALL);
                } else if (my_game->map[i][j] == FOOD_NUMBER) {
                    mvaddch (y, x, FOOD);
                } else if (my_game->map[i][j] == SWORD_NUMBER) {
                    mvaddch (y, x, SWORD);
                } else if (my_game->map[i][j] == ENEMY_NUMBER) {
                    mvaddch (y, x, ENEMY);
                } else if (my_game->map[i][j] == EMPTY_NUMBER) {
                    if (my_game->walked_on[i][j]) {
                        mvaddch (y, x, STEP);
                    } else {
                        mvaddch (y, x, EMPTY);
                    }
                }
            }
        }
    }

    for (int i = 0; i < my_game->n_enemies; i++) {
        if (
            my_game->enemies[i].y >= my_game->pl.y - FIELD_OF_VIEW &&
            my_game->enemies[i].y <=  my_game->pl.y + FIELD_OF_VIEW &&
            my_game->enemies[i].x >= my_game->pl.x - FIELD_OF_VIEW &&
            my_game->enemies[i].x <= my_game->pl.x + FIELD_OF_VIEW
        ) {
            my_game->enemies[i].last_seen_y = my_game->enemies[i].y;
            my_game->enemies[i].last_seen_x = my_game->enemies[i].x;
        }
    }
}

void print_full_map (game* my_game) {
    clear();
    int start_y = 2, start_x = col / 2 - my_game->w / 2;
    for (int i = 0, y = start_y; i < my_game->h; i++, y++) {
        for (int j = 0, x = start_x; j < my_game->w; j++, x++) {
            if (!my_game->visible[i][j]) {
                mvaddch (y, x, UNSEEN);
            } else if (my_game->map[i][j] == PLAYER_NUMBER) {
                mvaddch (y, x, PLAYER);
            } else if (my_game->map[i][j] == GOAL_NUMBER) {
                mvaddch (y, x, GOAL);
            } else if (my_game->map[i][j] == WALL_NUMBER) {
                mvaddch (y, x, WALL);
            } else if (my_game->map[i][j] == FOOD_NUMBER) {
                mvaddch (y, x, FOOD);
            } else if (my_game->map[i][j] == SWORD_NUMBER) {
                mvaddch (y, x, SWORD);
            } else if (my_game->map[i][j] == EMPTY_NUMBER) {
                if (my_game->walked_on[i][j]) {
                    mvaddch (y, x, STEP);
                } else {
                    mvaddch (y, x, EMPTY);
                }
            }
        }
    }
    for (int i = 0; i < my_game->n_enemies; i++) {
        if (my_game->enemies[i].alive && my_game->enemies[i].last_seen_y != -1) {
            mvaddch (my_game->enemies[i].last_seen_y + start_y, my_game->enemies[i].last_seen_x + start_x, ENEMY);
        }
    }
    map_legend (col / 2 + my_game->w / 2);


    mvprintw (my_game->h + 3, col / 2 - 5, "-> back");
    getch();
}

// --------------------------------------------------------------------------------------------------------------

int update_player (game* my_game, int direction) {
    if (direction == 4) {
        if (my_game->pl.food_units == 0) {
            return NOT_MOVED;
        } else if (my_game->pl.lives == MAX_LIVES) {
            return NOT_MOVED;
        } else {
            my_game->pl.food_units--;
            my_game->pl.lives++;
            add_messagge (my_game, PLAYER_EAT);
            return EAT;
        }
    }
    
    my_game->walked_on[my_game->pl.y][my_game->pl.x] = 1;
    int new_y = my_game->pl.y + dy[direction];
    int new_x = my_game->pl.x + dx[direction];

    int result = STANDARD_MOVE;
    if (my_game->map[new_y][new_x] == WALL_NUMBER) {
        return NOT_MOVED;
    }
    if (my_game->map[new_y][new_x] == GOAL_NUMBER) {
        my_game->score += LEVEL_SCORE;
        my_game->score += my_game->pl.food_units * FOOD_SCORE;
        my_game->score += (MAX_LIVES - my_game->pl.lives) * LIFE_SCORE;
        my_game->score += my_game->pl.has_sword * SWORD_SCORE;
        result = WIN;
    } else if (my_game->map[new_y][new_x] == FOOD_NUMBER) {
        my_game->pl.food_units++;
        add_messagge (my_game, PICK_UP_FOOD);
    } else if (my_game->map[new_y][new_x] == SWORD_NUMBER) {
        int extra_hit_probability = my_game->pl.hit_probability - PLAYER_BASE_HIT_PROBABILITY;
        my_game->pl.has_sword = 1;
        my_game->pl.hit_probability = PLAYER_BASE_HIT_PROBABILITY * 2 + extra_hit_probability;
        add_messagge (my_game, PICK_UP_SWORD);
    } else if (my_game->map[new_y][new_x] == ENEMY_NUMBER) {
        int enemy_index = -1;
        for (int i = 0; i < my_game->n_enemies; i++) {
            if (my_game->enemies[i].alive && my_game->enemies[i].y == new_y && my_game->enemies[i].x == new_x) {
                enemy_index = i;
                break;
            }
        }
        my_game->enemies[enemy_index].has_fought = 1;

        int fight_result = fight (my_game, enemy_index, 1);
        if (fight_result == KILLED_PLAYER) {
            return DEAD;
        } else {
            return FIGHT;
        }
    }

    my_game->map[my_game->pl.y][my_game->pl.x] = EMPTY_NUMBER;
    my_game->map[new_y][new_x] = PLAYER_NUMBER;
    my_game->pl.y = new_y;
    my_game->pl.x = new_x;
    
    return result;
}

int update_enemy (game* my_game, int index, int player_move) {
    if (!my_game->enemies[index].alive) {
        return NOTHING;
    }
    if (!my_game->enemies[index].has_fought) {
        if (player_move != STANDARD_MOVE) {
            for (int i = 0; i < 4; i++) {
                int ny = my_game->enemies[index].y + dy[i];
                int nx = my_game->enemies[index].x + dx[i];
                if (my_game->map[ny][nx] == PLAYER_NUMBER) {
                    int state = fight (my_game, index, 0);

                    my_game->enemies[index].has_fought = 0;
                    return state;
                }
            }
        }

        int direction;
        int** distance_map = get_distance_map (my_game, my_game->enemies[index].y, my_game->enemies[index].x);
        if (distance_map[my_game->pl.y][my_game->pl.x] <= ENEMY_FIELD_OF_VIEW) {
            direction = get_best_move (
                my_game,
                my_game->enemies[index].y,
                my_game->enemies[index].x,
                my_game->pl.y,
                my_game->pl.x
            );
        } else {
            direction = rand() % 4;
        }

        free_matrix (distance_map, my_game->h);

        int new_y = my_game->enemies[index].y + dy[direction];
        int new_x = my_game->enemies[index].x + dx[direction];

        if (my_game->map[new_y][new_x] == EMPTY_NUMBER) {
            my_game->map[my_game->enemies[index].y][my_game->enemies[index].x] = EMPTY_NUMBER;
            my_game->map[new_y][new_x] = ENEMY_NUMBER;
            my_game->enemies[index].y = new_y;
            my_game->enemies[index].x = new_x;
        }
    }
    my_game->enemies[index].has_fought = 0;
    return NOTHING;
}

int fight (game* my_game, int enemy_index, int is_moving) {
    int player_hit = 0;
    int enemy_hit = 0;
    if (is_moving) {
        if (rand() % 60 <= my_game->pl.hit_probability) {
            player_hit = 1;
            my_game->pl.hit_probability = PLAYER_BASE_HIT_PROBABILITY * (my_game->pl.has_sword + 1);
            add_messagge (my_game, PLAYER_HIT_ENEMY);
        } else {
            my_game->pl.hit_probability += HIT_PROBABILITY_CHANGE;
            add_messagge (my_game, PLAYER_MISS_ENEMY);
        }
    }
    if (rand() % 50 <= my_game->enemies[enemy_index].hit_probability) {
        enemy_hit = 1;
        my_game->enemies[enemy_index].hit_probability -= HIT_PROBABILITY_CHANGE * 2;
        add_messagge (my_game, ENEMY_HIT_PLAYER);
    } else {
        add_messagge (my_game, ENEMY_MISS_PLAYER);
    }


    if (player_hit) {
        my_game->map[my_game->enemies[enemy_index].y][my_game->enemies[enemy_index].x] = EMPTY_NUMBER;
        my_game->enemies[enemy_index].alive = 0;
        my_game->score += KILL_SCORE;
    }
    if (enemy_hit) {
        my_game->pl.lives--;
        if (my_game->pl.lives == 0) {
            my_game->map[my_game->pl.y][my_game->pl.x] = EMPTY_NUMBER;
            return KILLED_PLAYER;
        }
    }

    if (player_hit && enemy_hit) {
        return BOTH_HIT;
    } else if (player_hit) {
        return PLAYER_HIT;
    } else if (enemy_hit) {
        return ENEMY_HIT;
    } else {
        return NOTHING;
    }
}

void add_messagge (game* my_game, int message) {
    my_game->messagges[my_game->n_messagges] = message;
    my_game->n_messagges++;
}

// --------------------------------------------------------------------------------------------------------------

game* generate_game () {
    int** distance_map = NULL;
    int generate;
    game* my_game;

    do {
        generate = 0;
        my_game = (game*)calloc (1, sizeof (game));        

        // select dimension
        my_game->w = rand() % (MAX_MAP_DIM - MIN_MAP_DIM) + MIN_MAP_DIM + 2;
        my_game->h = rand() % (MAX_MAP_DIM - MIN_MAP_DIM) + MIN_MAP_DIM + 2;
        my_game->w += my_game->w % 2 + 1;
        my_game->h += my_game->h % 2 + 1;

        // initialize game map, everything to walls
        my_game->map = allocate_matrix (my_game->h, my_game->w);

        // generate maze
        create_map (my_game, 1, 1);


        my_game->pl = init_player ();
        // set player starting point
        do {
            my_game->pl.y = rand() % (my_game->h - 2) + 1;
            my_game->pl.x = rand() % (my_game->w - 2) + 1;
        } while (my_game->map[my_game->pl.y][my_game->pl.x] != EMPTY_NUMBER);
        my_game->map[my_game->pl.y][my_game->pl.x] = PLAYER_NUMBER;

        distance_map = get_distance_map (my_game, my_game->pl.y, my_game->pl.x);

        // set ending point
        do {
            my_game->goal_y = rand() % (my_game->h - 2) + 1;
            my_game->goal_x = rand() % (my_game->w - 2) + 1;
        } while (my_game->map[my_game->goal_y][my_game->goal_x] != EMPTY_NUMBER);
        my_game->map[my_game->goal_y][my_game->goal_x] = GOAL_NUMBER;

        // if the ending point is to close to the player, regenerate the whole map
        if (distance_map[my_game->goal_y][my_game->goal_x] < MIN_GOAL_DISTANCE) {
            free_matrix (distance_map, my_game->h);
            delete_game (my_game);
            generate = 1;
            continue;
        }

        // place enemies
        my_game->n_enemies = 0;
        int enemies_quantity = rand() % (MAX_ENEMIES_QUANTITY - MIN_ENEMIES_QUANTITY) + MIN_ENEMIES_QUANTITY;
        for (int i = 0; i < enemies_quantity; i++) {
            my_game->enemies[my_game->n_enemies] = init_enemy ();
            do {
                my_game->enemies[my_game->n_enemies].y = rand() % (my_game->h - 2) + 1;
                my_game->enemies[my_game->n_enemies].x = rand() % (my_game->w - 2) + 1;
            } while (my_game->map[my_game->enemies[my_game->n_enemies].y][my_game->enemies[my_game->n_enemies].x] != EMPTY_NUMBER ||
                     distance_map[my_game->enemies[my_game->n_enemies].y][my_game->enemies[my_game->n_enemies].x] < MIN_ENEMY_DISTANCE);
            
            my_game->map[my_game->enemies[my_game->n_enemies].y][my_game->enemies[my_game->n_enemies].x] = ENEMY_NUMBER;
            my_game->n_enemies++;
        }
        free_matrix (distance_map, my_game->h);
    } while (generate);

    my_game->visible = allocate_matrix (my_game->h, my_game->w);
    my_game->walked_on = allocate_matrix (my_game->h, my_game->w);

    // place sword
    int sword_y, sword_x;
    do {
        sword_y = rand() % (my_game->h - 2) + 1;
        sword_x = rand() % (my_game->w - 2) + 1;
    } while (my_game->map[sword_y][sword_x] != EMPTY_NUMBER);
    my_game->map[sword_y][sword_x] = SWORD_NUMBER;

    // place foods
    int food_quantity = rand() % (MAX_FOOD_QUANTITY - 1) + 1;
    int food_y, food_x;
    for (int i = 0; i < food_quantity; i++) {
        do {
            food_y = rand() % (my_game->h - 2) + 1;
            food_x = rand() % (my_game->w - 2) + 1;
        } while (my_game->map[food_y][food_x] != EMPTY_NUMBER);
        my_game->map[food_y][food_x] = FOOD_NUMBER;
    }

    return my_game;
}

// recursive backtracker
void create_map (game* my_game, int y, int x) {
    int extra_dig = 0;
    while (1) {
        int available_directions[4];
        int n_available_directions = 0;
        for (int i = 0; i < 4; i++) {
            int ny = y + dy[i] * 2, nx = x + dx[i] * 2;
            if (ny > 0 && ny < my_game->h - 1 && nx > 0 && nx < my_game->w) {
                if (my_game->map[ny][nx] == WALL_NUMBER) {
                    available_directions[n_available_directions] = i;
                    n_available_directions++;
                }
                if (!extra_dig && rand() % 25 == 0) {
                    int extra_dig_y = y + dy[i];
                    int extra_dig_x = x + dx[i];
                    if (extra_dig_y > 0 && extra_dig_y < my_game->h - 1 && extra_dig_x > 0 && extra_dig_x < my_game->w) {
                        my_game->map[extra_dig_y][extra_dig_x] = EMPTY_NUMBER;
                        extra_dig = 1;
                    }
                }
            }
        }

        if (n_available_directions == 0) {
            return;
        }

        int direction = available_directions[rand() % n_available_directions];
        my_game->map[y + dy[direction]][x + dx[direction]] = EMPTY_NUMBER;

        int ny = y + dy[direction] * 2;
        int nx = x + dx[direction] * 2;
        my_game->map[ny][nx] = EMPTY_NUMBER;

        create_map (my_game, ny, nx);
    }
}

// --------------------------------------------------------------------------------------------------------------

int save_map (game* my_game) {
    char file_name[1000];
    char path[1010] = "saves/";
    mvprintw (FIELD_OF_VIEW * 2 + 10, col / 2 - FIELD_OF_VIEW - 1, "file name: ");

    echo();
    curs_set (1);
    getstr (file_name);
    curs_set (0);
    noecho();

    FILE* f = fopen (strcat (path, file_name), "w");
    if (f == NULL) {
        mvprintw (FIELD_OF_VIEW * 2 + 11, col / 2 - FIELD_OF_VIEW - 1, "missing folder \"saves\"");
        getch();
        move (FIELD_OF_VIEW * 2 + 10, 0);
        clrtoeol();
        move (FIELD_OF_VIEW * 2 + 11, 0);
        clrtoeol();
        return 1;
    }
    move (FIELD_OF_VIEW * 2 + 10, 0);
    clrtoeol();
    move (FIELD_OF_VIEW * 2 + 11, 0);
    clrtoeol();

    fprintf (f, "%d %d\n", my_game->h, my_game->w);
    fprintf (f, "%d %d\n", my_game->level, my_game->score);
    fprintf (f, "%d %d %d %d\n", my_game->pl.lives, my_game->pl.food_units, my_game->pl.has_sword, my_game->pl.hit_probability);
    for (int i = 0; i < my_game->h; i++) {
        for (int j = 0; j < my_game->w; j++) {
            fprintf (f, "%d", my_game->map[i][j]);
        }
        fprintf (f, "\n");
    }
    for (int i = 0; i < my_game->h; i++) {
        for (int j = 0; j < my_game->w; j++) {
            fprintf (f, "%d", my_game->visible[i][j]);
        }
        fprintf (f, "\n");
    }
    for (int i = 0; i < my_game->h; i++) {
        for (int j = 0; j < my_game->w; j++) {
            fprintf (f, "%d", my_game->walked_on[i][j]);
        }
        fprintf (f, "\n");
    }

    for (int i = 0; i < my_game->n_enemies; i++) {
        fprintf (f, "%d %d\n", my_game->enemies[i].y, my_game->enemies[i].x);
        fprintf (f, "%d %d\n", my_game->enemies[i].last_seen_y, my_game->enemies[i].last_seen_x);
        fprintf (f, "%d %d\n", my_game->enemies[i].alive, my_game->enemies[i].hit_probability);
    }

    fclose (f);
    return 0;
}

game* load_map (int menu) {
    int y, x;
    if (menu) {
        y = 9;
        x = col / 2 - 4;
    } else {
        y = FIELD_OF_VIEW * 2 + 10;
        x = col / 2 - FIELD_OF_VIEW - 1;
    }

    char file_name[1000];
    char path[1010] = "saves/";
    mvprintw (y, x, "file name: ");

    echo();
    curs_set (1);
    getstr (file_name);
    curs_set (0);
    noecho();

    if (file_name[0] == '\0') {
        return NULL;
    }

    FILE* f = fopen (strcat (path, file_name), "r");
    if (f == NULL) {
        mvprintw (y + 1, x, "can't open file");
        getch();
        move (y, 0);
        clrtoeol();
        move (y + 1, 0);
        clrtoeol();
        return NULL;
    }
    move (y, 0);
    clrtoeol();
    move (y + 1, 0);
    clrtoeol();
    game* my_game = (game*)calloc (1, sizeof (game));

    fscanf (f, "%d %d%*c", &my_game->h, &my_game->w);
    fscanf (f, "%d %d%*c", &my_game->level, &my_game->score);
    fscanf (f, "%d %d %d %d%*c", &my_game->pl.lives, &my_game->pl.food_units, &my_game->pl.has_sword, &my_game->pl.hit_probability);
    my_game->map = allocate_matrix (my_game->h, my_game->w);
    my_game->visible = allocate_matrix (my_game->h, my_game->w);
    my_game->walked_on = allocate_matrix (my_game->h, my_game->w);
    my_game->n_enemies = 0;
    for (int i = 0; i < my_game->h; i++) {
        for (int j = 0; j < my_game->w; j++) {
            my_game->map[i][j] = fgetc (f) - '0';
            if (my_game->map[i][j] == PLAYER_NUMBER) {
                my_game->pl.y = i;
                my_game->pl.x = j;
            } else if (my_game->map[i][j] == GOAL_NUMBER) {
                my_game->goal_y = i;
                my_game->goal_x = j;
            }
        }
        fgetc (f);
    }
    for (int i = 0; i < my_game->h; i++) {
        for (int j = 0; j < my_game->w; j++) {
            my_game->visible[i][j] = fgetc (f) - '0';
        }
        fgetc (f);
    }
    for (int i = 0; i < my_game->h; i++) {
        for (int j = 0; j < my_game->w; j++) {
            my_game->walked_on[i][j] = fgetc (f) - '0';
        }
        fgetc (f);
    }

    while (1) {
        my_game->enemies[my_game->n_enemies] = init_enemy ();
        if (fscanf (f, "%d %d%*c", &my_game->enemies[my_game->n_enemies].y, &my_game->enemies[my_game->n_enemies].x) != 2) {
            break;
        }
        fscanf (f, "%d %d%*c", &my_game->enemies[my_game->n_enemies].last_seen_y, &my_game->enemies[my_game->n_enemies].last_seen_x);
        fscanf (f, "%d %d%*c", &my_game->enemies[my_game->n_enemies].alive, &my_game->enemies[my_game->n_enemies].hit_probability);
        my_game->n_enemies++;
    }

    fclose (f);
    return my_game;
}

// --------------------------------------------------------------------------------------------------------------

int** allocate_matrix (int h, int w) {
    int** matrix = (int**)malloc (h * sizeof (int*));
    for (int i = 0; i < h; i++) {
        matrix[i] = (int*)calloc (w, sizeof (int));
    }
    return matrix;
}

void free_matrix (int** matrix, int height) {
    if (matrix == NULL) {
        return;
    }
    for (int i = 0; i < height; i++) {
        free (matrix[i]);
    }
    free (matrix);
}

void delete_game (game* my_game) {
    free_matrix (my_game->map, my_game->h);
    free_matrix (my_game->visible, my_game->h);
    free_matrix (my_game->walked_on, my_game->h);
    my_game->map = NULL;
    my_game->visible = NULL;
    my_game->walked_on = NULL;
    free (my_game);
}

// --------------------------------------------------------------------------------------------------------------

int** get_distance_map (game* my_game, int start_y, int start_x) {
    int** distance_map = allocate_matrix (my_game->h, my_game->w);
    for (int i = 0; i < my_game->h; i++) {
        for (int j = 0; j < my_game->w; j++) {
            if (my_game->map[i][j] == WALL_NUMBER) {
                distance_map[i][j] = -1;
            } else {
                distance_map[i][j] = 1e9;
            }
        }
    }
    dfs (distance_map, start_y, start_x, 0);
    return distance_map;
}

void dfs (int** maze, int y, int x, int dist) {
    if (maze[y][x] == -1 || dist >= maze[y][x]) {
        return;
    }
    maze[y][x] = dist;
    for (int i = 0; i < 4; i++) {
        dfs (maze, y + dy[i], x + dx[i], dist + 1);
    }
}

int get_best_move (game* my_game, int start_y, int start_x, int goal_y, int goal_x) {
    int best = 1e9;
    int best_move = -1;

    for (int i = 0; i < 4; i++) {
        int ny = start_y + dy[i], nx = start_x + dx[i];
        if (my_game->map[ny][nx] == PLAYER_NUMBER) {
            return i;
        } else if (my_game->map[ny][nx] != EMPTY_NUMBER) {
            continue;
        }

        int** distances = get_distance_map (my_game, ny, nx);
        if (distances[goal_y][goal_x] < best) {
            best = distances[goal_y][goal_x];
            best_move = i;
        }

        free_matrix (distances, my_game->h);
    }
    return best_move;
}