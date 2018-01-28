#include "header.h"

void printBoard(struct TicBoard* board) {
    printf("\n\t   L   M   R\n"
           "\tT: %c | %c | %c \n"
           "\t  -----------\n"
           "\tC: %c | %c | %c \n"
           "\t  -----------\n"
           "\tB: %c | %c | %c \n\n",
           board->spaces[0],board->spaces[1],board->spaces[2],board->spaces[3],
           board->spaces[4],board->spaces[5],board->spaces[6],board->spaces[7],board->spaces[8]);
}

int checkTriad(struct TicBoard* board, int triadNum) {
    char* spaces = board->spaces;
    for (int i = 1; i < 3; i++) {
        char currentSpaceSymbol = spaces[TRIADS[triadNum][i]];
        if (currentSpaceSymbol == ' ' || currentSpaceSymbol != spaces[TRIADS[triadNum][i-1]]) {
            return 0;
        }
    }
    return 1;
}

int checkWin(struct TicBoard* board) {
    for (int i = 0; i < NUM_TRIADS; i++) {
        if (checkTriad(board, i)) return 1;
    }
    return 0;
}

void clearBoard(struct TicBoard* board) {
    for (int i = 0; i < 9; i++)
        board->spaces[i] = ' ';
    board->free = 9;
    board->canPlacePiece = 1;
}

void place(struct TicBoard* board, struct Player* player, int space) {
    board->spaces[space] = player->symbol;
    board->free--;
    if (checkWin(board)) {
        board->winnerID = player->idNumber;
        board->canPlacePiece = 0;
    }
    if (board->free == 0) board->canPlacePiece = 0;
}

/// CONSTRUCTOR ///

struct TicBoard* initializeEmptyBoard() {
    struct TicBoard* board = malloc(sizeof(struct TicBoard));
    clearBoard(board);
    board->winnerID = -1;
    return board;
}