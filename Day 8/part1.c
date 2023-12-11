#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node {
    int id;
    Node *leftNode;
    Node *rightNode;
};

Node nodeLUT[26*26*26];

int nodeHash (char* label){
    return (label[0]-'A')*26*26 + (label[1]-'A')*26 + (label[2]-'A');
}

void getLabelFromId (char* label, int id) {
    label[0] = 'A' + id/(26*26);
    label[1] = 'A' + (id/26)%26;
    label[2] = 'A' + (id%26);
}

int main(){
    FILE *file = fopen("input.txt", "r");
    char instructions[300];
    fgets(instructions, 300, file);
    while(fgetc(file)!='\n');
    char line[32];
    while(fgets(line, 32, file) != NULL){
        char *c = line;
        int id = nodeHash(c);
        c = strchr(c, '(')+1;
        int leftId = nodeHash(c);
        c = strchr(c, ' ')+1;
        int rightId = nodeHash(c);
        Node *node = &nodeLUT[id];
        node->id = id;
        node->leftNode = &nodeLUT[leftId];
        node->rightNode = &nodeLUT[rightId];
    }
    Node *node = &nodeLUT[nodeHash("AAA")];
    Node *lastNode = &nodeLUT[nodeHash("ZZZ")];
    int instructionIndex = 0;
    int steps = 0;
    char label[4] = {0};
    while(node != lastNode) {
        char instruction = instructions[instructionIndex++];
        if (instruction == '\n') {
            instructionIndex = 0;
            instruction = instructions[instructionIndex++];
        }
        getLabelFromId(label, node->id);
        printf("%s, %c \n", label, instruction);
        if (instruction == 'L') {
            node = node->leftNode;
        } else {
            node = node->rightNode;
        }
        steps++;
    }
    printf("steps: %d", steps);
    return 0;
}