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

void shuffleDeck(card* deck, char colors[][10]); //Shuffles the Deck when option 1 is pressed
void addDeck(card** head, int numCards, card* deck, int *numDeck); //Create linked lists from the deck
void printDeck(card* head); //Print the linked list
void currentTurn(card** head, int currTurn, card deck[], card** centerrow, int* numCards, int* numDeck, int *centerRowNum, bool *isDoubleMatch); //The function for a players turn
bool viablePlay(card* head, int numCardsPlayed, int numCardPlayed, int numCardPlayed2, card* centerrow); //Check if the current play is viable
bool checkDoubleMatch(card* head, int numCardPlayed, int numCardPlayed2, card* centerrow); //Check if there's a double color match
bool checkMatches(card* head, int numCards, card* centerrow, card* centerPT); //Look for viable plays before user makes a turn, else draw card
void playCard(card** head, int numCardPlayed, int *colorMatch, card* centerrow); //Remove card from player deck
void refreshCenter(card** centerrow); //Clear the centerrow
void addToCenter(card** centerrow, int numCardPlayed, card** head); //Remove card from player deck and add to centerrow
int calcPoints(card** head); //Calculate Points after a round
void freeGame(card** player1, card** player2, card** centerrow); //Clear all memory from the linked lists
void swap(card deck[]); //Shuffles the deck when option 2 is pressed