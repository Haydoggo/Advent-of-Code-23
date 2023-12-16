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

typedef struct
{
	Point point;
	Direction direction;
} Vector;

Point pointMoved(Point point, Direction direction)
{
	point.x += (direction & Left) ? -1 : 0;
	point.x += (direction & Right) ? 1 : 0;
	point.y += (direction & Up) ? -1 : 0;
	point.y += (direction & Down) ? 1 : 0;
	return point;
}

Direction directionRotated(Direction direction, Direction rotationDirection){
	// Right = CW, Left = CCW
	direction = rotationDirection == Left ? direction >> 1 : direction << 1;
	if (direction == 0) direction = 0b1000;
	if (direction == 0b10000) direction = 0b0001;
	return direction;
}

char getPipeAtPoint(Point point)
{
	return map[point.x + point.y * mapWidth];
}

void writeToPoint(Point point, char character) {
	int idx = point.x + point.y * mapWidth;
	if (idx >= 0 && idx < mapWidth * mapHeight) {
		map[idx] = character;
	}
}

char readAtPoint(Point point) {
	int idx = point.x + point.y * mapWidth;
	if (idx >= 0 && idx < mapWidth * mapHeight) {
		return map[idx];
	}
	return 0;
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

Vector *getLoopVectors(Point startPoint, Direction startDir)
{
	Vector *vectors = calloc(getLoopLength(startPoint, startDir), sizeof(Vector));
	int numPoints = 0;
	Point point = startPoint;
	Direction direction = startDir;
	do
	{
		vectors[numPoints].point = point;
		vectors[numPoints].direction = direction;
		numPoints++;
		point = pointMoved(point, direction);
		char pipe = getPipeAtPoint(point);
		direction = pipeDirs[pipe] - invertDirection(direction);
		
	} while (!pointsEqual(point, startPoint));
	return vectors;
}

int main()
{
	FILE *file = fopen("input.txt", "r");
	char line[MAX_LINE_LENGTH] = {0};
	Point startPoint = {0};
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
	writeToPoint(startPoint, 'L');

	printf("Start pipe: %c\n", getPipeAtPoint(startPoint));
	int numPoints = getLoopLength(startPoint, Right);
	Vector *vectors = getLoopVectors(startPoint, Right);
	
	char mapCopy[MAX_MAP_WIDTH * MAX_MAP_HEIGHT];
	memcpy(mapCopy, map, sizeof(map));

	// clear map
	for(int i = 0; i < mapWidth * mapHeight; i++) {
		map[i] = '.';
	}
	// repaint with loopPipes
	for(int i = 0; i < numPoints; i++){
		Point point = vectors[i].point;
		int idx = point.x + point.y * mapWidth;
		map[idx] = mapCopy[idx];
	}


	

	int numInsideTiles = 0;
	for(int row = 0; row < mapHeight; row++){
		Point point = {0};
		point.y = row;
		Direction dirs = 0;
		for(int col = 0; col < mapWidth; col++){
			point.x = col;
			int insideAtStart = dirs == Up+Down;
			Direction verticalPipeDirs = pipeDirs[getPipeAtPoint(point)] & (Up + Down);
			dirs ^= verticalPipeDirs;
			int insideAtEnd = dirs == Up+Down;
			if (insideAtStart && insideAtEnd && readAtPoint(point)=='.') {
				numInsideTiles++;
				writeToPoint(point, '#');
			}
		}
	}

// print map
	for(int r = 0; r < mapHeight; r++) {
		char str[MAX_MAP_WIDTH + 1] = {0};
		memcpy(str, &map[r*mapWidth], mapWidth);
		printf("%s\n", str);
	}

	printf("num inside tiles: %d", numInsideTiles);
	return 0;
}