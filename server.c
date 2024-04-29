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
#define NB_TILES 2
#define TIME_INSCRIPTION 30
#define TILE_NUMBER 20
#define SHM_KEY 1234
#define SEM_KEY 619
#define PERM 0666
#define MAX_PSEUDO 256
#define MAX_BUFFER 1000

Player tabPlayers[MAX_PLAYERS];
pid_t tabPids[MAX_PLAYERS];
volatile sig_atomic_t end_inscriptions = 0;

void endServerHandler(int sig)
{
    end_inscriptions = 1;
}

void childHandler(void *arg1, void *arg2)
{
    Player *player = arg1;
    int *nbPlayers = arg2;
    sclose(player->pipefdPC[1]);
    sclose(player->pipefdCP[0]);
    int endGame = 0;
    Message msgChild;
    while (!endGame)
    {
        // Lecture du pipe
        sread(player->pipefdPC[0], &msgChild, sizeof(msgChild));
        if (msgChild.code == TILE)
        {
            // envoie de la tuile
            swrite(player->sockfd, &msgChild, sizeof(msgChild));
            sread(player->sockfd, &msgChild, sizeof(msgChild));
            swrite(player->pipefdCP[1], &msgChild, sizeof(msgChild));
        }
        else if (msgChild.code == END_GAME)
        {
            endGame = 1;
            // envoie du code de fin de partie
            swrite(player->sockfd, &msgChild, sizeof(msgChild));
        }
    }
    // lecture du score
    sread(player->sockfd, &msgChild, sizeof(msgChild));
    // envoie du score
    swrite(player->pipefdCP[1], &msgChild, sizeof(msgChild));
    int sem_id = sem_get(SEM_KEY, 1);
    sem_down0(sem_id);

    int shm_id = sshmget(SHM_KEY, sizeof(Player) * MAX_PLAYERS, PERM);
    Player *shm_players = readPlayers(shm_id);
    msgChild.code = RANKING;
    msgChild.messageInt = *nbPlayers;
    char buffer[MAX_BUFFER];
    for (int i = 0; i < *nbPlayers; i++)
    {
        sprintf(buffer, "%d) %s - %d\n", (i+1), shm_players[i].pseudo, shm_players[i].score);
        strcat(msgChild.classement, buffer);
    }
    // envoie du classement
    swrite(player->sockfd, &msgChild, sizeof(msgChild));
    sem_up0(sem_id);
    sread(player->sockfd, &msgChild, sizeof(msgChild));
    swrite(player->pipefdCP[1], &msgChild, sizeof(msgChild));
    sclose(player->sockfd);
    sclose(player->pipefdPC[0]);
    sclose(player->pipefdCP[1]);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
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

    /* blocage des signaux SIGUSR1, SIGALRM et SIGINT */
    sigset_t blocked;
    ssigemptyset(&blocked);
    ssigaddset(&blocked, SIGUSR1);
    ssigaddset(&blocked, SIGALRM);
    ssigaddset(&blocked, SIGINT);
    while (true)
    {
        ssigprocmask(SIG_UNBLOCK, &blocked, NULL);
        end_inscriptions = 0;
        i = 0;
        int nbPlayers = 0;

        alarm(TIME_INSCRIPTION);
        // Inscription des joueurs
        while (!end_inscriptions)
        {
            newsockfd = accept(sockfd, NULL, NULL);
            if (newsockfd > 0)
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
                    else
                    {
                        msg.code = INSCRIPTION_KO;
                    }
                    ret = swrite(newsockfd, &msg, sizeof(msg));
                    printf("Nb Inscriptions : %i\n", nbPlayers);
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
            //sclose(sockfd);
            continue;
        }
        else
        {
            printf("PARTIE VA DEMARRER ... \n");
            msg.code = START_GAME;
            for (i = 0; i < nbPlayers; i++)
                swrite(tabPlayers[i].sockfd, &msg, sizeof(msg));
        }
        // initialisation des pipes et des processus
        for (int i = 0; i < nbPlayers; i++)
        {

            spipe(tabPlayers[i].pipefdPC);
            spipe(tabPlayers[i].pipefdCP);

            tabPids[i] = fork_and_run2(childHandler, &tabPlayers[i], &nbPlayers);

            fds[i].fd = tabPlayers[i].pipefdCP[0];
            fds[i].events = POLLIN;

            sclose(tabPlayers[i].pipefdPC[0]);
            sclose(tabPlayers[i].pipefdCP[1]);
        }
        ssigprocmask(SIG_BLOCK, &blocked, NULL);
        // boucle du jeu

        // creation de la memoire partagée et des sémaphores
        int *tabTiles = initRandomTiles(TILE_NUMBER);
        printTable(tabTiles, NB_TILES);
        for (int i = 0; i < NB_TILES; i++)
        {
            int nbResponse = 0;
            for (int j = 0; j < nbPlayers; j++)
            {
                msg.messageInt = tabTiles[i];
                msg.code = TILE;
                swrite(tabPlayers[j].pipefdPC[1], &msg, sizeof(msg));
            }
            printf("La tuile envoyée est %d\n", msg.messageInt);
            while (nbResponse < nbPlayers)
            {
                ret = spoll(fds, nbPlayers, 1000);
                nbResponse += ret;
            }
            for (int k = 0; k < nbPlayers; k++){
                sread(tabPlayers[k].pipefdCP[0], &msg, sizeof(msg));
            }
        }
        int sem_id = initSemaphore(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | PERM, 0);
        for (int i = 0; i < nbPlayers; i++)
        {
            msg.code = END_GAME;
            swrite(tabPlayers[i].pipefdPC[1], &msg, sizeof(msg));
            // reception des scores
            sread(tabPlayers[i].pipefdCP[0], &msg, sizeof(msg));
            printf("%d\n",msg.messageInt);
            tabPlayers[i].score = msg.messageInt;
            // shm_players[i].score = msg.messageInt;
        }


        int shm_id = createPlayers(SHM_KEY, sizeof(Player) * MAX_PLAYERS, IPC_CREAT | IPC_EXCL | PERM, tabPlayers, nbPlayers);
       
        Player *shm_players = readPlayers(shm_id);
        printf("%s\n",shm_players[0].pseudo);

        createRanking(shm_players, nbPlayers);
        printf("%s\n",shm_players[0].pseudo);

        sem_up0(sem_id);

        for (int i = 0; i < nbPlayers; i++){
            sread(tabPlayers[i].pipefdCP[0], &msg, sizeof(msg));
            // printf("%s a fini de lire la memoire\n",tabPlayers[i].pseudo);
        }
        // desallocation des ressources
        free(tabTiles);
        disconnect_players(tabPlayers, nbPlayers);
        destroyShm(shm_id);
        destroySemaphore(sem_id);

    }
}