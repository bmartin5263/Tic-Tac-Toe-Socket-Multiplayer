# Tic Tac Toe Socket Multiplayer

## Getting Started
__Language:__ C

__Compile:__
```
gcc -c main.c board.c inputHandler.c match.c gamesettings.c player.c
gcc -o main.o board.o inputHandler.o match.o gamesettings.o player.o -o tictactoe
```

__Usage:__
```
./tictactoe
```
The Uno Basic repository includes the following python modules:
* game.py
* main.py
* match.py
* ui.py
* uno_objs.py
* uno_players.py

Uno Basic does not require the download of any third-party modules and only uses standard Python library modules.

## How To Play

Uno Basic is a close recreation to the classic Uno Game and includes many of the same rules, however some rules have
been modified to add to a more competitive experience.

### Rules
* 2-4 Players.
* First turn is selected at random.
* Players place cards onto the pile that match either the color or value of the piles topmost card.
* First player to empty hand of all cards wins.
* Winning player gets points based on the values of the remaining cards in the opponents hands
* Draw Four cards may only be used when no other card can be legally placed.
* Reverse Cards act as skips during games with only two players.
* If deck is empty and player has no usuable cards, the player may pass their own turn.