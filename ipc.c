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

    
    shm_id = sshmget(key, size, shmflg);
    printf("Mémoire partagée créée avec succès !\n");

    
    Player *shm_players = sshmat(shm_id);
    printf("Mémoire partagée attachée !\n");

    
    
   memcpy(&shm_players, p, sizeof(Player)*nbPlayers);
    

    printf("Les joueurs ont bien été copié dans la mémoire partagée!\n");

    
    sshmdt(shm_players);
    printf("Mémoire partagée détachée !\n");
}



Player* readPlayers(int shm_id) {
    void* shared_memory = shmat(shm_id, NULL, 0);
    if (shared_memory == (void*) -1) {
        perror("Erreur lors de l'attachement à la mémoire partagée");
        exit(EXIT_FAILURE);
    }

    Player* joueurs = shared_memory;

    if (shmdt(shared_memory) == -1) {
        perror("Erreur lors du détachement de la mémoire partagée");
        exit(EXIT_FAILURE);
    }

    return joueurs;
}
