# Rogue
Simple roguelike ascii game, with procedural generated mazes.
<br><br>
You will need "ncurses" library. <br>
You can get it with:<br>
```sudo apt-get install libncurses5-dev libncursesw5-dev```<br>
To compile the program run the following command:<br>
```gcc -Wall -o rogue main.c game.c gui.c objects.c leaderboard.c -lncurses```<br>
And to execute it:<br>
```./rogue```<br>
<br>
Do not delete the "saves" folder or you will no longer be able to save and load games <br>
You will find the instructions in the game. <br>
If you have problems with the leaderboard, just re-create the "leaderboard.dat" file<br>
Suddenly this game only works on Linux, but you can still play it on Windows by running it on [WSL](https://www.microsoft.com/en-us/p/ubuntu/9nblggh4msv6?activetab=pivot:overviewtab).<br>
Unfortunately, running the game on WSL causes rendering bugs that I haven't been able to fix.
