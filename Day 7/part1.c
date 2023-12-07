#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HAND_SIZE 5
#define MAX_LINES 1000

unsigned char cardStrength[256];

typedef enum HandType {
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	FIVE_OF_A_KIND,
	HAND_TYPE_SIZE
} HandType;

typedef char Card;

typedef struct {
	Card cards[HAND_SIZE];
	HandType type;
	int bid;
} Hand;

enum HandType getHandType(Hand *hand){
	int duplicateCounts[HAND_SIZE] = {0};

	for(int i = 0; i < HAND_SIZE; i++){ //card index
		Card card = hand->cards[i];
		for(int j = 0; j < HAND_SIZE; j++){ // duplicate index
			if (card == hand->cards[j]){
				duplicateCounts[j]++;
			}
		}
	}

	int highestDuplicateCount = duplicateCounts[0];
	int numCardsInPairs = 0;
	for(int i = 0; i < HAND_SIZE; i++){
		int duplicateCount = duplicateCounts[i];
		if (duplicateCount > highestDuplicateCount)
			highestDuplicateCount = duplicateCount;
		if (duplicateCount == 2)
			numCardsInPairs++;
	}
	if (highestDuplicateCount == 5) return FIVE_OF_A_KIND;
	if (highestDuplicateCount == 4) return FOUR_OF_A_KIND;
	if (highestDuplicateCount == 3 && numCardsInPairs == 2) return FULL_HOUSE;
	if (highestDuplicateCount == 3) return THREE_OF_A_KIND;
	if (numCardsInPairs == 4) return TWO_PAIR;
	if (numCardsInPairs == 2) return ONE_PAIR;
	if (highestDuplicateCount == 1) return HIGH_CARD;
}

int doesThisHandWin(Hand *thisHand, Hand *otherHand){
	if (thisHand->type > otherHand->type) return 1;
	if (thisHand->type < otherHand->type) return 0;
	for(int i = 0; i < HAND_SIZE; i++){
		unsigned char thisCardStrength = cardStrength[thisHand->cards[i]];
		unsigned char otherCardStrength = cardStrength[otherHand->cards[i]];
		if(thisCardStrength > otherCardStrength) return 1;
		if(thisCardStrength < otherCardStrength) return 0;
	}
}

typedef struct HandListNode HandListNode;

struct HandListNode {
	Hand *hand;
	HandListNode *nextNode;
};

int main() {
	cardStrength['2'] = 1;
	cardStrength['3'] = 2;
	cardStrength['4'] = 3;
	cardStrength['5'] = 4;
	cardStrength['6'] = 5;
	cardStrength['7'] = 6;
	cardStrength['8'] = 7;
	cardStrength['9'] = 8;
	cardStrength['T'] = 9;
	cardStrength['J'] = 10;
	cardStrength['Q'] = 11;
	cardStrength['K'] = 12;
	cardStrength['A'] = 13;

	FILE *file = fopen("input.txt", "r");
	char line[32];

	int numHands = 0;
	Hand allHands[MAX_LINES] = {0};
	HandListNode allHandListNodes[MAX_LINES] = {0};
	HandListNode *listHead = allHandListNodes;
	
	// Parse input to get hands
	while(fgets(line, 32, file) != NULL){
		
		Hand *hand = &allHands[numHands];
		memcpy(hand->cards, line, HAND_SIZE);
		hand->type = getHandType(hand);
		hand->bid = strtol(line + HAND_SIZE, NULL, 10);
		
		HandListNode *node = &allHandListNodes[numHands];
		node->hand = hand;
		
		numHands++;
	}

	
	// Sort Hands in linked list (insertion sort)
	for(int i = 1; i < numHands; i++){
		HandListNode *node = &allHandListNodes[i];
		HandListNode *tempNode;
		HandListNode * lastNode = NULL;
		node->nextNode = listHead;
		while(node->nextNode != NULL && doesThisHandWin(node->hand, node->nextNode->hand)){
			tempNode = node->nextNode;
			node->nextNode = tempNode->nextNode;
			tempNode->nextNode = node;
			if (lastNode != NULL){
				lastNode->nextNode = tempNode;
			}
			lastNode = tempNode;

		}
		if (node->nextNode == listHead){
			listHead = node;
		}
	}
	
	// Sum total winnings
	long long totalWinnings = 0;
	int rank = 1;
	for (HandListNode *node = listHead; node != NULL; node = node->nextNode) {
		totalWinnings += node->hand->bid * rank++;
	}
	printf("Total winnings: %lld", totalWinnings);
	return 0;
}