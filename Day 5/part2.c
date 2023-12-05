#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 256
#define MAX_MAPS 64
#define MAX_SEED_RANGES 64
typedef unsigned int t_seed;

const int numSteps = 7;

struct MapLine {
	t_seed lowerInputRange;
	t_seed upperInputRange;
	t_seed offset;
};

struct SeedRange{
	t_seed lowerBound;
	t_seed upperBound;
};

void main(){
    // FILE* fptr = fopen("exampleInput.txt", "r");
    FILE* fptr = fopen("input.txt", "r");
	char line[MAX_LINE_LENGTH];
	char* c;
	
	// parse seeds
	int numSeedRanges = 0;
	struct SeedRange seedRanges[MAX_SEED_RANGES];
	fgets(line, MAX_LINE_LENGTH, fptr);
	c = strchr(line, ':')+1;
	t_seed num = 0;
	while (1)
	{
		char* prevC = c;
		t_seed lowerBound = strtoll(c, &c, 10);
		t_seed upperBound = lowerBound + strtoll(c, &c, 10);
		if (c == prevC) break;
		struct SeedRange* seedRange = &seedRanges[numSeedRanges++];
		seedRange->lowerBound = lowerBound;
		seedRange->upperBound = upperBound;
	}
	printf("\n");
	
	
	// skip next line
	fgets(line, MAX_LINE_LENGTH, fptr);

	// Iterate over each mapping step
	struct MapLine mapLines[MAX_MAPS];
	char* validLine = line;
	while(validLine != NULL){
		// skip description line
		fgets(line, MAX_LINE_LENGTH, fptr);
		// parse maps
		memset(mapLines, 0, sizeof(struct MapLine) * MAX_MAPS);
		int numMapLines = 0;
		while(validLine = fgets(line, MAX_LINE_LENGTH, fptr)){
			if (validLine == NULL) break; // end of all maps
			if (*line == '\n') break; // end of current map
			c = line;
			struct MapLine *mapLine = &mapLines[numMapLines++];
			t_seed lowerOutputRange = strtoll(c, &c, 10);
			t_seed lowerInputRange = strtoll(c, &c, 10);
			t_seed range = strtoll(c, &c, 10);

			mapLine->lowerInputRange = lowerInputRange;
			mapLine->upperInputRange = lowerInputRange + range - 1;
			mapLine->offset = lowerOutputRange - lowerInputRange;
		}


		// remap seeds based on map
		for(int i = 0; i < numSeedRanges; i++){
			struct SeedRange *seedRange = &seedRanges[i];
			for(int j = 0; j < numMapLines; j++){
				struct MapLine mapLine = mapLines[j];
				// If the seed range overlaps the mapline range
				if (seedRange->lowerBound >= mapLine.lowerInputRange && seedRange->lowerBound <= mapLine.upperInputRange){
					// if seed range does not fully fit in mapline input range
					if (seedRange->upperBound > mapLine.upperInputRange){
						// slice it into two seed ranges on the mapline upper boundary
						struct SeedRange *greaterSeedRange = &seedRanges[numSeedRanges++];
						greaterSeedRange->lowerBound = mapLine.upperInputRange+1;
						greaterSeedRange->upperBound = seedRange->upperBound;
						seedRange->upperBound = mapLine.upperInputRange;
					}
					seedRange->lowerBound += mapLine.offset;
					seedRange->upperBound += mapLine.offset;
					break;
				}
			}
		}
	}
	t_seed lowest = seedRanges[0].lowerBound;
	for(int i = 0; i < numSeedRanges; i++){
		if (seedRanges[i].lowerBound < lowest) lowest = seedRanges[i].lowerBound;
	}
	printf("Lowest: %ld", lowest);
}