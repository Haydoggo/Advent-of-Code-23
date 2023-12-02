#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
    FILE *fptr = fopen("input.txt", "r");
    char line[256] = {0};
    int total = 0;
    for (int index = 1; fgets(line, 256, fptr) != NULL; index ++)
    {
        char* lineStart = strchr(line, ':') + 2; // skip the Game #: + whitespace
        while(lineStart != NULL)
        {
            int num = strtol(lineStart, &lineStart, 10);
            lineStart++; // skip the whitespace after the number
            char color = *lineStart;
            if (color == 'r' && num > 12)
                break;
            if (color == 'g' && num > 13)
                break;
            if (color == 'b' && num > 14)
                break;
            lineStart = strchr(lineStart, ' ');
        }
        if (lineStart == NULL){
            total += index;
        }
    }
    printf("total: %d", total);
}