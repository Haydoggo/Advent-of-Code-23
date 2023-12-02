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
        int minR = 0, minG = 0, minB = 0;
        while(lineStart != NULL)
        {
            int num = strtol(lineStart, &lineStart, 10);
            lineStart++; // skip the whitespace after the number
            char color = *lineStart;
            if (color == 'r' && num > minR)
                minR = num;
            if (color == 'g' && num > minG)
                minG = num;
            if (color == 'b' && num > minB)
                minB = num;
            lineStart = strchr(lineStart, ' ');
        }
        int power = minR * minG * minB;
        total += power;
    }
    printf("total: %d", total);
}