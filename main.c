#include "objects.h"
#include "game.h"
#include "gui.h"
#include "leaderboard.h"

int main () {
    srand ((unsigned)time (NULL));
    initscr();
    getmaxyx (stdscr, row_, col);
    keypad(stdscr, TRUE);
    noecho();
    curs_set (0);

    game* my_game;
    int playing = 1;
    int in_menu;

    do {
        my_game = NULL;
        do {
            in_menu = 0;
            switch (main_menu()) {
            case NEW_GAME:
                my_game = generate_game ();
                break;
            case LOAD_GAME:
                my_game = load_map (1);
                if (my_game == NULL) {
                    in_menu = 1;
                }
                break;
            case LEADERBOARD:
                print_leaderboard ();
                in_menu = 1;
                break;
            case QUIT:
                playing = 0;
                break;
            default:
                in_menu = 1;
                break;
            }
        } while (in_menu);
        
        if (playing) {
            play (my_game);
        }
    } while (playing);

    endwin();

    return 0;
}
