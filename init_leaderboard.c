#include <stdio.h>
#include <stdlib.h>

int main () {
    FILE* f = fopen ("leaderboard.dat", "wb");
    char* str = (char*)calloc (11, sizeof (char));
    int n = -1;
    fwrite (str, sizeof (char), 10, f);
    fwrite (&n, sizeof (int), 1, f);
    
    fclose (f);
    return 0;
}