#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include <stdbool.h>

#define MAX_PSEUDO 256
#define MAX_TEXT 256

#define SERVER_IP "127.0.0.1" /* localhost */

typedef enum
{
  INSCRIPTION_REQUEST = 10,
  INSCRIPTION_OK = 11,
  INSCRIPTION_KO = 12,
  CANCEL_GAME = 13,
  START_GAME = 14,
  TILE = 15,
  END_GAME = 16,
  RANKING = 17,
  TILE_PLACED = 18,
} Code;

typedef struct Player
{
    char pseudo[MAX_PSEUDO];
    int sockfd;
    int score;
    int pipefdPC[2];
    int pipefdCP[2];
} Player;

typedef struct Message
{
    char messageText[MAX_TEXT];
    int messageInt;
    int code;
    bool tilePlaced;
    Player *players;
} Message;

#endif
