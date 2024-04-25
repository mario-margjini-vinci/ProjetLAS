#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "ipc.h"
#include "utils_v1.h"
#include "structures.h" 

void createPlayers(key_t key, size_t size, int shmflg, Player *p, int nbPlayers) {
    int shm_id;

    //Créa mémoire
    shm_id = sshmget(key, size, shmflg);
    printf("Mémoire partagée créée avec succès !\n");

    //Attachement
    Player *shm_players = sshmat(shm_id);
    printf("Mémoire partagée attachée !\n");

    
    //Copie des joueurs dans la memémoire
   memcpy(&shm_players, &players, sizeof(players)*nbPlayers);
    

    printf("Les joueurs ont bien été copié dans la mémoire partagée!\n");

    //detachement
    sshmdt(shm_players);
    printf("Mémoire partagée détachée !\n");
}



Player* readPlayers(int shm_id) {
    void* shared_memory = shmat(shm_id, NULL, 0);
    if (shared_memory == (void*) -1) {
        perror("Erreur lors de l'attachement à la mémoire partagée");
        exit(EXIT_FAILURE);
    }

    Player * joueurs = shared_memory;

    if (shmdt(shared_memory) == -1) {
        perror("Erreur lors du détachement de la mémoire partagée");
        exit(EXIT_FAILURE);
    }

    return joueurs;
}

void destroyShm(int shm_id) {
    sshmdelete(shm_id);
    printf("Mémoire partagée supprimée avec succès !\n");
}

int initSemaphore(key_t key, int nsems, int perm, int val) {
    
    int sem_id = sem_create(key, nsems, perm, val);
    printf("Ensemble de sémaphores initialisé avec succès !\n");

    return sem_id;
}



void destroySemaphore(int sem_id) {
    
   sem_delete(sem_id);
    printf("Sémaphores détruits avec succès !\n");
}




