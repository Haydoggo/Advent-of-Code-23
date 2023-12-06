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
    FILE* file = fopen("input.txt", "r");
    char line[256] = {0};
    char *c;
    

    // get time
    fgets(line, 256, file);
    c = strchr(line, ':')+1;
    char numString[32]={0};
    char *numStringChar = numString;
    while(*c++){
        if (*c>='0'&&*c<='9') (*numStringChar++ = *c);
    }
    long long raceTime = strtoll(numString,NULL,10);

    // get distance
    fgets(line, 256, file);
    c = strchr(line, ':')+1;
    memset(numString, 0, 32);
    numStringChar = numString;
    while(*c++){
        if (*c>='0'&&*c<='9') (*numStringChar++ = *c);
    }
    long long recordDistance = strtoll(numString,NULL,10);
    
    // do races
    int wins = 0;
    for(long long windTime = 0; windTime <= raceTime; windTime++){
        long long movingTime = raceTime - windTime;
        long long speed = windTime;
        long long travelDist = speed*movingTime;
        wins += travelDist > recordDistance;
    }
    printf("total: %d", wins);
}