#ifndef _IPC_H_
#define _IPC_H_

#include "utils_v1.h"
#include "structures.h"

/**
 * PRE: key : a valid shared memory key
 *      size : the size in bytes of the shared memory
 *      p : point to a valid array of players
 *      nbrPlayers : the number of players in the array
 * POST: creates a shared memory with the specified key and size ;
 *       copies the content of the player array into the shared memory ;
 *       display a success message if the operation succeeds ;
 *       in case of failure, display an error message and exits the program
 * RES: returns the shared memory identifier
 */
int createPlayers(key_t key, size_t size, int shmflg, Player * p, int nbrPlayers);

/**
 * PRE: shm_id : the valid identifier of a shared memory
 * POST: reads player data from the shared memory identified by shm_id ;
 *       returns a pointer to the player data structure ;
 *       display an error message and exits the program in case of failure
 * RES: returns a pointer to the player data structure
 */
Player* readPlayers(int shm_id);

/**
 * PRE: shm_id : the valid identifier of a shared memory
 * POST: removes the shared memory identified by shm_id;
 *       display a success message if the operation succeeds
 */
void destroyShm(int shm_id);

/**
 * PRE: key : a valid semaphore key
 *      nsems : the number of semaphores to initialize
 *      perm : the permission mode of the semaphores
 *      val : the initial value of the semaphores
 * POST: initializes a set of semaphores with the specified key, number of semaphores ; 
 *       permission mode, and initial value specified ;
 *       display a success message if the operation succeeds ;
 *       in case of failure, display an error message and exits the program
 * RES: returns the semaphore set identifier
 */
int initSemaphore(key_t key, int nsems, int perm, int val);

/**
 * PRE: sem_id : the valid identifier of a semaphore set
 * POST: destroys the semaphore set identified by sem_id ;
 *       display a success message if the operation succeeds
 */
void destroySemaphore(int sem_id);

#endif
