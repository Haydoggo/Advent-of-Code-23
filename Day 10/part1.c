#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define MAX_MAP_WIDTH 140
#define MAX_MAP_HEIGHT 140
#define MAX_LINE_LENGTH 256

int mapWidth = 0;
int mapHeight = 0;
char map[MAX_MAP_WIDTH * MAX_MAP_HEIGHT];

typedef enum
{
	Left = 0b0001,
	Up = 0b0010,
	Right = 0b0100,
	Down = 0b1000,
} Direction;

char pipeDirs[] = {
	['|'] = Up + Down,
	['-'] = Left + Right,
	['L'] = Up + Right,
	['J'] = Up + Left,
	['7'] = Down + Left,
	['F'] = Down + Right,
	['.'] = 0,
};

typedef struct
{
	int x;
	int y;
} Point;

Point pointMoved(Point point, Direction direction)
{
	point.x += (direction & Left) ? -1 : 0;
	point.x += (direction & Right) ? 1 : 0;
	point.y += (direction & Up) ? -1 : 0;
	point.y += (direction & Down) ? 1 : 0;
	return point;
}

char getPipeAtPoint(Point point)
{
	return map[point.x + point.y * mapWidth];
}

int pointsEqual(Point p1, Point p2)
{
	return p1.x == p2.x && p1.y == p2.y;
}

Direction invertDirection(Direction direction)
{
	Direction ret = 0;
	ret += (direction & Left) ? Right : 0;
	ret += (direction & Right) ? Left : 0;
	ret += (direction & Up) ? Down : 0;
	ret += (direction & Down) ? Up : 0;
	return ret;
}

int getLoopLength(Point startPoint, Direction startDir)
{
	Point point = startPoint;
	Direction direction = startDir;
	int length = 0;
	do
	{
		point = pointMoved(point, direction);
		char pipe = getPipeAtPoint(point);
		direction = pipeDirs[pipe] - invertDirection(direction);
		length++;
	} while (!pointsEqual(point, startPoint));
	return length;
}

int main()
{
	FILE *file = fopen("input.txt", "r");
	char line[MAX_LINE_LENGTH] = {0};
	Point startPoint;
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
	{
		mapWidth = strlen(line);
		memcpy(map + mapWidth * mapHeight++, line, mapWidth);
		char *c = strchr(line, 'S');
		if (c != NULL)
		{
			startPoint.x = c - line;
			startPoint.y = mapHeight - 1;
		}
	}
	printf("Start pipe: %c\n", getPipeAtPoint(startPoint));
	int length = getLoopLength(startPoint, Right);
	printf("Length: %d (longest distance from animal: %d)\n", length, length/2);
	return 0;
}