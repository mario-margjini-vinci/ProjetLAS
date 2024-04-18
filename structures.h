#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#define MAX_PSEUDO 256
#define MAX_TEXT 256

typedef struct Player
{
    char pseudo[MAX_PSEUDO];
    int sockfd;
    int score;
} Player;

typedef struct Message
{
    char messageText[MAX_TEXT];
    int messageInt;
    int code;
    bool tilePlaced;
}

#endif