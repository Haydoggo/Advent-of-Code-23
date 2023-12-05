#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 256
#define MAX_MAPS 64
#define MAX_SEEDS 64
typedef long long t_seed;

const int numSteps = 7;

struct MapLine {
	t_seed lowerInputRange;
	t_seed upperInputRange;
	t_seed offset;
};

void main(){
    // FILE* fptr = fopen("exampleInput.txt", "r");
    FILE* fptr = fopen("input.txt", "r");
	char line[MAX_LINE_LENGTH];
	char* c;
	
	// parse seeds
	int numSeeds = 0;
	t_seed seeds[MAX_SEEDS];
	fgets(line, MAX_LINE_LENGTH, fptr);
	c = strchr(line, ':')+1;
	t_seed num = 0;
	while (1)
	{
		char* prevC = c;
		num = strtol(c, &c, 10);
		if (c == prevC) break;
		seeds[numSeeds++] = num;
	}
	
	
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
		for(int i = 0; i < numSeeds; i++){
			seeds[i];
			for(int j = 0; j < numMapLines; j++){
				struct MapLine mapLine = mapLines[j];
				if (seeds[i] >= mapLine.lowerInputRange && seeds[i] <= mapLine.upperInputRange){
					seeds[i] += mapLine.offset;
					break;
				}
			}
		}
	}
	t_seed lowest = seeds[0];
	for(int i = 0; i < numSeeds; i++){
		if (seeds[i] < lowest) lowest = seeds[i];
	}
	printf("Lowest: %ld", lowest);
}