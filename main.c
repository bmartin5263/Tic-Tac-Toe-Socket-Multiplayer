//
//  main.c
//  TicTacToe
//
//  Created by Brandon Martin on 6/22/17.
//  Copyright © 2017 Brandon Martin. All rights reserved.
//

#include "header.h"


int main(int argc, const char * argv[]) {
    printf("\e[1;1H\e[2J");
    printf("Welcome to Tic-Tac-Toe!\n");
    
    struct GameSettings* gs = initializeGame();
    beginGame(gs);
    
    while (!gs->isComplete) {
        
        struct Match* match = initializeMatch(gs);
        int winner;
        
        while (!match->isComplete) {
            nextTurn(match);
        }
        
        winner = endMatch(match);
        
        if (1) {
            printf("%s Wins!\n",gs->players[winner]->name);
            gs->isComplete = 1;
        } else {
            if (winner == -1) {
                printf("Draw! Beginning Next Match. *press enter*");
                gs->consecutiveWin = -1;
                recieveBlankInput();
            }
            else {
                printf("%s Won a Match!\nWin One More To Be Victorious! *press enter*\n",gs->players[winner]->name);
                gs->consecutiveWin = winner;
                recieveBlankInput();
            }
            if (gs->firstTurn == 0) gs->firstTurn = 1;
            else gs->firstTurn = 0;
        }
    }
    
    endGame(gs);
    
}
