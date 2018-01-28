#include "header.h"

void nextTurn(struct Match* m) {
    printf("\e[1;1H\e[2J");
    struct TicBoard* board = m->board;
    struct Player* player = m->gs->players[m->turn];
    char send[BUFFER_LEN];
    char receive[BUFFER_LEN];
    char *ptr;
    size_t bytesRead;
    bzero(send, BUFFER_LEN);
    bzero(receive, BUFFER_LEN);
        
    if (m->gs->myID == m->turn) {
    
        int turnComplete = 0;
        char userInput[BUFFER_LEN];
        int space;
        
        while(!turnComplete) {
        
            printBoard(board);
            printf("%s please select a space.\n",player->name);
            recieveInput(userInput, "Space: ", 1);
            space = checkValidSpace(userInput, board);
            
            while(space == -1 || space == -2) {
                if (space == -1) printf("Invalid Space!\n");
                else printf("Space Already Taken!\n");
                printf("%s please select a space.\n",player->name);
                recieveInput(userInput, "Space: ", 1);
                space = checkValidSpace(userInput, board);
            }
            
            place(board, player, space);
            sprintf(send, "%d", space);
            write(m->gs->sock,send,strlen(send)+1);
            
            turnComplete = 1;
            
        }
        
    } else {
        
        printBoard(board);
        printf("Waiting for %s\n",player->name);
        
        bytesRead = read(m->gs->sock,receive,BUFFER_LEN);
        
        if (bytesRead <= 0) {
            printf("%s Forfeited!\n",player->name);
            sleep(3);
            m->isComplete = 1;
            m->playerWhoWon = m->gs->myID;
            return;
        }
        
        long spaceLong = strtol(receive, &ptr, 10);
        int space = (int)spaceLong;
        place(board, player, space);
        
    }
    
    if (!board->canPlacePiece) {
        m->isComplete = 1;
        m->playerWhoWon = m->board->winnerID;
    } else {
        if (m->turn == 0) m->turn = 1;
        else m->turn = 0;
    }
}

struct Match* initializeMatch(struct GameSettings* gs) {
    struct Match* m = malloc(sizeof(struct Match));
    struct TicBoard* b = initializeEmptyBoard();
    
    m->gs = gs;
    m->board = b;
    m->isComplete = 0;
    m->turn = gs->firstTurn;
    m->playerWhoWon = -1;
    return m;
}

int endMatch(struct Match* m) {
    printf("\e[1;1H\e[2J");
    printBoard(m->board);
    int winner = m->playerWhoWon;
    free(m->board);
    free(m);
    return winner;
}
