#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "structures.h"
#include "utils_v1.h"
#include "ipc.h"
#include "game.h"
#include "network.h"

#define MAX_WAITING 100
#define MAX_PLAYERS 10
#define BACKLOG 5
#define TIME_INSCRIPTION 30

Player tabPlayers[MAX_PLAYERS];
Player tabWaitingPlayers[MAX_WAITING];
volatile sig_atomic_t end_inscriptions = 0;

void endServerHandler(int sig)
{
	end_inscriptions = 1;
}

int main(int argc, char **argv){
    int sockfd, newsockfd, i;
    Message msg;
    int ret;
    struct pollfd fds[MAX_PLAYERS];

    ssigaction(SIGALRM, endServerHandler);

    sockfd = initSocketServer(argv[1]);
    printf("Le serveur tourne sur le port : %i \n", argv[1]);

    i= 0;
    int nbPlayers = 0;

    alarm(TIME_INSCRIPTION);

    while (!end_inscriptions)
    {
        newsockfd = accept(sockfd);
        if(newsockfd > 0)
        {
            ret = sread(newsockfd, &msg, sizeof(msg));
            if (msg.code == INSCRIif (nb)PTION_REQUEST)
            {
                printf("Inscription demandée par le joueur : %s\n", msg.messageText);

                strcpy(tabPlayers[i].pseudo, msg.messageText);
                tabPlayers[i].sockfd = newsockfd;
                i++;

                if (nbPLayers < MAX_PLAYERS)
				{
					msg.code = INSCRIPTION_OK;
					nbPLayers++;
					if (nbPLayers == MAX_PLAYERS)
					{
						alarm(0); // cancel alarm
						end_inscriptions = 1;
					}
				}
                else{
                    msg.code = INSCRIPTION_KO;

                }
if ()


