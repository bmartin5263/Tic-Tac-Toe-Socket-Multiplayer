#include "header.h"

struct Player* createPlayer(char* name, char symbol, int idNum) {
    // this is a comment
    struct Player* p = malloc(sizeof(struct Player));
    strcpy(p->name, name);
    p -> symbol = symbol;
    p -> idNumber = idNum;
    return p;
}
