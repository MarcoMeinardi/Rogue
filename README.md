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
Don't delete "saves" folder or you won't be able to save and load games again. <br>
You will find the instructions in to the game. <br>
If you have any problem with the leaderboard, just recreate the file "leaderboard.dat"<br>
Suddenly this game works only on Linux, but you can still play it on Windows by running it on the [WSL](https://www.microsoft.com/en-us/p/ubuntu/9nblggh4msv6?activetab=pivot:overviewtab).<br>
Unfortunately, running the game on the WSL causes rendering bugs that i haven't been able to fix.
