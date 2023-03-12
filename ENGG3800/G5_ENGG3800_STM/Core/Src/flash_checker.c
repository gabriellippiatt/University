#include "flash_checker.h"

void flash_checker(int check)
{
    flash_read(0x0803F800, 3);
    continuity = atoi(flashBuffer);
    if (strcmp(check, continuity) != 0) { // only flash if the given value differes from the value stored in memory
        continuity = check;
        sprintf(sendBuffer, "%d", check);
        flash_write(0x0803F800, sendBuffer, 3);
    }
}
