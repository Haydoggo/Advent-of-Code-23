#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct Node Node;
#define MAX_DIRECTIONS 512
#define MAX_LINE_LENGTH 32
#define NUM_POSSIBLE_LABELS 17576 // 26*26*26

struct Node {
	char label[4];
	Node *leftNode;
	Node *rightNode;
};

Node nodeLUT[NUM_POSSIBLE_LABELS];

int nodeHash (char* label){
	return (label[0]-'A')*26*26 + (label[1]-'A')*26 + (label[2]-'A');
}

long long getLoopSize(Node *node, char* directions){
	int directionIndex = 0;
	int numDirections = strlen(directions);

	// traverse the map until one node has been visited twice at the same instruction
	Node **localLUT = calloc(numDirections * NUM_POSSIBLE_LABELS, sizeof(Node*));
	long long nodeIndex = nodeHash(node->label) + directionIndex * NUM_POSSIBLE_LABELS;
	while(localLUT[nodeIndex] == NULL){
		localLUT[nodeIndex] = node;

		char direction = directions[directionIndex];
		directionIndex = (directionIndex + 1) % numDirections;

		node = (directionIndex=='L')? node->leftNode : node->rightNode;
		nodeIndex = nodeHash(node->label) + directionIndex * NUM_POSSIBLE_LABELS;
	}
	free(localLUT);

	// find size of loop
	long long steps = 0;
	long long rootIndex = nodeIndex;
	int numEnds = 0;
	directionIndex = nodeIndex/NUM_POSSIBLE_LABELS;
	do {
		char direction = directions[directionIndex];
		directionIndex = (directionIndex + 1) % numDirections;
		
		node = (directionIndex=='L')? node->leftNode : node->rightNode;
		nodeIndex = nodeHash(node->label) + directionIndex * NUM_POSSIBLE_LABELS;
		steps++;
	} while (nodeIndex != rootIndex);
	
	return steps;
}

long long gcd(long long int a, long long int b) 
{ 
    if (b == 0) 
        return a; 
    return gcd(b, a % b); 
} 

long long lcm(int a, int b) {
	 return (a / gcd(a, b)) * b; 
	 }

int main(){
	FILE *file = fopen("input.txt", "r");

	// parse input for directions
	char directions[MAX_DIRECTIONS] = {0};
	fgets(directions, MAX_DIRECTIONS, file);
	while(fgetc(file)!='\n');
	
	// parse input for nodes
	char line[MAX_LINE_LENGTH];
	long long loopSizes[64] = {0};
	Node *nodesOfInterest[64] = {0};
	int numNodesOfInterest = 0;
	int numNodes = 0;
	while(fgets(line, MAX_LINE_LENGTH, file) != NULL){
		char *c = line;
		int id = nodeHash(c);
		c = strchr(c, '(')+1;
		int leftId = nodeHash(c);
		c = strchr(c, ' ')+1;
		int rightId = nodeHash(c);
		Node *node = &nodeLUT[id];
		memcpy(node->label, line, 3);
		node->leftNode = &nodeLUT[leftId];
		node->rightNode = &nodeLUT[rightId];
		if (line[2] == 'A') {
			nodesOfInterest[numNodesOfInterest++] = node;
		}
		numNodes++;
	}
	for(int i = 0; i < numNodesOfInterest; i++){
		printf("loop size %d = %lld\n", i, getLoopSize(nodesOfInterest[i], directions));
	}
	return 0;
}

// 	Node *node;
// 	int directionIndex = 0;
// 	long long steps = 0;

// 	for(int i = 0; i < numNodesOfInterest; i++){
// 		Node localLUT[NUM_POSSIBLE_LABELS * NUM_INSTRUCTIONS];
// 		Node *node = nodesOfInterest[i];
// 		Node *orginalNode = node;
// 		int directionIndex = 0;
// 		Node blankNode = {0};
// 		long long loopSize = 0;
// 		memset(localLUT, 0, sizeof(localLUT));
// 		while (*(char*)&localLUT[nodeHash(node->label) * (directionIndex+1)] == 0){
// 			memcpy(&localLUT[nodeHash(node->label) * (directionIndex+1)], node, sizeof(Node));

// 			char direction = directions[directionIndex++];
// 			if (direction == '\n') {
// 				directionIndex = 0;
// 				direction = directions[directionIndex++];
// 			}
// 			if (direction == 'L') {
// 				node = node->leftNode;
// 			} else {
// 				node = node->rightNode;
// 			}
// 			loopSize++;
// 		}
// 		printf("%s loop size: %lld\tdirection index: %d\n", node->label, loopSize, directionIndex);
// 	}

// 	printf("steps: %d\n", steps-1);
// 	return 0;
// }