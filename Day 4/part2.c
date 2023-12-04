#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STRING_SIZE 255
#define MAX_NUM_CARDS 256

void main(){
	char line[MAX_STRING_SIZE];
	char numbers[100] = {0};
	FILE* fptr = fopen("input.txt", "r");
	int total = 0;
    int cardNumber = 0;
    int cardCopies[MAX_NUM_CARDS] = {0};
    while(fgets(line, MAX_STRING_SIZE, fptr) != NULL){
        cardCopies[cardNumber]++;
        total += cardCopies[cardNumber];
		memset(numbers, 0, 100);
		int matches = 0;
		char* c = strchr(line, ':') + 1;
		while(*(c + 1) != '|'){
			numbers[strtol(c, &c, 10) - 1]++;
		}
		c += 2;
		while(*c != '\n'){
			int num = strtol(c, &c, 10);
			matches += numbers[num-1];
			numbers[num-1] = 0;
		}
        for(int i = 0; i < matches; i++){
            cardCopies[cardNumber + i + 1] += cardCopies[cardNumber];
        }
        cardNumber++;
	}
	printf("total: %d", total);
}