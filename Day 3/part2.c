#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 256

void main(){

	FILE* fptr = fopen("exampleInput.txt", "r");
	char prevLine[MAX_LINE_LENGTH] = {'.'};
	char currentLine[MAX_LINE_LENGTH] = {'.'};
	char nextLine[MAX_LINE_LENGTH] = {'.'};
	char* lines[] = {prevLine, currentLine, nextLine};
	memset(prevLine, '.', MAX_LINE_LENGTH);
	fgets(currentLine+1, MAX_LINE_LENGTH-1, fptr);
	int lastLoop = 0;
	int total = 0;
	do {
		if (fgets(nextLine+1, MAX_LINE_LENGTH-1, fptr) == NULL) {
			memset(nextLine, '.', MAX_LINE_LENGTH-1);
			lastLoop = 1;
		}
		char* c = currentLine;
		while(*(++c)){
			if (*c == '*'){
                int partNumCount = 0;
				int symbolFound = 0;
                int gearPos = c - currentLine;
                int ratio = 1;
				for(int lineSelection = 0; lineSelection < 3; lineSelection++){
					char* testLine = lines[lineSelection];
                    char* start = testLine + gearPos - 1;
                    char* end = testLine + gearPos + 1;
					for(char* testChar = start; testChar <= end; testChar++){
						if (!isdigit(*testChar)) continue;
                        while(isdigit(*--testChar)) continue;
                        testChar++;
                        ratio *= strtol(testChar, &testChar, 10);
                        partNumCount++;
                    }
				}
				if (partNumCount == 2) {
					total += ratio;
				}
			}
		}
		strcpy(prevLine, currentLine);
		strcpy(currentLine, nextLine);
	} while(!lastLoop);
	printf("Total: %d", total);
}