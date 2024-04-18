#ifndef _GAME_H_
#define _GAME_H_

#include "utils_v1.h"

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
int* initPlate(int size);

/**
 * 
 * 
 * 
 * 
 * 
 */
int placeTile(int pos, int tile, int** plate); // enlever le double étoile de int ?

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