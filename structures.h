#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#define MAX_PSEUDO 256
#define MAX_TEXT 256

#define SERVER_IP "127.0.0.1" /* localhost */

typedef enum
{
  INSCRIPTION_REQUEST = 10,
  INSCRIPTION_OK = 11,
  INSCRIPTION_KO = 12
} Code;

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
} Message;

#endif
