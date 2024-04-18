#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#define MAX_PSEUDO 256

typedef struct Player
{
    char pseudo[MAX_PSEUDO];
    int sockfd;
    int score;
} Player;

#endif