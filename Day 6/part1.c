#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROUNDS 16

struct Round {
    int time;
    int distance;
};

void main(){
    int numRounds = 0;
    int times[MAX_ROUNDS] = {0};
    // FILE* file = fopen("exampleInput.txt", "r");
    FILE* file = fopen("input.txt", "r");

    char line[256] = {0};
    fgets(line, 256, file);
    char *c;
    c = strchr(line, ':')+1;
    int num;
    // get times
    while(1){
        char *oldC = c;
        int time = strtol(c,&c,10);
        if (c==oldC) break;
        times[numRounds++] = time;
    }
    fgets(line, 256, file);
    c = strchr(line, ':')+1;
    
    int totalWinProduct = 1;
    // get distances
    for(int i = 0; i < numRounds; i++){
        char *oldC = c;
        int recordDistance = strtol(c,&c,10);
        if (c==oldC) break;
        int raceTime = times[i];
        int wins = 0;
        // do races
        for(int windTime = 0; windTime <= raceTime; windTime++){
            int movingTime = raceTime - windTime;
            int speed = windTime;
            int travelDist = speed*movingTime;
            wins += travelDist > recordDistance;
        }
        totalWinProduct *= wins;
    }
    printf("total: %d", totalWinProduct);
}