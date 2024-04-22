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
#include "utils_v1.h"
#include "network.h"

int main(int argc, char *argv[])
{
    /* PARTIE INSCRIPTION */
    printf("Bienvenue dans le programe d'inscription au serveur de jeu\n");
    printf("Pour participer entrez votre nom :\n");
    Message msg;
    int ret = sread(0, msg.messageText, MAX_TEXT);
    msg.messageText[ret - 1] = '\0';
    msg.code = INSCRIPTION_REQUEST;

    int sockfd = initSocketClient(SERVER_IP, SERVER_PORT);
    swrite(sockfd, &msg, sizeof(msg));

    sread(sockfd, &msg, sizeof(msg));
    if (msg.code == INSCRIPTION_KO)
    {
        printf("Réponse du serveur : Inscription refusée\n");
    }

    else if (msg.code == INSCRIPTION_OK)
    {
        printf("Réponse du serveur : Inscription acceptée\n");
    }
    
    /* SI INSCRIPTION REUSSIE = LANCER PARTIE DE JEU */

    sclose(sockfd); //le fermer à la fin de la partie
    return 0;
}