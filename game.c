
static int[] tabCalc = {0,1,3,5,7,9,11,15,20,25,30,35,40,50,60,70,85,100,150,300};
static int[40] tiles =

/**
 * 
 *
 * 
 *  
 * 
 */
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