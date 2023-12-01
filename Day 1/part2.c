#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main() {
    const char* numbers[] = {
        "0", "zero",
        "1", "one",
        "2", "two",
        "3", "three",
        "4", "four",
        "5", "five",
        "6", "six", 
        "7", "seven",
        "8", "eight",
        "9", "nine"
    };

    int runningTotal = 0;

    FILE* fptr;
    fptr = fopen("input.txt", "r");
    char line[128];
    while (fgets(line, 128, fptr) != NULL){
        int firstDigitStringIndex = -1;
        int firstDigitLookupIndex = -1;
        int lastDigitStringIndex = -1;
        int lastDigitLookupIndex = -1;
        int pos;
        for(int i = 0; i < 20; i++){
            char* place = strstr(line, numbers[i]);
            if (place == NULL) continue;
            else pos = place - line; 
            if (firstDigitStringIndex == -1 || pos < firstDigitStringIndex) {
                firstDigitStringIndex = pos;
                firstDigitLookupIndex = i;
            }
            // try to find the last occurence of the number
            char* start = place + 1;
            while((place = strstr(start, numbers[i])) != NULL) {
                start = place + 1;
                pos = place - line;
            }
            if (lastDigitStringIndex == -1 || pos > lastDigitStringIndex) {
                lastDigitStringIndex = pos;
                lastDigitLookupIndex = i;
            }
        }
        //bitshifts are just cool integer division
        int firstNumber = firstDigitLookupIndex >> 1; 
        int lastNumber = lastDigitLookupIndex >> 1;
        int number = firstNumber * 10 + lastNumber;
        runningTotal += number;
    }
	fclose(fptr);
    printf("Total: %d", runningTotal);
}