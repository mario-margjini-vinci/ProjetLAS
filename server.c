#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

#include "structures.h"
#include "utils_v1.h"
#include "ipc.h"
#include "game.h"
#include "network.h"

#define MAX_WAITING 100
#define MAX_PLAYERS 10
#define TIME_INSCRIPTION 30

Player tabPlayers[MAX_PLAYERS];
volatile sig_atomic_t end_inscriptions = 0;

void endServerHandler(int sig)
{
	end_inscriptions = 1;
}

int main(int argc, char **argv){
    if (argc < 2){
        printf("Pour utiliser l'appli, veuillez rentrer en paramètre le numéro de port");
        exit(1);
    }
    int sockfd, newsockfd, i;
    Message msg;
    int ret;
    //struct pollfd fds[MAX_PLAYERS];
    int serverPort = atoi(argv[1]);

    ssigaction(SIGALRM, endServerHandler);

    sockfd = initSocketServer(serverPort);
    printf("Le serveur tourne sur le port : %i \n", serverPort);

    i= 0;
    int nbPlayers = 0;

    alarm(TIME_INSCRIPTION);

    while (!end_inscriptions)
    {
        newsockfd = accept(sockfd,NULL,NULL);
        if(newsockfd > 0)
        {
            ret = sread(newsockfd, &msg, sizeof(msg));
            if (msg.code == INSCRIPTION_REQUEST)
            {
                printf("Inscription demandée par le joueur : %s\n", msg.messageText);

                strcpy(tabPlayers[i].pseudo, msg.messageText);
                tabPlayers[i].sockfd = newsockfd;
                i++;

                if (nbPlayers < MAX_PLAYERS)
				{
					msg.code = INSCRIPTION_OK;
					nbPlayers++;
					if (nbPlayers == MAX_PLAYERS)
					{
						// cancel alarm
						end_inscriptions = 1;
					}
				}
                else{
                    msg.code = INSCRIPTION_KO;
                }
                ret = swrite(newsockfd, &msg, sizeof(msg));
				printf("Nb Inscriptions : %i\n", nbPlayers);
                printf("%i", ret);

            }
        }
    }

    printf("FIN DES INSCRIPTIONS\n");


}