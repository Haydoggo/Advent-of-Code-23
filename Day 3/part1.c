#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 256

void main(){
	FILE* fptr = fopen("input.txt", "r");
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
			if (isdigit(*c)){
				char* numStart = c;
				int numStartPos = c - currentLine;
				int num = strtol(c, &c, 10);
				int numLength = c - numStart;
				int symbolFound = 0;
				for(int lineSelection = 0; lineSelection < 3; lineSelection++){
					char* testLine = lines[lineSelection];
					for(int i = 0; i < numLength + 2; i++){
						char testChar = testLine[numStartPos - 1 + i];
						if (isdigit(testChar) || testChar == '.' || testChar == '\n') {
							continue;
						}
						symbolFound = 1;
						break;
					}
					if (symbolFound) break;
				}
				if (symbolFound) {
					total += num;
				}
				else {
					printf("Partless number: %d\n", num);
				}
			}
		}
		strcpy(prevLine, currentLine);
		strcpy(currentLine, nextLine);
	} while(!lastLoop);
	printf("Total: %d", total);
}