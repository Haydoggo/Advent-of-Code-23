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
	writeToPoint(startPoint, 'L');

	printf("Start pipe: %c\n", getPipeAtPoint(startPoint));
	int numPoints = getLoopLength(startPoint, Right);
	Vector *vectors = getLoopVectors(startPoint, Right);

	// paint left and right
	for(int i = 0; i < numPoints; i++){
		Vector vector = vectors[i];
		Direction previousDirection = pipeDirs[getPipeAtPoint(vector.point)] - vector.direction;
		previousDirection = invertDirection(previousDirection);
		
		Point p;
		p = pointMoved(vector.point, directionRotated(vector.direction, Left));
		writeToPoint(p, 'l');
		p = pointMoved(vector.point, directionRotated(vector.direction, Right));
		writeToPoint(p, 'r');
		p = pointMoved(vector.point, directionRotated(previousDirection, Left));
		writeToPoint(p, 'l');
		p = pointMoved(vector.point, directionRotated(previousDirection, Right));
		writeToPoint(p, 'r');
	}
	// paint loop
	for(int i = 0; i < numPoints; i++){
		Vector vector = vectors[i];
		if (vector.direction == Left) writeToPoint(vector.point, '<');
		if (vector.direction == Right) writeToPoint(vector.point, '>');
		if (vector.direction == Up) writeToPoint(vector.point, '^');
		if (vector.direction == Down) writeToPoint(vector.point, 'v');
		// writeToPoint(vector.point, '_');
	}
	// clear map
	for(int i = 0; i < mapWidth * mapHeight; i++) {
		if (map[i] == 'l') continue;
		if (map[i] == 'r') continue;
		if (map[i] == '<') continue;
		if (map[i] == '>') continue;
		if (map[i] == '^') continue;
		if (map[i] == 'v') continue;
		if (map[i] == '_') continue;

		map[i] = '.';
	}

	

	// print map
	for(int r = 0; r < mapHeight; r++) {
		char str[MAX_MAP_WIDTH + 1] = {0};
		memcpy(str, &map[r*mapWidth], mapWidth);
		printf("%s\n", str);
	}

	// flood fill
	int changeMade = 1;
	char directions[] = {Left, Right, Up, Down};
	while(changeMade){
		changeMade = 0;
		for(int i = 0; i < mapWidth * mapHeight; i++) {
			Point point;
			point.x = i%mapWidth;
			point.y = i/mapHeight;
			if (map[i] == 'l' || map[i] == 'r') {
				for(int j = 0; j < 4; j++) {
					Point neighbour = pointMoved(point, directions[j]);
					if (readAtPoint(neighbour) == '.') {
						writeToPoint(neighbour, map[i]);
						changeMade = 1;
						continue;
					}
					if ((map[i] == 'l' && readAtPoint(neighbour) == 'r') || (map[i] == 'r' && readAtPoint(neighbour) == 'l'))
					{
						//map[i] = '*';
						//changeMade = 1;
					}
				}
			}
			if (map[i] == '*') {
				for(int j = 0; j < 4; j++) {
					Point neighbour = pointMoved(point, directions[j]);
					if (readAtPoint(neighbour) == 'l' || readAtPoint(neighbour) == 'r'|| readAtPoint(neighbour) == '.') {
						//writeToPoint(neighbour, '*');
						//changeMade = 1;
					}
				}
			}
		}
	}

	// print map
	for(int r = 0; r < mapHeight; r++) {
		char str[MAX_MAP_WIDTH + 1] = {0};
		memcpy(str, &map[r*mapWidth], mapWidth);
		printf("%s\n", str);
	}
	free(vectors);

	//printf("Length: %d (longest distance from animal: %d)\n", length, length/2);
	return 0;
}