#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define MAX_LINE_LENGTH 256
#define MAX_HISTORY_SIZE 32

typedef struct{
	int values[MAX_HISTORY_SIZE];
	int numValues;
} History;

int main(){
	FILE *file = fopen("input.txt", "r");
	char line[MAX_LINE_LENGTH] = {0};
	int total = 0;
	while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
		History history = {0};
		char *c = line;
		while(1) {
			char *cStart = c;
			int num = strtol(c, &c, 10);
			if (c == cStart) break;
			history.values[history.numValues++] = num;
			if (history.numValues >= MAX_HISTORY_SIZE){
				printf("too many numbers bro");
				return 1;
			}
		}

		History *subHistories = calloc(sizeof(History),history.numValues);
		subHistories[0] = history;
		int numSubHistories = 1;
		for(int i = 0; i < history.numValues; i++){
			History *subHistory = &subHistories[i];
			int isAllZeros = 1;
			for(int j = 0; j < subHistory->numValues; j++){
				if (subHistory->values[j] != 0) {
					isAllZeros = 0;
					break;
				}
			}
			if (isAllZeros){
				break;
			}
			History *nextSubHistory = &subHistories[numSubHistories++];
			for(int j = 1; j < subHistory->numValues; j++){
				nextSubHistory->values[j-1] = subHistory->values[j] - subHistory->values[j-1];
				nextSubHistory->numValues++;
			}
		}
		for(int i = numSubHistories-1; i > 0; i--){
			History *lowerHistory = &subHistories[i];
			History *upperHistory = &subHistories[i-1];
			upperHistory->values[upperHistory->numValues] = upperHistory->values[upperHistory->numValues-1] + lowerHistory->values[lowerHistory->numValues-1];
			upperHistory->numValues++;
		}
		total += subHistories[0].values[subHistories[0].numValues-1];
		free(subHistories);
	}
	printf("total: %d\n", total);
	return 0;
}