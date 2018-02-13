#include "header.h"

struct Player* createPlayer(char* name, char symbol, int idNum) {
    struct Player* p = malloc(sizeof(struct Player));
    strcpy(p->name, name); //a comment
    p -> symbol = symbol;
    p -> idNumber = idNum;
    return p;
}
