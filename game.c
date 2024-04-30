#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "game.h"
#include "structures.h"
#include "utils_v1.h"

#define NB_TILES 40
#define NB_TILES_TO_PLAY 20
#define POINTS_CHECK 20
#define PERM 0666
#define MAX_LINE_LENGTH 10
#define MAX_FILE_LENGTH 60

FILE *file = NULL;

static int tabCalc[POINTS_CHECK] = {0,1,3,5,7,9,11,15,20,25,30,35,40,50,60,70,85,100,150,300};
static int tiles[NB_TILES] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                            11, 11, 12, 12, 13, 13, 14, 14,
                            15, 15, 16, 16, 17, 17, 18, 18,
                            19, 19, 20, 21, 22, 23, 24, 25, 
                            26, 27, 28, 29, 30, 31};

int* initPlate(int size){
    int* plate = smalloc(size *sizeof(int));
    return plate;
}

int* initRandomTiles(int nTiles){
    int* randomTiles;
    randomTiles = (int*) smalloc(nTiles * sizeof(int));

    int tempTiles[NB_TILES];
    for (int i = 0; i < NB_TILES; i++) {
        tempTiles[i] = tiles[i];
    }

    for (int i = 0; i < NB_TILES_TO_PLAY; i++){
        int r = randomIntBetween(0, NB_TILES-1);
        if (tempTiles[r] != 0){
            randomTiles[i] = tempTiles[r];
            tempTiles[r] = 0;
        }
        else
            i--;
    }
    return randomTiles;
}

int* initRandomTilesWithFile(FILE * file){
    // int fd = sopen(fileName, O_RDONLY, 0200);
    int* buffer = (int*) smalloc(MAX_FILE_LENGTH * sizeof(int));
    // char** tab = readFileToTable(fd);
    // for (int i = 0; i < NB_TILES_TO_PLAY; i++)
    // {
    //     buffer[i] = atoi(tab[i]);
    //     free(tab[i]);
    // }
    // free (tab);
    // return buffer;
    char ligne[MAX_LINE_LENGTH];
    // file = fopen(fileName, "r");
    if (file == NULL){
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }
    int i = 0;
    long int position = ftell(file);
    while (fgets(ligne, MAX_LINE_LENGTH, file) != NULL) {
        char* ptr = strchr(ligne, '\n');
        if (ptr != NULL){
            *ptr = '\0';
        }
        buffer[i] = atoi(ligne);
        position = ftell(file);
        i++;
    }
    fseek(file, position, SEEK_SET);
    if(feof(file)){
        fclose(file);
    }
    return buffer;
}

int placeTile(int pos, int tile, int* plate, int size){
    while (plate[pos%size] != 0){
        pos++;
    }
    plate[pos%size] = tile;
    return (pos+1)%size;
}


int getScore(int* plate, int size){
    int score = 0;
    int suite = 1;
    for (int i = 1; i < size; ++i)
    {
        if (plate[i]>plate[i-1] || plate[i] == 31 || plate[i-1] == 31)
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

void createRanking(Player* players, int size) {
    int i, j;
    for (i = 1; i < size; i++) {
        Player key = players[i];
        j = i - 1;
        while (j >= 0 && players[j].score < key.score) {
            players[j + 1] = players[j];
            j = j - 1;
        }
        players[j + 1] = key;
    }
}