#ifndef _IPC_H_
#define _IPC_H_

/**
 * 
 * 
 * 
 */
void createPlayers(key_t key, size_t size, int shmflg);

/**
 * 
 * 
 * 
 * 
 * 
 */
Player* readPlayers(int shm_id);

/**
 * 
 * 
 * 
 * 
 * 
 * 
 */
void destroyShm(int shm_id);

/**
 * 
 * 
 * 
 * 
 * 
 */
int initSemaphore(key_t key, int nsems, int perm, int val);

/**
 * 
 * 
 * 
 * 
 * 
 */
void destroySemaphore(int sem_id);

#endif