#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "structures.h"
#include "game.h"
#include "utils_v1.h"
#include "network.h"

#define PLATE_SIZE 20

int main(int argc, char *argv[])
{
    if (argc != 2){
        printf("Pour utiliser l'appli, veuillez rentrer en paramètre le numéro de port");
        exit(1);
    }
    /* PARTIE INSCRIPTION */
    int serverPort = atoi(argv[1]);

    printf("Bienvenue dans le programe d'inscription au serveur de jeu\n");
    printf("Pour participer entrez votre nom :\n");
    Message msg;
    char* bufferPseudo = readLine();

    strncpy(msg.messageText, bufferPseudo, sizeof(msg.messageText));
    free(bufferPseudo);
    msg.code = INSCRIPTION_REQUEST;

    int sockfd = initSocketClient(SERVER_IP, serverPort);
    swrite(sockfd, &msg, sizeof(msg));

    sread(sockfd, &msg, sizeof(msg));
    if (msg.code == INSCRIPTION_KO)
    {
        printf("Réponse du serveur : Inscription refusée\n");
    }

    else if (msg.code == INSCRIPTION_OK)
    {
        printf("Réponse du serveur : Inscription acceptée\n");
        int* plate = initPlate(PLATE_SIZE);
        while(msg.code != END_GAME){
            sread(sockfd, &msg, sizeof(msg));
            if (msg.code == START_GAME){
                printf("%s\n\n", "La partie commence");
            }
            else if (msg.code == CANCEL_GAME){
                printf("%s\n", "La partie est annulée");
                sclose(sockfd);
                exit(0);
            }
            else if (msg.code == TILE){
                printf("\nVous avez reçu la tuile %d\n", msg.messageInt);
                printf("%s\n", "Entrez la position de cette tuile sur votre plateau:");
                printTable(plate, PLATE_SIZE);
                char* buffer = readLine();
                int position = atoi(buffer);
                int correctPos = placeTile(position-1, msg.messageInt, plate, PLATE_SIZE);
                printf("La tuile a été placée à la position %d\n", correctPos);
                msg.tilePlaced = true;
                msg.code = TILE_PLACED;
                swrite(sockfd, &msg, sizeof(msg));
                free(buffer);
            }
        }
        printTable(plate, PLATE_SIZE);
        printf("%s\n", "La partie est terminée");
        int score = getScore(plate, PLATE_SIZE);
        printf("Votre score est de %d\n", score);

        // envoie du score
        msg.messageInt = score;
        swrite(sockfd, &msg, sizeof(msg));

        // lecture du classement
        sread(sockfd, &msg, sizeof(msg));
        printf("Voici le classement final:\n");
        printf("%s\n", msg.classement);
        msg.code = MEMORY_READ;
        swrite(sockfd, &msg, sizeof(msg));
        free(plate);
    }
    sclose(sockfd); //le fermer à la fin de la partie
    return 0;
}