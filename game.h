#ifndef _GAME_H_
#define _GAME_H_

#include "utils_v1.h"
#include "structures.h"

/**
 * 
 * 
 * 
 * 
 * 
 */
int* initRandomTiles(int nTiles);

/**
 * 
 * 
 * 
 * 
 * 
 */
int* initPlate(int size);

/**
 * 
 * 
 * 
 * 
 * 
 */
// rajouter le double étoile de int ?
int placeTile(int pos, int tile, int* plate, int size);

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

/**
 * 
 * 
 * 
 * 
 * 
 * 
 */
void createRanking(Player* players, int size);

#endif