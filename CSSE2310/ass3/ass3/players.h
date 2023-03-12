struct GameStructure {
    char** siteList;
    char** boardPositions;
    char* sites;
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

char* read_line(FILE* file);
void eof_check(FILE* fp);
int valid_move(int column, struct GameStructure* pointer);
void check_response(char* response, struct GameStructure* pointer, struct PlayerStructure* play);
void shared_player_functionality(int argc, char** argv, struct GameStructure* pointer, struct PlayerStructure* play);
int get_numberOfSites(struct GameStructure* pointer);
char** get_siteList(struct GameStructure* pointer);