#include "header.h"

int getRandomNumberInRange(int low, int high) {
    return rand() % (high + 1 - low) + low;
}

void recieveInput(char* userInput, char* prompt, int makeUpper) {
    printf("%s",prompt);
    fgets(userInput, BUFFER_LEN, stdin);
    if (makeUpper) {
        char* characterPtr;
        for (characterPtr = userInput; *characterPtr != '\0'; characterPtr++)
            *characterPtr = toupper(*characterPtr);
    }
}

void recieveBlankInput() {
    char buffer[8];
    fgets(buffer, 8, stdin);
}

int getSpaceNumber(char* userInput) {
    for (int i = 0; i < NUM_SPACES; i++)
        if (strncmp(userInput,SPACE_NAMES[i],2) == 0 || strncmp(userInput,SPACE_NAMES_REVERSED[i],2) == 0) return i;
    return -1;
}

int checkValidSpace(char* userInput, struct TicBoard* board) {
    int spaceNum = getSpaceNumber(userInput);
    if (spaceNum == -1) return -1;
    if (board->spaces[spaceNum] == ' ')
        return spaceNum;
    else
        return -2;
}

int checkValidRange(char* userInput, int low, int high) {
    int numPlayers = userInput[0] - '0';
    if (numPlayers >= low && numPlayers <= high) {
        return numPlayers;
    }
    return -1;
}

int checkNameTaken(struct GameSettings* gs, char* name) {
    for (int i = 0; i < gs->numPlayersEntered; i++) {
        struct Player* p = gs->players[i];
        if (strcmp(name,p->name) == 0) return 0;
    }
    return 1;
}

int checkValidName(struct GameSettings* gs, char* userInput, int maxLength) {
    userInput[strcspn(userInput, "\n")] = 0;
    int length = (int)strlen(userInput);
    if (userInput[0] == '\n') return 0;
    if (length > maxLength) return 0;
    return 1;
}

int checkValidSymbol(struct GameSettings* gs, char* userInput) {
    char symbol = userInput[0];
    if (symbol == ' ' || symbol == '\n') return 0;
    return 1;
}

int checkValidChoice(struct GameSettings* gs, char* userInput) {
    char choice = userInput[0];
    if (choice == 'y' || choice == 'n' || choice == 'Y' || choice == 'N') return 1;
    return 0;
}