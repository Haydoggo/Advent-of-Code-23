#include <stdio.h>
#include <ctype.h>

int main() {
    FILE* fptr;
    fptr = fopen("input.txt", "r");
    char c;
    int firstNum = -1;
    int lastNum = -1;
    int runningTotal = 0;
    while ((c=fgetc(fptr)) != EOF){
        if (isdigit(c)) {
            int num = c-'0';
            if (firstNum == -1) {
                firstNum = num;
            }
            lastNum = num;
        }
        if (c == '\n') {
            int number = (firstNum * 10) + lastNum;
            runningTotal += number;
            firstNum = -1;
            lastNum = -1;
        }
    }
	fclose(fptr);
    printf("Total: %d", runningTotal);
}