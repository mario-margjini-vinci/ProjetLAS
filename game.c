#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "game.h"
#include "structures.h"
#include "utils_v1.h"

#define NB_TILES 40
#define NB_TILES_TO_PLAY 20

static int[] tabCalc = {0,1,3,5,7,9,11,15,20,25,30,35,40,50,60,70,85,100,150,300};
static int[NB_TILES] tiles = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                            11, 11, 12, 12, 13, 13, 14, 14,
                            15, 15, 16, 16, 17, 17, 18, 18,
                            19, 19, 20, 21, 22, 23, 24, 25, 
                            26, 27, 28, 29, 30, 31};

int* initPlate(int size){
    return int[size];
    // int* plate;
    // plate = (int*) malloc(size * sizeof(int));
    // // /!\ tester la valeur renvoyée => si null exit(1)
    // return plate;
}

int* initRandomTiles(int nTiles){
    // int* randomTiles;
    // randomTiles = (int*) malloc(nTiles * sizeofint);
    // if (randomTiles == NULL) return null;
    int[NB_TILES_TO_PLAY] randomTiles;

    int tempTiles[NB_TILES];
    for (i = 0; i < NB_TILES; i++) {
        tempTiles[i] = tiles[i];
    }

    for (int i = 0; i < NB_TILES_TO_PLAY; i++){
        int r = randomIntBetween(0, NB_TILES);
        if (tiles[r] != 0)
            randomTiles[i] = tiles[r];
        else
            i--;
    }
    return randomTiles;
}

int placeTile(int pos, int tile, int* plate, int size){
    while (plate[pos%size] != 0){
        pos++;
    }
    plate[pos] = tile;
    return pos;
}


int getScore(int* plate, int size){
    int score = 0;
    int suite = 1;
    for (int i = 1; i < size; ++i)
    {
        if (plate[i]>plate[i-1])
        {
            suite++;
        }
        else{
            score += tabCalc[suite-1];
            suite = 1;
        }
    }
    score += tabCalc[suite-1];
    return score;
}

void printTable(int* plate, int size){
    for (int i = 0; i < size; i++)
    {
        printf("| %d ", plate[i]);
    }
    printf("|\n");
}

void createRanking(Player** players, int size) {
    int i, j;
    for (i = 1; i < size; i++) {
        Player* key = players[i];
        j = i - 1;
        // Déplacement des éléments du tableau[0..i-1], qui sont plus grands que la clé,
        // vers une position à l'avant de leur position actuelle
        while (j >= 0 && players[j]->score < key->score) {
            players[j + 1] = players[j];
            j = j - 1;
        }
        players[j + 1] = key;
    }
}