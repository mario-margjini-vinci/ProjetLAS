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
#define TILE_NUMBER 20
#define SHM_KEY 1234
#define SEM_KEY 619
#define PERM 0666


Player tabPlayers[MAX_PLAYERS];
pid_t tabPids[MAX_PLAYERS];
int tabTiles[TILE_NUMBER];
volatile sig_atomic_t end_inscriptions = 0;

void endServerHandler(int sig)
{
	end_inscriptions = 1;
}

void childHandler(void *arg1){
    Player *player = arg1;
    sclose(player->pipefdPC[1]);
    sclose(player->pipefdCP[0]);
    int endGame = 0;
    Message msgChild;
    while(!endGame){
        //Lecture du pipe
        sread(player->pipefdPC[0], &msgChild, sizeof(msgChild));
        if(msg.code == TILE){
            swrite(player->sockfd, &msgChild, sizeof(msgChild));
            sread(player->sockfd, &msgChild, sizeof(msgChild));
            swrite(player->pipefdCP[1], &msgChild, sizeof(msgChild));    
        }else if(msg.code == END_GAME){
            endGame = 1;
        }       
    }



}

int main(int argc, char **argv){
    if (argc < 2){
        printf("Pour utiliser l'appli, veuillez rentrer en paramètre le numéro de port");
        exit(1);
    }
    int sockfd, newsockfd, i;
    Message msg;
    int ret;
    struct pollfd fds[MAX_PLAYERS];
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
	if (nbPlayers <= 1)
	{
		printf("PARTIE ANNULEE .. PAS ASSEZ DE JOUEURS\n");
		msg.code = CANCEL_GAME;
		for (i = 0; i < nbPlayers; i++)
		{
			swrite(tabPlayers[i].sockfd, &msg, sizeof(msg));
		}
		disconnect_players(tabPlayers, nbPlayers);
		sclose(sockfd);
		exit(0);
	}
	else
	{
		printf("PARTIE VA DEMARRER ... \n");
		msg.code = START_GAME;
		for (i = 0; i < nbPlayers; i++)
			swrite(tabPlayers[i].sockfd, &msg, sizeof(msg));
	}

    for(int i = 0; i < nbPlayers; i++){
        fds[i].fd = tabPlayers[i].sockfd;
		fds[i].events = POLLIN;

        spipe(tabPlayers[i].pipefdPC);
        spipe(tabPlayers[i].pipefdCP);

        tabPids[i] = fork_and_run1(childHandler,tabPlayers[i]);

        sclose(tabPlayers[i].pipefdPC[0]);
        sclose(tabPlayers[i].pipefdCP[1]);
    }
    
    tabTiles = initTiles(TILE_NUMBER);
    for(int i = 0; i < NB_TILES; i++){
        int nbResponse = 0;
        for(int j = 0; j < nbPlayers; j++){
            msg.messageInt = tabTiles[i];
            msg.code = TILE;
            swrite(tabPlayers[j].pipefdPC[1], &msg, sizeof(tabTiles[i]));
        }
        while(nbResponse<NB_PLAYERS){
            ret = spoll(fds, nbPlayers, 1000);
            nbResponse += ret;
        }
    }
    for(int i = 0; i < nbPlayers; i++){
        msg.code = END_GAME;
        swrite(tabPlayers[i].pipefdPC[1], &msg, sizeof(msg));
        sread(tabPlayers[i].pipefdCP[0], &msg, sizeof(msg));
        tabPlayers[i].score = msg.messageInt;
        
    }

    int shm_id = createPlayers(SHM_KEY, sizeof(Player) * MAX_PLAYERS, IPC_CREAT | IPC_EXCL | PERM, tabPlayers, nbPlayers);
    int sem_id = initSemaphore(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | PERM, 0);

    Player* shm_players = readPlayers(shm_id);

    for


}