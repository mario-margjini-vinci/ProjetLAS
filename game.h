#ifndef _GAME_H_
#define _GAME_H_

/**
 * 
 * 
 * 
 * 
 * 
 */
int* initTiles(int nTiles);

/**
 * 
 * 
 * 
 * 
 * 
 */
int* initPalte(int size);

/**
 * 
 * 
 * 
 * 
 * 
 */
int placeTile(int pos, int tile, int** plate);

/**
 * 
 *
 * 
 *  
 * 
 */
int getScore(int* plate, int size);

/**
 * 
 * 
 * 
 * 
 * 
 * 
 */
void printTable(int* plate, int size);

#endif