#ifndef _GAME_H_
#define _GAME_H_

#include "utils_v1.h"
#include "structures.h"

/**
 * PRE: nTiles: number of tiles to be generated
 * POST: allocates memory for nTiles random tiles
 * RES:  return a pointer to the first tile
 */
int* initRandomTiles(int nTiles);


/**
 * PRE: fileName: a valid file name
 * POST: reads the file fileName and allocates memory for NB_TILES_TO_PLAY tiles
 * RES:  return a pointer to the first tile
 * 
 */
int* initRandomTilesWithFile(char* fileName);

/**
 * PRE: size: the size of the plate
 * POST: allocates memory for a plate of size size
 * RES:  return a pointer to the first tile
 */
int* initPlate(int size);

/**
 * PRE: pos: the position where the tile should be placed
 *      tile: the tile to be placed
 *      plate: the plate where the tile should be placed
 *      size: the size of the plate 
 * POST: places the tile on the plate at the position pos
 * RES:  return the position in which the tile has been placed
 */
int placeTile(int pos, int tile, int* plate, int size);

/**
 * PRE: plate: the plate from which the score should be calculated
 *     size: the size of the plate
 * POST: calculates the score of the plate
 * RES:  return the score of the plate
 */
int getScore(int* plate, int size);

/**
 * PRE: plate: the plate to be printed
 *     size: the size of the plate
 * POST: prints the plate
 */
void printTable(int* plate, int size);

/**
 * PRE: players: the players to be ranked 
 *      size: the number of players
 * POST: sorts the players by score
 */
void createRanking(Player* players, int size);

#endif