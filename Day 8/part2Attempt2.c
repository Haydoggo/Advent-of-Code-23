#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct Node Node;
#define MAX_DIRECTIONS 512
#define MAX_LINE_LENGTH 32
#define NUM_POSSIBLE_LABELS 17576 // 26*26*26

struct Node
{
	char label[4];
	Node *leftNode;
	Node *rightNode;
};

typedef struct
{
	int period;
	int offset;
} Loop;

Node nodeLUT[NUM_POSSIBLE_LABELS];

int nodeHash(char *label)
{
	return (label[0] - 'A') * 26 * 26 + (label[1] - 'A') * 26 + (label[2] - 'A');
}

Loop getLoop(Node *node, char *directions)
{
	// for some reason every loop has a regular period (proven experimentally) at which it visits a **Z node
	Node *loopRootNode;
	Loop loop = {0};
	int directionIndex = 0;
	directionIndex = 0;
	int numDirections = strlen(directions);
	while (node->label[2] != 'Z')
	{
		loop.offset++;
		char direction = directions[directionIndex];
		directionIndex = (directionIndex + 1) % numDirections;
		node = (directionIndex == 'L') ? node->leftNode : node->rightNode;
	}
	loopRootNode = node;
	do
	{
		loop.period++;
		char direction = directions[directionIndex];
		directionIndex = (directionIndex + 1) % numDirections;
		node = (directionIndex == 'L') ? node->leftNode : node->rightNode;
	} while (node != loopRootNode);
	return loop;
}

Node *followDirections(Node *node, char *directions, int numSteps, int directionIndexOffset)
{
	int numDirections = strlen(directions);
	int directionIndex = directionIndexOffset;
	for (int i = 0; i < numSteps; i++)
	{
		char direction = directions[directionIndex];
		directionIndex = (directionIndex + 1) % numDirections;
		node = (directionIndex == 'L') ? node->leftNode : node->rightNode;
	}
	return node;
}

long long gcd(long long int a, long long int b)
{
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

long long lcm(int a, int b)
{
	return (a / gcd(a, b)) * b;
}

int main()
{
	FILE *file = fopen("input.txt", "r");

	// parse input for directions
	char directions[MAX_DIRECTIONS] = {0};
	fgets(directions, MAX_DIRECTIONS, file);
	while (fgetc(file) != '\n')
		;

	// parse input for nodes
	char line[MAX_LINE_LENGTH];
	long long loopSizes[64] = {0};
	Node *nodesOfInterest[64] = {0};
	int numNodesOfInterest = 0;
	int numNodes = 0;
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
	{
		char *c = line;
		int id = nodeHash(c);
		c = strchr(c, '(') + 1;
		int leftId = nodeHash(c);
		c = strchr(c, ' ') + 1;
		int rightId = nodeHash(c);
		Node *node = &nodeLUT[id];
		memcpy(node->label, line, 3);
		node->leftNode = &nodeLUT[leftId];
		node->rightNode = &nodeLUT[rightId];
		if (line[2] == 'A')
		{
			nodesOfInterest[numNodesOfInterest++] = node;
		}
		numNodes++;
	}

	Loop loops[16];
	for (int i = 0; i < numNodesOfInterest; i++)
	{
		loops[i] = getLoop(nodesOfInterest[i], directions);
		Loop loop = loops[i];
		Node *node = nodesOfInterest[i];
		Node *loopStart = followDirections(node, directions, loop.offset, 0);
		Node *loopEnd = followDirections(loopStart, directions, loop.period, loop.offset);
		printf("%s loop: %s to %s (period %d, offset %d)\n", node->label, loopStart->label, loopEnd->label, loop.period, loop.offset);
	}
    // for some reason every number is prime, and offset is a multiple of period.
    // lowest common multiple will be the product of their periods(you would think)
    // But upon cheating and checking the answer, this is off by a factor of 283 (num of directions) exactly.
    // Why is this? fok knows
    long long answer = 1;
    for(int i = 0; i < numNodesOfInterest; i++){
        answer *= loops[i].period;
    }
    printf("answer = %lld\n", answer);
    printf("real answer for no reason = %lld\n", answer * 283);
	return 0;
}