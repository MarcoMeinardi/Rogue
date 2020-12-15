#include "leaderboard.h"

void print_leaderboard () {
    clear();
    mvprintw (2, (col - 5) / 2, "ROGUE");
    mvprintw (3, col / 2 - 5, "LEADERBOARD");
    mvprintw (5, col / 2 - 9, "player");
    mvprintw (5, col / 2 + 5, "score");
    mvprintw (6, col / 2 - 12, "----------------------");

    FILE* f = fopen ("leaderboard.dat", "rb");
    if (f == NULL) {
        mvprintw (8, col / 2 - 12, "can't find \"leaderboard.dat\"");
        getch();
        return;
    }

    int row = 0;
    char player_name[11] = {0};
    int score;
    int position = 0;
    while (fread (player_name, sizeof (char), 10, f) == 10) {
        fread (&score, sizeof (int), 1, f);
        position++;
        mvprintw (row + 7, col / 2 - 12, "%2d", position);
        mvprintw (row + 7, col / 2 - 9, player_name);
        mvprintw (row + 7, col / 2 + 3, "%7d", score);
        row++;
    }

    if (position == 0) {
        row++;
        mvprintw (7, col / 2 - 12, "no one submitted his score yet");
    }

    fclose (f);

    while (1) {
        int choice = 0;
        int in_select;
        do {
            in_select = 1;
            for (int i = 0; i < 3; i++) {
                move (row + 8 + i, 0);
                clrtoeol();
            }
                
            mvprintw (row + 8, col / 2 - 8, "back");
            mvprintw (row + 8, col / 2 + 4, "reset");

            mvprintw (row + 8, col / 2 - 11 + 12 * choice, "->");
            switch (getch ()) {
            case KEY_LEFT:
            case KEY_RIGHT:
                choice++;
                choice %= 2;
                break;
            case '\n':
                in_select = 0;
                break;
            default:
                break;
            }
        } while (in_select);
        
        if (choice == 1) {
            mvprintw (row + 9, col / 2 - 18, "every score will be lost, are you sure?");
            if (yes_no (row + 10, col / 2 - 3) == 0) {
                reset_leaderboard ();
                print_leaderboard ();
                break;
            } else {
                continue;
            }
        } else {
            break;
        }
    }
}

void submit_score (int score) {
    int in_submit;
    do {
        in_submit = 0;
        mvprintw (FIELD_OF_VIEW * 2 + 6, col / 2 - 5, "submit score?");
        if (yes_no (FIELD_OF_VIEW * 2 + 7, col / 2 - 5) == 0) {
            char player_name[1000];
            mvprintw (FIELD_OF_VIEW * 2 + 8, col / 2 - 5, "insert name: ");
            echo();
            curs_set (1);
            getstr (player_name);
            curs_set (0);
            noecho();


            if (update_leaderboard (score, player_name) == 1) {
                mvprintw (FIELD_OF_VIEW * 2 + 9, col / 2 - 5, "can't open \"leaderboard.dat\"");
                getch();
                in_submit = 1;
            }
        }
        for (int i = 0; i < 4; i++) {
            move (FIELD_OF_VIEW * 2 + 6 + i, 0);
            clrtoeol ();
        }
    } while (in_submit);
}

int update_leaderboard (int score, char* player_name) {
    FILE* f = fopen ("leaderboard.dat", "rb+");

    if (f == NULL) {
        return 1;
    }

    char read_player[11] = {0};
    int read_score;
    int cnt = 0;
    int original = 1;

    while (fread (read_player, sizeof (char), 10, f) == 10) {
        fread (&read_score, sizeof (int), 1, f);
        if (score > read_score || (score == read_score && !original)) {
            fseek (f, -1 * (sizeof (int) + sizeof (char) * 10), SEEK_CUR);
            fwrite (player_name, sizeof (char), 10, f);
            fwrite (&score, sizeof (int), 1, f);
            fflush (f);

            strcpy (player_name, read_player);
            score = read_score;
            original = 0;
        }
        cnt++;
    }
    if (cnt < MAX_PLAYERS) {
        fwrite (player_name, sizeof (char), 10, f);
        fwrite (&score, sizeof (int), 1, f);
        fflush (f);
    }

    fclose (f);
    return 0;
}

void reset_leaderboard () {
    FILE* f = fopen ("leaderboard.dat", "wb");
    fclose (f);
}
