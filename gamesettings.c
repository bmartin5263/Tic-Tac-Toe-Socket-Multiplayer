#include "header.h"

void beginGame(struct GameSettings* gs) {
    gs->isComplete = 0;
    gs->consecutiveWin = -1;
    
    struct Player* player1 = gs->players[0];
    struct Player* player2 = gs->players[1];
    
    /*
    printf("Player 1 is named %s with symbol %c\n",player1->name, player1->symbol);
    printf("Player 2 is named %s with symbol %c\n",player2->name, player2->symbol);
    printf("First Turn is %i\n",gs->firstTurn);
    printf("My id is %i\n",gs->myID);
    
    recieveBlankInput();
    */
    
    printf("\e[1;1H\e[2J");
    printf("Beginning Game! %s (%c) vs %s (%c)\n", player1->name, player1->symbol, player2->name, player2->symbol);
    printf("First Turn Will Be %s\n",gs->players[gs->firstTurn]->name);
    sleep(3);
}

void endGame(struct GameSettings* gs) {
    for (int i = 0; i < 2; i++) {
        free(gs->players[i]);
    }
    free(gs);
}

struct GameSettings* initializeGame() {
    struct GameSettings* gs = malloc(sizeof(struct GameSettings));
    char prompt[128];
    char playerNameInput[BUFFER_LEN];
    char symbolInput[BUFFER_LEN];
    char symbol;
    ssize_t bytesRead;
    
    snprintf(prompt, 128, "Host Game (y/n): ");
    char hostInput[BUFFER_LEN];
    recieveInput(hostInput, prompt, 0);
    while (!checkValidChoice(gs,hostInput)) {
        printf("\033[91mInvalid Input\033[0m\n");
        recieveInput(hostInput, prompt, 0);
    }
    
    if (hostInput[0] == 'y' || hostInput[0] == 'Y') {
        
        snprintf(prompt, 128, "Enter Your Name: ");
        recieveInput(playerNameInput, prompt, 0);
        while (!checkValidName(gs,playerNameInput, 12)) {
            printf("\033[91mName Invalid\033[0m\n");
            recieveInput(playerNameInput, prompt, 0);
        }
        
        snprintf(prompt, 128, "Enter Your Symbol: ");
        recieveInput(symbolInput, prompt, 0);
        while (!checkValidSymbol(gs, symbolInput)) {
            printf("\033[91mSymbol Invalid\033[0m\n");
            recieveInput(symbolInput, prompt, 0);
        }
        symbol = symbolInput[0];
        
        gs->players[0] = createPlayer(playerNameInput, symbol, 0);
        
        /////////////////////////////////////////
        
        printf("\e[1;1H\e[2J");
        char nameStr[BUFFER_LEN];
        char symStr[BUFFER_LEN];
        char firstTurnString[10];
        int listenSock, clientSock;
        
        struct sockaddr_in serverAddress;
        
        listenSock = socket(AF_INET, SOCK_STREAM, 0);
        
        bzero( &serverAddress, sizeof(serverAddress));
        
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = htons(INADDR_ANY);
        serverAddress.sin_port = htons(22000);
        
        bind(listenSock, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
        
        listen(listenSock, 5);
        
        char hostbuf[512];
        char *ipbuf;
        struct hostent *hostentry;
        int ret;
        ret = gethostname(hostbuf,sizeof(hostbuf));
        hostentry = gethostbyname(hostbuf);
        //ipbuf = inet_ntoa(*((struct in_addr *)hostentry->h_addr_list[0]));
        
        //printf("Host IP: %s\n", ipbuf);
        printf("Waiting for Opponent...\n");
        
        clientSock = accept(listenSock, (struct sockaddr*) NULL, NULL);
        write(clientSock, playerNameInput, strlen(playerNameInput)+1);
        printf("Found Opponent! Entering Name...\n");
        bzero(nameStr, BUFFER_LEN);
        bytesRead = read(clientSock,nameStr,BUFFER_LEN);
        if (bytesRead <= 0) {
            printf("\033[91mConnection Lost :(\033[0m\n");
            exit(1);
        }
        
        write(clientSock, symbolInput, 2);
        printf("Entering Symbol...\n");
        bzero(symStr, BUFFER_LEN);
        bytesRead = read(clientSock,symStr,BUFFER_LEN);
        
        if (bytesRead <= 0) {
            printf("\033[91mConnection Lost :(\033[0m\n");
            exit(1);
        }
        
        printf("%s's symbol is %c\n",nameStr, symStr[0]);
    
        gs->players[1] = createPlayer(nameStr, symStr[0], 1);
        
        int firstTurn = getRandomNumberInRange(-1, 1);
        sprintf(firstTurnString, "%d", firstTurn);
        printf("Sending First Turn %i, %s\n",firstTurn, firstTurnString);
        write(clientSock, firstTurnString, 2);
        
        gs->firstTurn = firstTurn;
        gs->myID = 0;
        gs->sock = clientSock;
        
    } else {
        
        int sock;
        char sendline[BUFFER_LEN];
        char hostName[BUFFER_LEN];
        char hostSymbol[BUFFER_LEN];
        char firstTurnString[10];
        char *ptr;
        struct sockaddr_in clientAddress;
        
        sock = socket(AF_INET,SOCK_STREAM,0);
        bzero(&clientAddress,sizeof clientAddress);
        
        clientAddress.sin_family=AF_INET;
        clientAddress.sin_port=htons(22000);
        
        char ipInput[BUFFER_LEN];
        snprintf(prompt, 128, "Enter Host IP: ");
        recieveInput(ipInput, prompt, 0);
        
        ipInput[strcspn(ipInput, "\n")] = 0;
        
        if (strlen(ipInput) == 1) {
            //inet_pton(AF_INET,ipInput,&(clientAddress.sin_addr));
            inet_pton(AF_INET,"127.0.0.1",&(clientAddress.sin_addr));
        } else {
            inet_pton(AF_INET,ipInput,&(clientAddress.sin_addr));
        }
        
        printf("Connecting to Host...\n");
        int status = connect(sock,(struct sockaddr *)&clientAddress,sizeof(clientAddress));
        if  (status < 0)
        {
            printf("\033[91mConnection Failed :(\033[0m\n");
            exit(1);
        }
        
        printf("\e[1;1H\e[2J");
        
        bzero(hostName, BUFFER_LEN);
        bzero(sendline, BUFFER_LEN);
        bytesRead = read(sock,hostName,BUFFER_LEN);
        
        if (bytesRead <= 0) {
            printf("\033[91mConnection Dropped :(\033[0m\n");
            exit(1);
        }
        
        printf("Connected to %s\n",hostName);
        
        snprintf(prompt, 128, "Enter Your Name: ");
        recieveInput(playerNameInput, prompt, 0);
        while (!checkValidName(gs,playerNameInput, 12) || strcmp(playerNameInput, hostName) == 0) {
            printf("\033[91mName Invalid\033[0m\n");
            recieveInput(playerNameInput, prompt, 0);
        }
        
        write(sock,playerNameInput,strlen(playerNameInput)+1);
        
        bzero(hostSymbol, BUFFER_LEN);
        bzero(sendline, BUFFER_LEN);
        bytesRead = read(sock,hostSymbol,BUFFER_LEN);
        
        if (bytesRead <= 0) {
            printf("\033[91mConnection Lost :(\033[0m\n");
            exit(1);
        }
        
        printf("%s's symbol is %c\n",hostName, hostSymbol[0]);
        
        snprintf(prompt, 128, "Enter Your Symbol: ");
        recieveInput(symbolInput, prompt, 0);
        while (!checkValidSymbol(gs, symbolInput) || hostSymbol[0] == symbolInput[0]) {
            printf("\033[91mSymbol Invalid\033[0m\n");
            recieveInput(symbolInput, prompt, 0);
        }
        symbol = symbolInput[0];
        
        write(sock,symbolInput,2);
        
        gs->players[0] = createPlayer(hostName, hostSymbol[0], 0);
        gs->players[1] = createPlayer(playerNameInput, symbol, 1);
        
        bytesRead = read(sock,firstTurnString,10);
        
        if (bytesRead <= 0) {
            printf("\033[91mConnection Lost :(\033[0m\n");
            exit(1);
        }
        
        printf("First turn is %s\n", firstTurnString);
        
        long firstTurnLong = strtol(firstTurnString, &ptr, 10);
        int firstTurn = (int)firstTurnLong;
        gs->firstTurn = firstTurn;
        gs->myID = 1;
        gs->sock = sock;
        
        
    }
    
    return gs;
    
}