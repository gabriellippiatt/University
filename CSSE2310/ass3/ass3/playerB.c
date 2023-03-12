#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "players.h"

// for use with strtol
char* p;

// return 0 if true
// return 1 if false
int check_cards(struct GameStructure* game, struct PlayerStructure* play) {

    int player = 0;
    int totalcards = 0;
    
    for(int i = 0; i < game->playerCount - 1; i++) {
        int currentcards = 0;
        int nextcards = 0;

        struct PlayerStructure* currentplayer = &play[i];
        struct PlayerStructure* nextplayer = &play[i + 1];

        currentcards = currentplayer->cardA + currentplayer->cardB + 
                currentplayer->cardC + currentplayer->cardD + 
                currentplayer->cardE;

        nextcards = nextplayer->cardA + nextplayer->cardB + nextplayer->cardC +
                nextplayer->cardD + nextplayer->cardE;

        totalcards = totalcards + currentcards + nextcards;

        if(currentcards >= nextcards) {
            player = currentplayer->playerID;
        } else if(currentcards <= nextcards) {
            player = nextplayer->playerID;
        }
    }

    if(totalcards == 0 || player == game->currentPlayer) {
        return 0;
    } else {
        return 1;
    }

}

// Player B Strategy
void strategy_b(struct GameStructure* game, struct PlayerStructure* play) {   //////////////  FUNCTION TOO LONG
    struct PlayerStructure* currentplayer = &play[game->currentPlayer];
    bool move = false;
    int nextSite = currentplayer->playerAtSite + 1;
    int playerCount = game->playerCount;
    int counter = 0;
    if(valid_move(nextSite, game) == 0) {
        for (int i = 0; i < playerCount; i++) {
            for (int j = 0; j < nextSite; j++) {
                if(game->boardPositions[i][j] != ' ') {
                    counter++;
                }
            }
        }
        if(counter == 1) {
            printf("DO%d\n", nextSite);
            fflush(stdout);
            move = true;
        }
    }
    if(!move && ((currentplayer->playerMoney % 2) == 1)) {
        for(int i = currentplayer->playerAtSite + 1; 
                i < game->numberOfSites; i++) {
            if(game->siteList[i][0] == 'M' && game->siteList[i][1] == 'o' && 
                    valid_move(i, game) == 0) {
                printf("DO%d\n", i);
                fflush(stdout);
                move = true;
                break;
            } else if (game->siteList[i][0] == ':' && 
                    game->siteList[i][1] == ':') {
                break;
            }
        }
    }
    if(!move) {
        if(check_cards(game, play) == 0) {
            for(int i = currentplayer->playerAtSite + 1;
                    i < game->numberOfSites; i++) {
                if(game->siteList[i][0] == 'R' && game->siteList[i][1] == 'i' 
                        && valid_move(i, game) == 0) {
                    printf("DO%d\n", i);
                    fflush(stdout);
                    move = true;
                    break;
                } else if (game->siteList[i][0] == ':' && 
                        game->siteList[i][1] == ':') {
                    break;
                }
            }
        }
    }
    if(!move) {
        for(int i = currentplayer->playerAtSite + 1; 
                i < game->numberOfSites; i++) {
            if(game->siteList[i][0] == 'V' && game->siteList[i][1] == '2' && 
                    valid_move(i, game) == 0) {
                printf("DO%d\n", i);
                fflush(stdout);
                move = true;
                break;
            } else if (game->siteList[i][0] == ':' && 
                    game->siteList[i][1] == ':') {
                break;
            }
        }
    }
    if(!move) {
        for(int i = currentplayer->playerAtSite + 1;
                i < game->numberOfSites; i++) {
            if(valid_move(i, game) == 0) {
                printf("DO%d\n", i);
                fflush(stdout);
                move = true;
                break;
            }
        }
    }
}

// save current player to the struct
void save_current_player(int player, struct GameStructure* game) {
    game->currentPlayer = player;
}

// check for correct arguments
void incorrect_num_args(int argc) {
    if (argc != 3) {
        fprintf(stderr, "Usage: player pcount ID\n");
        exit(1);
    }
}

// check for correct number of players
void invalid_player_count(char* number, struct GameStructure* game) {
    if (isdigit(number[0])) {
        game->playerCount = strtol(number, &p, 10);
        if (game->playerCount < 1) {
            fprintf(stderr, "Invalid player count\n");
            exit(2);
        }
    } else {
        fprintf(stderr, "Invalid player count\n");
        exit(2);
    }
}

// main function
int main(int argc, char** argv) {
    struct GameStructure game;
    incorrect_num_args(argc);
    invalid_player_count(argv[1], &game);
    
    int numPlayers = strtol(argv[1], &p, 10);

    struct PlayerStructure play[numPlayers];

    save_current_player(strtol(argv[2], &p, 10), &game);
    // runs the shared functionality
    shared_player_functionality(argc, argv, &game, &play[0]);
    // after the board is printed promt the dealer for a response
    char* response = (char*)malloc(sizeof(char) * 100);
    while(1 == 1) {
        response = read_line(stdin);
        eof_check(stdin);
        // could probably move this out of main
        if (strcmp(response, "YT") == 0) {
            strategy_b(&game, &play[0]);
        } else {
            check_response(response, &game, &play[0]);
        }
        // else if its a different message than refer to players.c
    }
    free(response);
    return 0;
    
}