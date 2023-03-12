#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "players.h"

// for use with strtol
char* p;

// Player A Strategy
void strategy_a(struct GameStructure* game, struct PlayerStructure* play) {
    struct PlayerStructure* currentplayer = &play[game->currentPlayer];
    bool money = false;
    bool move = false;
    if(currentplayer->playerMoney > 0) {
        money = true;
    }
    if(money && !move) {
        for(int i = currentplayer->playerAtSite + 1; 
                i < game->numberOfSites; i++) {
            if(game->siteList[i][0] == 'D' && game->siteList[i][1] == 'o' &&
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
            if(game->siteList[i][0] == 'M' && 
                    game->siteList[i][1] == 'o' && valid_move(i, game) == 0) {
                printf("DO%d\n", i);
                fflush(stdout);
                move = true;
                break;
            } else {
                break;
            }
        }
    }
    if(!move) {
        for(int i = currentplayer->playerAtSite + 1; 
                i < game->numberOfSites; i++) {
            if(((game->siteList[i][0] == 'V' && 
                    game->siteList[i][1] == '1')
                    || (game->siteList[i][0] == 'V' && 
                    game->siteList[i][1] == '2')
                    || (game->siteList[i][0] == ':' && 
                    game->siteList[i][1] == ':')) && 
                    valid_move(i, game) == 0) {
                printf("DO%d\n", i);
                fflush(stdout);
                // flush
                move = true;
                break;
            }
        }
    }
}

// save the current player to the struct
void save_current_player(int player, struct GameStructure* game) {
    game->currentPlayer = player;
}

// check number of args
void incorrect_num_args(int argc) {
    if (argc != 3) {
        fprintf(stderr, "Usage: player pcount ID\n");
        exit(1);
    }
}

// main function
int main(int argc, char** argv) {
    struct GameStructure game;
    incorrect_num_args(argc);
    
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
        if (strcmp(response, "YT") == 0) {
            strategy_a(&game, &play[0]);
        } else {
            check_response(response, &game, &play[0]);
        }
        // else if its a different message than refer to players.c
    }
    free(response);
    return 0;
    
}