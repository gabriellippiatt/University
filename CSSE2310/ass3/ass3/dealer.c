#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

#define WRITE 1
#define READ 0
#define INITARG 3

struct GameStructure {
    char** siteList;
    char** boardPositions;
    char* sites;
    char* pathIn;
    char* pathOut;
    char* deckOut;
    int numChars;
    int numberOfSites;
    int playerCount;
    int currentPlayer;
};

struct PlayerStructure {
    int playerID;
    int playerAtSite;
    int playerMoney;
    int playerV1;
    int playerV2;
    int playerPoints;
    int cardA;
    int cardB;
    int cardC;
    int cardD;
    int cardE;
};

struct PipingStructure {
    int* dealerToPlayerPipe;
    int* playerToDealerPipe;
    pid_t* processID;
};

struct PointerStructure {
    FILE* playerRead;
    FILE* playerWrite;
    FILE* dealerRead;
    FILE* dealerWrite;
};

// for use with strtol
char* p;

// function used to read a line from input
char* read_line(FILE* file) {
    char* line = malloc(sizeof(char) * 80);
    int currentPossition = 0;
    int nextCharacter = 0;
    int capacity = 80;

    while (1) {
        nextCharacter = fgetc(file);
        if (nextCharacter == EOF || nextCharacter == '\n') {
            line[currentPossition] = '\0';
            return line;
        } else {
            if (currentPossition >= capacity - 3) {
                capacity = capacity + 80;
                line = realloc(line, sizeof(char) * capacity);
            }
            line[currentPossition++] = (char)nextCharacter;
        }
    }
}

// check that a correct number of args has been given
void check_args(int argc, char** argv) {
    if(argc < 4) {
        fprintf(stderr, "Usage: 2310dealer deck path p1 {p2}\n");
        exit(1);
    }
}

// check for valid path
void invalid_path(char** argv, char* path, struct GameStructure* game) {
    // create a for loop that checks for number of sites 
    //greater than one digit
    game->pathIn = (char*)calloc(game->numChars, sizeof(char));
    
    int numberOfChars = strlen(path);

    char* numSiteChars = (char*)malloc(sizeof(char) * 100);

    for (int i = 0; i < numberOfChars; i++) {
        if(path[i] != ';') {
            numSiteChars[i] = path[i]; 
        } else {
            break;
        }
    }
    
    int numSites = strtol(numSiteChars, &p, 10);
    char* siteNumChar = (char*)malloc(sizeof(char) * 100);
    sprintf(siteNumChar, "%d", numSites);
    int siteNumLen = strlen(siteNumChar);

    int totalPathLen = (siteNumLen + 1 + (numSites * 3));
    game->numChars = totalPathLen;
    
    if (numSites < 2) {
        fprintf(stderr, "Error reading path\n");
        exit(3);
    } else if (path[numberOfChars - 3] != ':' && 
            path[numberOfChars - 2] != ':' && 
            path[numberOfChars - 1] != '-' && 
            path[2] != ':' && path[3] != ':' &&
            path[4] != '-') {
        fprintf(stderr, "Error reading path\n");
        exit(3);
    } else if (numSites == 2) {
        if (strcmp(path, "2;::-::-") != 0) {
            fprintf(stderr, "Error reading path\n");
            exit(3);
        }
    }
    game->pathIn = path;
}

// deck needs at least 4 cards
void invalid_deck(char* deck, struct GameStructure* game) {
    // char representation of the number of cards
    char* charCards = (char*)malloc(sizeof(char) * 100);
    
    for(int i = 0; i < 1000; i++) {
        if(deck[i] != 'A' || deck[i] != 'B' || deck[i] != 'C' || 
                deck[i] != 'D' || deck[i] != 'E') {
            charCards[i] = deck[i];
        }
    }
    int numCards = strtol(charCards, &p, 10);
    char* lenChars = (char*)malloc(sizeof(char) * 100);

    sprintf(lenChars, "%d", numCards);
    int numLen = strlen(lenChars);

    int lenNum = strlen(lenChars);

    char cards[numCards];

    int counter = 0;
    for(int i = lenNum; i < (numCards + lenNum); i++) {
        cards[counter] = deck[i];
        counter++;
    }

    for(int i = 0; i < numCards; i++) {
        if((cards[i] != 'A') && (cards[i] != 'B') && (cards[i] != 'C') && 
                (cards[i] != 'D') && (cards[i] != 'E')) {
            fprintf(stderr, "Error reading deck\n");
            exit(2);
        }
    }

    game->deckOut = (char*)malloc(sizeof(char) * (numLen + numCards));

    for(int i = 0; i < (numLen + numCards); i++) {
        game->deckOut[i] = deck[i];
    }
}

// checks the file inputs
void bad_file(char** argv, struct GameStructure* game) {
    FILE* fp;
    char* deck = argv[1];
    char* path = argv[2];
    fp = fopen(deck, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error reading deck\n");
        exit(2);
    } else if (fp != NULL) {
        char* deckString = (char*)malloc(sizeof(char) * 100);
        for(int i = 0; i < 1000; i++) {
            deckString[i] = fgetc(fp);
        }
        invalid_deck(deckString, game);
    }
    fclose(fp);
    fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error reading path\n");
        exit(3);
    } else {
        char* pathInput = (char*)malloc(sizeof(char) * 100);
        for(int i = 0; i < 1000; i++) {
            pathInput[i] = fgetc(fp);
        }
        invalid_path(argv, pathInput, game);
    }
    fclose(fp);
}

// check if valid player types are given
void bad_player(int argc, char** argv) {
    for(int i = 3; i < argc; i++) {
        int len = strlen(argv[i]);
        if(argv[i][len - 1] == 'A') {
            // nothing
        } else if(argv[i][len - 1] == 'B') {
            // nothing
        } else {
            fprintf(stderr, "Error starting process\n");
            exit(4);
        }
    }
}

// initialise player
void initialise_player(int playerCount, struct GameStructure* game, 
        struct PlayerStructure* play) {

    game->playerCount = playerCount;
    //fprintf(stderr, "player count %d\n", game->playerCount);

    for(int i = 0; i < game->playerCount; i++) {

        struct PlayerStructure* currentplayer = &play[i];

        currentplayer->playerMoney = 7;
        currentplayer->playerV1 = 0;
        currentplayer->playerV2 = 0;
        currentplayer->playerPoints = 0;
        currentplayer->cardA = 0;
        currentplayer->cardB = 0;
        currentplayer->cardC = 0;
        currentplayer->cardD = 0;
        currentplayer->cardE = 0;
    }
}

// store the given player types (not the best way to do it)
char* get_player_type(int argc, char** argv, 
        struct GameStructure* game, int player) {

    char* playerType = (char*)malloc(sizeof(char) * 100);
    char* playerTypeInput = (char*)malloc(sizeof(char) * 100);
    playerTypeInput = argv[player + 3];
    int length = strlen(playerTypeInput);
    if(playerTypeInput[length - 1] == 'A') {
        playerType = "./2310A";
    } else if(playerTypeInput[length - 1] == 'B') {
        playerType = "./2310B";
    }
    return playerType;
}

// set up all the pipes
void setting_up_pipes(int argc, char** argv, struct GameStructure* game,
        struct PipingStructure* piper, struct PointerStructure** pointer) {
    int numPlayers = (argc - 3);
    int suppressStderr[2];
    pipe(suppressStderr);
    // dealer -> player
    piper->dealerToPlayerPipe = (int*)calloc(2 * numPlayers, sizeof(int));
    // player -> dealer
    piper->playerToDealerPipe = (int*)calloc(2 * numPlayers, sizeof(int));
    // process IDs
    piper->processID = (pid_t*)malloc(numPlayers * sizeof(pid_t));
    // create the same number of processes as there are players
    for(int i = 0; i < numPlayers; i++) {
        pipe(piper->dealerToPlayerPipe);
        pipe(piper->playerToDealerPipe);
        piper->processID[i] = fork();
        if((piper->processID[i] < 0)) {
            fprintf(stderr, "Error starting process\n");
            exit(4);
        }
        // code for child process (the players)
        if(piper->processID[i] == 0) {
            // closes the write end of the pipe
            close(piper->dealerToPlayerPipe[WRITE]);
            // closes the read end of the pipe
            close(piper->playerToDealerPipe[READ]);

            dup2(suppressStderr[READ], STDERR_FILENO);
            close(suppressStderr[READ]);
            dup2(suppressStderr[WRITE], STDERR_FILENO);
            close(suppressStderr[WRITE]);
            // make stdin the read end of the pip
            dup2(piper->dealerToPlayerPipe[READ], STDIN_FILENO);
            dup2(piper->playerToDealerPipe[WRITE], STDOUT_FILENO);
            // number of players stored as a string
            char* numPlayersChar = (char*)malloc(sizeof(char) * 100);
            sprintf(numPlayersChar, "%d", numPlayers);
            // this player id stored as a string
            char* playerChar = (char*)malloc(sizeof(char) * 100);
            sprintf(playerChar, "%d", i);
            char* playerType = get_player_type(argc, argv, game, i);
            execlp(argv[i + INITARG], playerType, 
                    numPlayersChar, playerChar, NULL);
            exit(0);
        }
        close(piper->dealerToPlayerPipe[READ]);
        close(piper->playerToDealerPipe[WRITE]);
        pointer[i]->dealerWrite = 
                fdopen(piper->dealerToPlayerPipe[WRITE], "w");
        pointer[i]->dealerRead = fdopen(piper->playerToDealerPipe[READ], "r");
    }
}

// send the path to the player
void send_path(char** argv, int player, struct GameStructure* game, 
        struct PipingStructure* piper, struct PointerStructure** pointer) {
    game->pathOut = (char*)calloc(game->numChars, sizeof(char));

    for(int i = 0; i < game->numChars; i++) {
        game->pathOut[i] = game->pathIn[i];
    }

    fprintf(pointer[player]->dealerWrite, "%s\n", game->pathOut);
    fflush(pointer[player]->dealerWrite);
}

// stores the path in the struct
void store_path(char* sites, int numberOfSites, 
        struct GameStructure* game) {
    game->siteList = (char**)malloc(sizeof(char*) * (numberOfSites));
    for (int i = 0; i < numberOfSites; i++) {
        game->siteList[i] = 
                (char*)malloc(sizeof(char) * 4);
    }
    for (int i = 0; i < numberOfSites; i++) {
        int k = 0;
        for (int j = i * 3; j < ((i * 3) + 3); j++) {
            game->siteList[i][k] = sites[j];
            k++;
        }
    }
}

// print the sites
void print_sites(struct GameStructure* game) {
    char* path = (char*)malloc(sizeof(char) * 100);
    path = game->pathOut;
    int length = strlen(path);

    char* numberOfSitesChars = (char*)malloc(sizeof(char) * 100);
    for (int i = 0; i < length; i++) {
        if(path[i] != ';') {
            numberOfSitesChars[i] = path[i];
        } else {
            break;
        }
    }

    int numberOfSites = strtol(numberOfSitesChars, &p, 10);
    game->numberOfSites = numberOfSites;
    game->sites = (char*)malloc(sizeof(char) * 100);
    char* lenChar = (char*)malloc(sizeof(char) * 100);
    sprintf(lenChar, "%d", numberOfSites);
    int len = strlen(lenChar);

    int j = 0;
    for (int i = strlen(numberOfSitesChars) + 1; i < length; i++) {
        game->sites[j] = path[i];
        j++;
    }

    j = 0;
    for (int i = len + 1; i < length; i++) {
        if ((i - numberOfSites) % 3 == 0) {
            game->sites[j] = ' ';
            j++;
        } else {
            game->sites[j] = path[i];
            j++;
        }
    }

    fprintf(stdout, "%s\n", game->sites);
}

// initialises the board where the players move around 
//(this is underneath the sites)
void initialise_board(int playerCount,
        struct GameStructure* game, struct PlayerStructure* play) {

    game->playerCount = playerCount;

    int numberOfSites = game->numberOfSites;
    game->boardPositions = (char**)malloc(sizeof(char*) * (playerCount));

    for (int i = 0; i < playerCount; i++) {
        game->boardPositions[i] = 
                (char*)malloc(sizeof(char) * numberOfSites);
    }
    for (int i = 0; i < playerCount; i++) {
        for (int j = 0; j < numberOfSites; j++) {
            game->boardPositions[i][j] = ' ';
        }
    }
    for (int i = 0; i < playerCount; i++) {

        struct PlayerStructure* currentplayer = &play[i];
        int playerNumber = ((playerCount - 1) - i);
        game->boardPositions[i][0] = playerNumber + '0';
        currentplayer->playerAtSite = 0;
    }
    for (int i = 0; i < playerCount; i++) {
        for (int j = 0; j < numberOfSites; j++) {
            fprintf(stdout, "%c  ", game->boardPositions[i][j]);
        }
        fprintf(stdout, "\n");
    }
}

// checks to make sure that the player has been initialised
void carrot_checker(int player, struct PointerStructure** pointer) {
    FILE* fp;
    char response;

    fp = pointer[player]->dealerRead;
    if(fp == NULL) {
        fprintf(stderr, "Error starting process\n");
        exit(4);
    }
    response = fgetc(pointer[player]->dealerRead);
    if (response == '^') {
        // nothing
    } else {
        fprintf(stdout, "bummer no carrot\n");
    }
}

// generates a hap message
void hap_message_generator(int player, int column, struct GameStructure* game, 
        struct PlayerStructure* play, struct PointerStructure** pointer) {
    
    struct PlayerStructure* currentplayer = &play[player];

    int p, n, s, m, c;
    s = m = c = 0;
    p = player;
    n = column;

    if(game->siteList[n][0] == 'M') {
        m = 3;
        currentplayer->playerMoney += m;
    } else if(game->siteList[n][0] == 'V' && game->siteList[n][1] == '1') {
        currentplayer->playerV1 += 1;
    } else if(game->siteList[n][0] == 'V' && game->siteList[n][1] == '2') {
        currentplayer->playerV2 += 1;
    } else if(game->siteList[n][0] == 'D') {
        int temp = currentplayer->playerMoney;
        currentplayer->playerMoney = 0;
        s = (temp / 2);
        currentplayer->playerPoints += (temp / 2);

    // calculate the cards (for player type B)
    } else if(game->siteList[n][0] == 'R') {
        // do later
    }

    int playerMoney = currentplayer->playerMoney;
    int playerV1 = currentplayer->playerV1;
    int playerV2 = currentplayer->playerV2;
    int playerPoints = currentplayer->playerPoints;
    int cardA = currentplayer->cardA;
    int cardB = currentplayer->cardB;
    int cardC = currentplayer->cardC;
    int cardD = currentplayer->cardD;
    int cardE = currentplayer->cardE;



    fprintf(stdout, "Player %d Money=%d V1=%d V2=%d Points=%d A=%d B=%d C=%d D=%d E=%d\n", /////// DONT KNOW HOW TO FIX
            p, playerMoney, playerV1,
            playerV2, playerPoints, cardA, 
            cardB, cardC, cardD, 
            cardE);

    for(int i = 0; i < game->playerCount; i++) {
        fprintf(pointer[i]->dealerWrite, "HAP%d,%d,%d,%d,%d\n",
            p, n, s, m, c);
        fflush(pointer[i]->dealerWrite);
    }
}

// Prints the board from memory (except for empty rows)
void print_board(int playerCount, int numberOfSites, 
        struct GameStructure* game) {
    for (int i = 0; i < playerCount; i++) {
        for (int j = 0; j < numberOfSites; j++) {
            if(game->boardPositions[i][j] != ' ') {
                for (int k = 0; k < numberOfSites; k++) {
                    fprintf(stdout, "%c  ", game->boardPositions[i][k]);
                }
                fprintf(stdout, "\n");
                break;
            }
        }
    }
}

// removes the player from there postion
void remove_player(int player, struct GameStructure* game) {
    int counter = 0;
    for (int i = 0; i < game->playerCount; i++) {
        for (int j = 0; j < game->numberOfSites; j++) {
            if(game->boardPositions[i][j] == player + '0') {
                game->boardPositions[i][j] = ' ';
                counter++;
            }
        }
    }
}

// if move is valid then make the move
void move(int player, int column, struct GameStructure* game, 
        struct PlayerStructure* play) {
    // check how many players are in this site and whether there is 
    int playercount = game->playerCount;

    // count how many people are at this site
    // if count < capacity then place then run find_space
    for (int i = 0; i < playercount; i++) {
        if(game->boardPositions[i][column] != ' ') {
            // do nothing
        } else {
            remove_player(player, game);
            game->boardPositions[i][column] = player + '0';
            break;
        }
    }
    print_board(playercount, game->numberOfSites, game);
}

// make initial turns for the player at the starting barrier
void initial_turns(int player, struct GameStructure* game, 
        struct PlayerStructure* play, struct PointerStructure** pointer) {
    fprintf(pointer[player]->dealerWrite, "%s\n", "YT");
    fflush(pointer[player]->dealerWrite);

    FILE* fp;
    char response[3];

    fp = pointer[player]->dealerRead;
    if(fp == NULL) {
        fprintf(stderr, "Error starting process\n");
        exit(4);
    }
    response[0] = fgetc(pointer[player]->dealerRead);
    response[1] = fgetc(pointer[player]->dealerRead);
    response[2] = fgetc(pointer[player]->dealerRead);
    int column = response[2] - '0';

    if (response[0] == 'D' && response[1] == 'O') {
        store_path(game->sites, game->numberOfSites, game);
        print_sites(game);        
        move(player, column, game, &play[player]);
        hap_message_generator(player, column, game, &play[0], pointer);
    } else {
        // nothing
    }

}

// find the next player to have a turn
void next_player(int numPlayers, struct GameStructure* game, struct PlayerStructure* play, struct PointerStructure** pointer) {
    int player = 0;
    int playerFound = 0;
    for (int i = (game->playerCount - 1); i >= 0; i--) {
        for (int j = 0; j < game->numberOfSites; j++) {
            if(game->boardPositions[i][j] != ' ') {
                player = game->boardPositions[i][j] - '0';
                playerFound = 1;
                break;
            }
        }
        if(playerFound == 1) {
            break;
        }
    }
    initial_turns(player, game, &play[0], pointer);
}

// checks the final barrier to see if it is the end of the game
int check_final_barrier(int numPlayers, struct GameStructure* game) {
    int counter = 0;
    int j = game->numberOfSites - 1;
    for (int i = 0; i < numPlayers; i++) {
        if(game->boardPositions[i][j] != ' ') {
            counter++;
        }
    }
    if(counter == numPlayers) {
        return 1;
    } else {
        return 0;
    }
}

// ends the game normally
void end_game(int player, struct PointerStructure** pointer) {
    fprintf(pointer[player]->dealerWrite, "DONE\n");
    fflush(pointer[player]->dealerWrite);
}

// adds points from cards to players
void card_points(int player, struct GameStructure* game, 
        struct PlayerStructure* play) {
    struct PlayerStructure* currentplayer = &play[player];

    int cardA = currentplayer->cardA;
    int cardB = currentplayer->cardB;
    int cardC = currentplayer->cardC;
    int cardD = currentplayer->cardD;
    int cardE = currentplayer->cardE;
    int counter = 0;

    while(cardA != 0 || cardB != 0 || cardC != 0 || cardD != 0 || cardE != 0) {
        if(cardA > 0) {
            cardA -= 1;
            counter++;
        }
        if(cardB > 0) {
            cardB -= 1;
            counter++;
        }
        if(cardC > 0) {
            cardC -= 1;
            counter++;
        }
        if(cardD > 0) {
            cardD -= 1;
            counter++;
        }
        if(cardE > 0) {
            cardE -= 1;
            counter++;
        }
        if(counter == 5) {
            currentplayer->playerPoints += 10;
        } else if (counter == 4) {
            currentplayer->playerPoints += 7;
        } else if (counter == 3) {
            currentplayer->playerPoints += 5;
        } else if (counter == 2) {
            currentplayer->playerPoints += 3;
        } else if (counter == 1) {
            currentplayer->playerPoints += 1;
        }
        counter = 0;
    }


}

// calculates the scores of the players
void scoring(struct GameStructure* game, struct PlayerStructure* play) {
    fprintf(stdout, "Scores:");
    bool score = false;

    for(int i = 0; i < game->playerCount; i++) {
        struct PlayerStructure* currentplayer = &play[i];
        currentplayer->playerPoints += currentplayer->playerV1;
        currentplayer->playerPoints += currentplayer->playerV2;
        card_points(i, game, play);
        if(isdigit(currentplayer->playerPoints) == 0) {
            score = true;
        }
    }
    if (score) {
        fprintf(stdout, " ");
    }
    for(int i = 0; i < game->playerCount; i++) {
        if(i > 0) {
            fprintf(stdout, ",");
        }
        struct PlayerStructure* currentplayer = &play[i];
        fprintf(stdout, "%d", currentplayer->playerPoints);   
    }
    fprintf(stdout, "\n");
    exit(0);
}

// main function
int main(int argc, char** argv) { 
    check_args(argc, argv);
    struct GameStructure game;
    struct PipingStructure piper;
    struct PointerStructure** pointer;
    
    pointer = 
            (struct PointerStructure**)calloc(argc - 3, sizeof(
            struct PointerStructure*));
    for(int i = 0; i < argc - 3; i++) {
        pointer[i] = (struct PointerStructure*)calloc(1, sizeof(
                struct PointerStructure));
    }

    int numPlayers = (argc - 3);
    
    struct PlayerStructure play[numPlayers];

    initialise_player(numPlayers, &game, &play[0]);
    
    

    bad_file(argv, &game);
    bad_player(argc, argv);
    setting_up_pipes(argc, argv, &game, &piper, pointer);

    for(int i = 0; i < numPlayers; i++) {
        send_path(argv, i, &game, &piper, pointer);
    }
    for(int i = 0; i < numPlayers; i++) {
        carrot_checker(i, pointer);
    }

    print_sites(&game);
    initialise_board(numPlayers, &game, &play[0]);

    for(int i = 0; i < numPlayers; i++) {
        initial_turns(i, &game, &play[0], pointer);
    }

    while(1) {
        next_player(numPlayers, &game, &play[0], pointer);
        if(check_final_barrier(numPlayers, &game) == 1) {
            break;
        }
    }
    for(int i = 0; i < numPlayers; i++) {
        end_game(i, pointer);
    }

    scoring(&game, &play[0]);
    

    return 0;
}
