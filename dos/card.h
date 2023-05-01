#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct card_s {
	char color[10];
	int value;
	char action[15];
	struct card_s* pt;
	//bool isMatched;
} card;

void shuffleDeck(card* deck, char colors[][10]);
void addDeck(card** head, int numCards, card* deck, int *numDeck);
void printDeck(card* head);
void currentTurn(card** head, int currTurn, card deck[], card** centerrow, int* numCards, int* numDeck, int *centerRowNum, bool *isDoubleMatch);
bool viablePlay(card* head, int numCardsPlayed, int numCardPlayed, int numCardPlayed2, card* centerrow);
bool checkDoubleMatch(card* head, int numCardPlayed, int numCardPlayed2, card* centerrow);
bool checkMatches(card* head, int numCards, card* centerrow, card* centerPT);
void playCard(card** head, int numCardPlayed, int *colorMatch, card* centerrow);
void refreshCenter(card** centerrow);
void addToCenter(card** centerrow, int numCardPlayed, card** head);
int calcPoints(card** head);
void freeGame(card** player1, card** player2, card** centerrow);
void swap(card deck[]);