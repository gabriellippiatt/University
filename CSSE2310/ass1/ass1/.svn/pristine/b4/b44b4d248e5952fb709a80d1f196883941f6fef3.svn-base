#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// create a struct for the board
struct GameStructure {
    char** board;
};

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

// create, read, and prints the board from a save file
void print_board(int rows, int columns, FILE* fp, 
        struct GameStructure* pointer) {
    pointer->board = (char**)malloc(sizeof(char*) * (rows + 1));
    for (int i = 0; i < rows; i++) {
        pointer->board[i] = 
                (char*)malloc(sizeof(char) * ((columns * 2) + 10));
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= (columns * 2); j++) {
            pointer->board[i][j] = fgetc(fp);
            if (i == rows - 1 && j == columns * 2) {
                break;
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= (columns * 2); j++) {
            printf("%c", pointer->board[i][j]);
        }
    }
    fclose(fp);
}

// add up each players score
int scoring(char nextplayer, int rows, int columns, 
        struct GameStructure* pointer) {
    int playerScore = 0;
    for (int i = 1; i < (rows - 1); i++) {
        for (int j = 2; j < (columns * 2) - 2; j++) {
            if (pointer->board[i][j] == nextplayer) {
                playerScore += ((pointer->board[i][j - 1]) - '0');
            }
        }
    }
    return playerScore;
}

// check if the board is full
int check_board_array(int rows, int columns, struct GameStructure* pointer) {
    int counter = 0;
    for (int i = 1; i < (rows - 1); i++) {
        for (int j = 2; j < (columns * 2) - 2; j++) {
            if (pointer->board[i][j] == '.') {
                counter++;
            }
        }
    }
    if (counter != 0) {
        return 0; // return 0 if board is not full
    } else {
        return 1; // return 1 if board is full
    }
}

// save the game
int save_game(const char* path, char nextplayer, 
        int rows, int columns, char** board) {
    FILE* fp;
    fp = fopen(path, "w");

    fprintf(fp, "%d %d\n", rows, columns);
    fprintf(fp, "%c\n", nextplayer);

    // this prints the board to the given file
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= (columns * 2); j++) {
            fprintf(fp, "%c", board[i][j]);
        }
    }
    fclose(fp);

    return 0;
}

// if upper edged then move stones down (if valid) 
int upper_edge(int row, int column, char nextplayer, int rows, 
        int columns, struct GameStructure* pointer) {
    if (pointer->board[(row + 1)][column] != '.') {
        for (int i = 2; i < rows; i++) {
            if (pointer->board[i][column] == '.') {
                for (int j = i; j > 0; j--) {
                    pointer->board[j][column] = pointer->board[j - 1][column];
                }
                pointer->board[row + 1][column] = nextplayer;
                return 0; // valid push
                break;
            } 
        }
        return 2; // no more space
    } else {
        return 1; // invalid push
    }
}

// if lower edged then move stones up (if valid)
int lower_edge(int row, int column, char nextplayer, int rows, 
        int columns, struct GameStructure* pointer) {
    if (pointer->board[(row - 1)][column] != '.') {
        for (int i = (rows - 3); i >= 0; i--) {
            if (pointer->board[i][column] == '.') {
                for (int j = i; j < (rows - 1); j++) {
                    pointer->board[j][column] = pointer->board[j + 1][column];
                }
                pointer->board[row - 1][column] = nextplayer;
                return 0; // valid push
                break;
            } 
        }
        return 2; // no more space
    } else {
        return 1; // invalid push
    }
}

// if left edged then move stones right (if valid)
int left_edge(int row, int column, char nextplayer, int rows, 
        int columns, struct GameStructure* pointer) {
    if (pointer->board[row][(column + 3)] != '.') {
        for (int i = 5; i < ((columns * 2)); i++) {
            if ((i % 2) == 1) {
                if (pointer->board[row][i] == '.') {
                    for (int j = i; j > 1; j--) {
                        if ((j % 2) == 1) {
                            pointer->board[row][j] = 
                                    pointer->board[row][j - 2];
                        }
                    }
                    pointer->board[row][(column + 2)] = nextplayer;
                    return 0; // valid push
                    break;
                }
            }
        }
        return 2; // no more space
    } else {
        return 1; // invalid push
    }
}

// if right edged then move stones left (if valid)
int right_edge(int row, int column, char nextplayer, int rows, 
        int columns, struct GameStructure* pointer) {
    if (pointer->board[row][((columns * 2) - 3)] != '.') {
        for (int i = ((columns * 2) - 5); i > 0; i--) {
            if ((i % 2) == 1) {
                if (pointer->board[row][i] == '.') {
                    for (int j = i; j < ((columns * 2) - 1); j++) {
                        if ((j % 2) == 1) {
                            pointer->board[row][j] = 
                                    pointer->board[row][j + 2];
                        }
                    }
                    pointer->board[row][((columns * 2) - 3)] = nextplayer;
                    return 0; // valid push
                    break;
                }
            }
        }
        return 2; // no more space
    } else {
        return 1; // invalid push
    }
}

// check if given coordinate is on edge of the board
int on_edge(int row, int column, char nextplayer, int rows, 
        int columns, struct GameStructure* pointer) {
    if (row == 0 && 0 <= column && column <= (columns - 1)) {
        if (upper_edge(row, column, nextplayer, rows, columns, pointer) == 0) {
            return 0; // successfull push
        } else if (upper_edge(row, column, nextplayer, 
                rows, columns, pointer) == 2) {
            return 2; // column is full
        } else {
            return 1; // invalid push
        }
    } else if (row == (rows - 1) && 0 < column && column < (columns - 1)) {
        if (lower_edge(row, column, nextplayer, rows, columns, pointer) == 0) {
            return 0; // successfull push
        } else if (lower_edge(row, column, nextplayer, 
                rows, columns, pointer) == 2) {
            return 2; // column is full
        } else {
            return 1; // invalid push
        }
    } else if (column == 1 && 0 < row && row < (rows - 1)) {
        if (left_edge(row, column, nextplayer, rows, columns, pointer) == 0) {
            return 0; // successfull push
        } else if (left_edge(row, column, nextplayer, 
                rows, columns, pointer) == 2) {
            return 2; // column is full
        } else {
            return 1; // invalid push
        }
    } else if (column == ((columns * 2) - 1) && 0 < row && row < (rows - 1)) {
        if (right_edge(row, column, nextplayer, rows, columns, pointer) == 0) {
            return 0; // successfull push
        } else if (right_edge(row, column, nextplayer, 
                rows, columns, pointer) == 2) {
            return 2; // column is full
        } else {
            return 1; // invalid push
        }
    } else {
        return 3; // coordinate is not on the edge
    }
}

// check for EOF in stdin
void eof_check(FILE* fp) {
    if (feof(fp)) {
        fprintf(stderr, "End of file\n");
        exit(5);
    }
}

// check the human input and make a move or save game
void human_player_move(char nextplayer, int rows, int columns, 
        struct GameStructure* pointer) {
    bool validMove = false;
    char* humanMove = (char*)malloc(sizeof(char) * 100);
    char* saveFile = (char*)malloc(sizeof(char) * 100);
    int row, column;

    // check whether the move is valid and re-prompt user if not
    while (!validMove) {
        printf("%c:(R C)> ", nextplayer);
        humanMove = saveFile = read_line(stdin);

        if (strlen(humanMove) != 3) {
            eof_check(stdin);
            if (humanMove[0] == 's' && strlen(humanMove) == 1) {
            } else if (humanMove[0] == 's' && humanMove[1] != '/') {
                for (int i = 0; i < (strlen(humanMove)); i++) {
                    saveFile[i] = saveFile[i + 1];
                }
                save_game(saveFile, nextplayer, rows, columns,
                        (pointer->board));
            } else if (humanMove[1] == '/') {
                fprintf(stderr, "Save failed\n");
            }
            // user input is a valid coordinate
        } else if ((humanMove[0] - '0') % 1 == 0 && 
                (humanMove[2] - '0') % 1 == 0) {
            row = humanMove[0] - '0';
            column = ((humanMove[2] - '0') * 2) + 1;
            // valid location to place a marker
            if (pointer->board[row][column] == '.') {
                if (on_edge(row, column, nextplayer, 
                        rows, columns, pointer) == 0) {
                    validMove = true;
                } else if (on_edge(row, column, nextplayer, rows, columns, 
                        pointer) == 2 || on_edge(row, column, nextplayer, 
                        rows, columns, pointer) == 1) {
                    break;
                } else {
                    validMove = true;
                    pointer->board[row][column] = nextplayer;
                }
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j <= (columns * 2); j++) {
                        printf("%c", pointer->board[i][j]);
                    }
                }
            }
        }
    }
    free(humanMove);
}

// Type 0 Automated Player for Player O
void zero_player_move_o(char nextplayer, int rows, int columns, 
        struct GameStructure* pointer) {
    int placedRow;
    int placedColumn;
    bool placed = false;

    while (!placed) {
        for (int i = 1; i < (rows - 1); i++) {
            for (int j = 2; j < (columns * 2) - 2; j++) {
                if (pointer->board[i][j] == '.') {
                    pointer->board[i][j] = nextplayer;
                    placedRow = i;
                    placedColumn = j;
                    placed = true; 
                    break;  
                }
            }
            if (placed) {
                break;
            }
        } 
    }
    int columnCoord = (placedColumn - 1) / 2;
    printf("Player %c placed at %d %d\n", nextplayer, placedRow, columnCoord);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= (columns * 2); j++) {
            printf("%c", pointer->board[i][j]);
        }
    }
}

// Type 0 Automated Player for Player X
void zero_player_move_x(char nextplayer, int rows, int columns, 
        struct GameStructure* pointer) {
    int placedRow;
    int placedColumn;
    bool placed = false;

    while (!placed) {
        for (int i = (rows - 2); i > 0; i--) {
            for (int j = (columns * 2) - 3; j > 1; j--) {
                if (pointer->board[i][j] == '.') {
                    pointer->board[i][j] = nextplayer;
                    placedRow = i;
                    placedColumn = j;
                    placed = true; 
                    break;  
                }
            }
            if (placed) {
                break;
            }
        } 
    }
    int columnCoord = (placedColumn - 1) / 2;
    printf("Player %c placed at %d %d\n", nextplayer, placedRow, columnCoord);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= (columns * 2); j++) {
            printf("%c", pointer->board[i][j]);
        }
    }
}

// type 1 automated player (not implemented)
void one_player_move() {
    printf("\nautomatic player '1' not implemented\n\n");
}

// check that the user iputs the right number of arguments
void incorrect_number_args(int argc) {
    if (argc != 4) {
        fprintf(stderr, "Usage: push2310 typeO typeX fname\n");
        exit(1);
    }
}

// check to see if the user has input valid player types
void bad_player_type(char** argv) {
    if ((strcmp(argv[1], "H") != 0 && strcmp(argv[1], "1") != 0 && 
            strcmp(argv[1], "0") != 0) || ((strcmp(argv[2], "H") != 0 && 
            strcmp(argv[2], "1") != 0 && strcmp(argv[2], "0") != 0))) {

        fprintf(stderr, "Invalid player type\n");
        exit(2);
    }
}

// check that the user input is a valid file to load from
void bad_file(char** argv) {
    FILE* fp;
    char* filename = argv[3];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "No file to load from\n");
        exit(3);
    }
    fclose(fp);
}

// check the given dimentions match the actual dimentions of the board
void invalid_dimentions(char** argv) {
    FILE* fp;
    char** saveBoard;
    char* filename = argv[3];
    fp = fopen(filename, "r");
    char* rowChars = (char*)malloc(sizeof(char) * 100);
    char* columnChars = (char*)malloc(sizeof(char) * 100);

    int i = 0, j = 0;
    for(i = 0; i < 1000; i++) {
        rowChars[i] = fgetc(fp);
        if(rowChars[i] == ' ') {
            break;
        }
    }
    rowChars[i] = '\0';
    for(j = 0; j < 1000; j++) {
        columnChars[j] = fgetc(fp);
        if(columnChars[j] == '\n') {
            break;
        }
    }
    columnChars[j] = '\0';
    int rows = atoi(rowChars);
    int columns = atoi(columnChars);
    char nextplayer = (char)fgetc(fp);
    int maxCharacters = columns * 2;
    char str[maxCharacters];
    fgets(str, maxCharacters, fp);
    saveBoard = (char**)malloc(sizeof(char*) * (rows + 1));
    for (int i = 0; i < rows; i++) {
        saveBoard[i] = (char*)malloc(sizeof(char) * ((columns * 2) + 1));
    }
    int counter = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= (columns * 2); j++) {
            saveBoard[i][j] = fgetc(fp);
            if (saveBoard[i][j] == '.' || 
                    saveBoard[i][j] == 'X' || saveBoard[i][j] == 'O') {
                counter++; 
            }
        }
    }
    if (counter != ((rows * columns) - 4) || (nextplayer != 'X' 
            && nextplayer != 'O')) {
        fprintf(stderr, "Invalid file contents\n");
        exit(4);
    }
    fclose(fp);
}

// count how many free spots there are in an array
int counter(FILE* fp, int rows, int columns, char** saveBoard) {
    int counter = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= (columns * 2); j++) {
            if (i == rows - 1 && j == columns * 2) {
                break;
            }
            saveBoard[i][j] = fgetc(fp);
            if (0 < i && i < (rows - 1) && 1 < j &&
                    j < ((columns * 2) - 2)) {
                if (saveBoard[i][j] == '.') {
                    counter++;
                }
            }
        }
    }
    return counter;
}

// check and see if the save board is full
void check_board_save(char** argv) {
    FILE* fp = fopen(argv[3], "r");
    char** saveBoard;
    char* rowChars = (char*)malloc(sizeof(char) * 100);
    char* columnChars = (char*)malloc(sizeof(char) * 100);
    int i = 0, j = 0;
    for(i = 0; i < 1000; i++) {
        rowChars[i] = fgetc(fp);
        if(rowChars[i] == ' ') {
            break;
        }
    }
    rowChars[i] = '\0';

    for(j = 0; j < 1000; j++) {
        columnChars[j] = fgetc(fp);
        if(columnChars[j] == '\n') {
            break;
        }
    }
    columnChars[j] = '\0';

    int rows = atoi(rowChars);
    int columns = atoi(columnChars);
    int maxCharacters = columns * 2;
    char str[maxCharacters];
    fgets(str, maxCharacters, fp);

    saveBoard = (char**)malloc(sizeof(char*) * (rows + 1));
    for (int i = 0; i < rows; i++) {
        saveBoard[i] = (char*)malloc(sizeof(char) * ((columns * 2) + 1));
    }

    int spots = counter(fp, rows, columns, saveBoard);
    if (spots == 0) {
        fprintf(stderr, "Full board in load\n");
        exit(6);
    }
    fclose(fp);
}

// check all the errors
void error_checking(int argc, char** argv) {
    incorrect_number_args(argc);
    bad_player_type(argv);
    bad_file(argv);
    invalid_dimentions(argv);
    check_board_save(argv);
}

// check player type and make a move
void making_a_move(char* playerOType, char* playerXType, char nextplayer, 
        int rows, int columns, struct GameStructure* pointer) {
    bool boardFull = false;
    int counter;
    if (nextplayer == 'X') {
        counter = 0;
    } else if (nextplayer == 'O') {
        counter = 1;
    }

    while(!boardFull) {
        if (counter % 2 == 0) {
            if (check_board_array(rows, columns, pointer) == 1) {
                boardFull = true;
            } else if (playerXType[0] == 'H') {
                human_player_move(nextplayer, rows, columns, pointer);
                nextplayer = 'O';
            } else if (playerXType[0] == '0') {
                zero_player_move_x(nextplayer, rows, columns, pointer);
                nextplayer = 'O';
            } else {
                one_player_move();
                break;
            }
        } else if (counter % 2 == 1) {
            if (check_board_array(rows, columns, pointer) == 1) {
                boardFull = true;
            } else if (playerOType[0] == 'H') {
                human_player_move(nextplayer, rows, columns, pointer);
                nextplayer = 'X';
            } else if (playerOType[0] == '0') {
                zero_player_move_o(nextplayer, rows, columns, pointer);
                nextplayer = 'X';
            } else {
                one_player_move();
                break;
            }
        } 
        counter++;
    }
}

// get the score of both players and find the winner
void decide_winner(int rows, int columns, struct GameStructure* pointer) {
    int playerXScore = scoring('X', rows, columns, pointer);
    int playerOScore = scoring('O', rows, columns, pointer);
    char winner;
    if (playerXScore > playerOScore) {
        winner = 'X';
        printf("Winners: %c\n", winner);
    } else if (playerXScore < playerOScore) {
        winner = 'O';
        printf("Winners: %c\n", winner);
    } else {
        printf("Winners: O X\n");
    }
}

int main(int argc, char** argv) {
    FILE* fp;
    error_checking(argc, argv);
    struct GameStructure play;

    char* filename = argv[3];
    char* playerOType = argv[1];
    char* playerXType = argv[2];
    
    fp = fopen(filename, "r");

    char* rowChars = (char*)malloc(sizeof(char) * 100);
    char* columnChars = (char*)malloc(sizeof(char) * 100);

    int i = 0, j = 0;
    for(i = 0; i < 1000; i++) {
        rowChars[i] = fgetc(fp);
        if(rowChars[i] == ' ') {
            break;
        }
    }
    rowChars[i] = '\0';

    for(j = 0; j < 1000; j++) {
        columnChars[j] = fgetc(fp);
        if(columnChars[j] == '\n') {
            break;
        }
    }
    columnChars[j] = '\0';

    int rows = atoi(rowChars);
    int columns = atoi(columnChars);
    int maxCharacters = columns * 2;
    char str[maxCharacters];
    char nextplayer = (char)fgetc(fp);
    fgets(str, maxCharacters, fp);

    print_board(rows, columns, fp, &play);
    making_a_move(playerOType, playerXType, nextplayer, rows, columns, &play);
    decide_winner(rows, columns, &play);

    return 0;
    fclose(fp);
}