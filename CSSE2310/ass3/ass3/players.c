#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "players.h"

// used for strtol
char* p;

// read stdin and strore it in an array
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

// make sure that the player ID is valid
void invalid_id(char* number, struct GameStructure* game, 
        struct PlayerStructure* play) {

    if (isdigit(number[0])) {

        int id = strtol(number, &p, 10);

        struct PlayerStructure* currentplayer = &play[id];
        currentplayer->playerID = id;
        
        if (currentplayer->playerID < 0 || 
                currentplayer->playerID > game->playerCount) {
            fprintf(stderr, "Invalid ID\n");
            exit(3);
        }
    } else {
        fprintf(stderr, "Invalid ID\n");
        exit(3);
    }
}

// make sure that the path is valid
void invalid_path(char* path) {
    // create a for loop that checks for number of sites 
    //greater than one digit

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
    
    if (numSites < 2) {
        fprintf(stderr, "Invalid path\n");
        exit(4);
    } else if (path[numberOfChars - 3] != ':' && 
            path[numberOfChars - 2] != ':' && 
            path[numberOfChars - 1] != '-' && 
            path[2] != ':' && path[3] != ':' &&
            path[4] != '-') {
        fprintf(stderr, "Invalid path\n");
        exit(4);
    } else if (numSites == 2) {
        if (strcmp(path, "2;::-::-") != 0) {
            fprintf(stderr, "Invalid path\n");
            exit(4);
        }
    }
}

// run initial error checks
void error_checking(int argc, char** argv, struct GameStructure* game, 
        struct PlayerStructure* play) {
    invalid_id(argv[2], game, play);
}

// create an array that stores the sites
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
void print_sites(char* path, struct GameStructure* game) {
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
    
    int j = 0;
    for (int i = strlen(numberOfSitesChars) + 1; i < length; i++) {
        game->sites[j] = path[i];
        j++;
    }
    store_path(game->sites, numberOfSites, game);
    j = 0;
    for (int i = strlen(numberOfSitesChars) + 1; i < length; i++) {
        if ((i - strlen(numberOfSitesChars)) % 3 == 0) {
            game->sites[j] = ' ';
            j++;
        } else {
            game->sites[j] = path[i];
            j++;
        }
    }
    fprintf(stderr, "%s\n", game->sites);
    //free(sites);
}

// initialise the players inventory (each player starts with 7 money)
void initialise_player(struct GameStructure* game, 
        struct PlayerStructure* play) {

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

// initialises the board where the players move around 
//(this is underneath the sites)
void initialise_board(int playerCount, int numberOfSites, 
        struct GameStructure* game, struct PlayerStructure* play) {
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
            fprintf(stderr, "%c  ", game->boardPositions[i][j]);
        }
        fprintf(stderr, "\n");
    }
}

// Check for EOF
void eof_check(FILE* fp) {
    if (feof(fp)) {
        fprintf(stderr, "Communications error\n");
        exit(6);
    }
}

// Prints the board from memory (except for empty rows)
void print_board(int playerCount, int numberOfSites, 
        struct GameStructure* game) {
    for (int i = 0; i < playerCount; i++) {
        for (int j = 0; j < numberOfSites; j++) {
            //fprintf(stderr, "%c  ", game->boardPositions[i][j]);
            if(game->boardPositions[i][j] != ' ') {
                for (int k = 0; k < numberOfSites; k++) {
                    fprintf(stderr, "%c  ", game->boardPositions[i][k]);
                }
                fprintf(stderr, "\n");
                break;
            }
        }
    }
}

// check to see if the player move is valid
// 0 is valid 1 is not valid
int valid_move(int column, struct GameStructure* game) {

    int siteCapacity = 0;

    if(game->siteList[column][2] != '-') {
        siteCapacity = game->siteList[column][2] - '0';
    } else {
        siteCapacity = 1000;
    }

    int counter = 0;

    for (int i = 0; i < game->playerCount; i++) {
        if(game->boardPositions[i][column] != ' ') {
            counter++;
        }
    }

    if(siteCapacity > counter) {
        return 0;
    } else {
        return 1;
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
    fprintf(stderr, "%s\n", game->sites);
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

// check for valid HAP message variables
int valid_hap(int* variables, struct GameStructure* game) {
    if((variables[0] <= game->playerCount) && 
            (variables[1] <= game->numberOfSites) &&
            (variables[4] < 6)) {
        return 0;
    } else {
        return 1;
    }
}

// add the points from the sites to the players
void site_points(int player, int column, struct GameStructure* game, 
        struct PlayerStructure* play) {
    struct PlayerStructure* currentplayer = &play[player];

    if(game->siteList[column][0] == 'V' && 
            game->siteList[column][1] == '1') {
        currentplayer->playerV1 = currentplayer->playerV1 + 1;
    } else if(game->siteList[column][0] == 'V' && 
            game->siteList[column][1] == '2') {
        currentplayer->playerV2 = currentplayer->playerV2 + 1;
    }
}

// add the cards to the player
void add_cards(int player, int* variables, struct PlayerStructure* play) {
    struct PlayerStructure* currentplayer = &play[player];
    if(variables[4] == 1) {
        currentplayer->cardA = currentplayer->cardA + 1;
    } else if(variables[4] == 2) {
        currentplayer->cardB = currentplayer->cardB + 1;
    } else if(variables[4] == 3) {
        currentplayer->cardC = currentplayer->cardC + 1;
    } else if(variables[4] == 4) {
        currentplayer->cardD = currentplayer->cardD + 1;
    } else if(variables[4] == 5) {
        currentplayer->cardE = currentplayer->cardE + 1;
    }

}

// store the values given in the HAP message
void hap_message(char* hapMessage, struct GameStructure* game, struct PlayerStructure* play) { ///////// FUNCTION TOO LONG
    // need to account for the variables being more than one char
    int messageLength = strlen(hapMessage);
    char* temp = (char*)malloc(sizeof(char) * 100);
    int* variables = (int*)malloc(sizeof(int) * 100);
    int counter1 = 0;
    int counter2 = 0;
    int counter3 = 0;

    for(int i = 0; i < messageLength; i++) {
        if(hapMessage[i] == ',') {
            counter3++;
        }
    }

    if(counter3 != 4) {
        //do nothing
    } else {
        // variables begin at hapMessage[3]
        for (int i = 3; i < messageLength - 1; i++) {
            while(hapMessage[i] != ',') {
                temp[counter1] = hapMessage[i];
                i++;
                counter1++;
            }
            // add the temp chars to the variable array
            variables[counter2] = strtol(temp, &p, 10);
            counter2++;            
            // clear the temp string
            for (int i = 0; i < counter1; i++) {
                temp[i] = '\0';
            }
            counter1 = 0;
        }

        variables[counter2] = hapMessage[messageLength - 1] - '0';

        if(valid_hap(variables, game) == 0) {

            int player = variables[0];
            struct PlayerStructure* currentplayer = &play[player];

            int newSite = variables[1];
            currentplayer->playerAtSite = newSite;
            // additional score
            currentplayer->playerPoints = 
                    currentplayer->playerPoints + variables[2];
            // change in money for player
            currentplayer->playerMoney += variables[3];
            
            add_cards(player, variables, play);

            site_points(player, newSite, game, play);

            int playerMoney = currentplayer->playerMoney;
            int playerV1 = currentplayer->playerV1;
            int playerV2 = currentplayer->playerV2;
            int playerPoints = currentplayer->playerPoints;
            int cardA = currentplayer->cardA;
            int cardB = currentplayer->cardB;
            int cardC = currentplayer->cardC;
            int cardD = currentplayer->cardD;
            int cardE = currentplayer->cardE;

            if(valid_move(newSite, game) == 0) {
                fprintf(stderr, "Player %d Money=%d V1=%d V2=%d Points=%d A=%d B=%d C=%d D=%d E=%d\n", /////// DONT KNOW HOW TO FIX
                        player, playerMoney, playerV1,
                        playerV2, playerPoints, cardA, 
                        cardB, cardC, cardD, 
                        cardE);
                move(player, newSite, game, play);
            }
        } else {
            fprintf(stderr, "Communications error\n");
            exit(6);
        }
    }
}

// calculate the points from the players cards
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

// function for scoring
void scoring(struct GameStructure* game, struct PlayerStructure* play) {
    fprintf(stderr, "Scores:");
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
        fprintf(stderr, " ");
    }
    for(int i = 0; i < game->playerCount; i++) {
        if(i > 0) {
            fprintf(stderr, ",");
        }
        struct PlayerStructure* currentplayer = &play[i];
        fprintf(stderr, "%d", currentplayer->playerPoints);   
    }
    fprintf(stderr, "\n");
    exit(0);
}


// Check the dealer input 
void check_response(char* response, struct GameStructure* game, 
        struct PlayerStructure* play) {
    if (strcmp(response, "EARLY") == 0) {
        fprintf(stderr, "Early game over");
        exit(5);
    } else if(strcmp(response, "DONE") == 0) {
        scoring(game, play);
    } else if(response[0] == 'H' && response[1] == 'A' && response[2] == 'P') {
        hap_message(response, game, play);
    } else {
        fprintf(stderr, "Communications error\n");
        exit(6);
    }
}

// initialises the player and the board
void shared_player_functionality(int argc, char** argv, 
        struct GameStructure* game, struct PlayerStructure* play) {

    int numPlayers = strtol(argv[1], &p, 10);        
    game->playerCount = numPlayers;

    initialise_player(game, play);
    error_checking(argc, argv, game, play);

    // printing ^ lets the dealer know that the player is initialised
    //fprintf(stderr, "before carrot\n");
    printf("^");
    fflush(stdout);
    //fprintf(stderr, "after carrot\n");
    // prompt the dealer to provide a path
    char* path = (char*)malloc(sizeof(char) * 100);
    path = read_line(stdin);
    // save a copy of the path
    // char* pathCopy = (char*)malloc(sizeof(char) * 100);
    // pathCopy = path;
    // check the path for erorr (still need to finish)
    //fprintf(stderr, ">>%s<<\n", path);
    invalid_path(path);
    // print the sites (above the board)
    print_sites(path, game);
    // print the board
    initialise_board(game->playerCount, game->numberOfSites, game, play);
    free(path);  
}
