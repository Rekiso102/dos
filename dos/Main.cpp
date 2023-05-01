#define _CRT_SECURE_NO_WARNINGS
#include "card.h" //A header file that includes the function and struct definitions

#define DECKSIZE 108 //Define size of a gamedeck
#define USERDECK 7 //Define size of a user's deck
#define P1 1
#define P2 2

/*
Known Bugs:
1. Incorrect Inputs
2. Viable play is fixed, but the move selection and centerrow removal must be changed to implement?

Extra Credit:
1. More Players
*/


int main(void) {
	srand((int)time(0));

	card deck[DECKSIZE];
	card* player1_deck = NULL; //head of player1's deck
	card* player2_deck = NULL; //head of player2's deck
	card* centerrow = NULL; //head of centerrow deck
	card* centerPT = NULL; //centerrow pointer

	char colors[5][10] = { "yellow", "red", "green", "blue", "anycolor" }; //array containing the color options for cards
	char userFile[100];

	bool isDoubleMatch = false;

	int userMode, numDeck = DECKSIZE, currTurn = P1, numCardsP1 = USERDECK, numCardsP2 = USERDECK, centerRowNum = 2;
	int P1Score = 0, P2Score = 0, roundPoints = 0, rdWinner = 0, winner = 0;
	
	printf("Welcome to Dos!\n");
	printf("By: Conrad Kusion & Tony Pluta\n");
	printf("Enter 1 to shuffle a deck, 2 to load a deck: ");
	scanf("%d", &userMode);

	if (userMode == 1) {
		shuffleDeck(deck, colors);
		printf("The deck is shuffled. Press any key to deal cards.\n");

	}
	else if (userMode == 2) {
		printf("Enter the name of a valid deck file: ");
		scanf(" %s", userFile);
		FILE* deckFile = NULL;
		deckFile = fopen(userFile, "r");
		if (deckFile == NULL) {
			printf("Error 5: Unable to open file \"%s\"\n", userFile);
			return 5;
		}

		for (int i = 0; i < DECKSIZE; i++) {
			fscanf(deckFile, "%s %d %*s", deck[i].color, &deck[i].value);
			if (strcmp(deck[i].color, "black") == 0 && deck[i].value == 2) {
				strcpy(deck[i].color, "anycolor");
			}
			else if (deck[i].value == 11) {
				deck[i].value = 0;
				strcpy(deck[i].action, "#");
			}	
		}

		swap(deck);
		fclose(deckFile);
	}

	//TODO: Check for User pressing a key

	for (int i = 1; i <= 7; i++) { //Alternate Deal for userDecks
		addDeck(&player1_deck, 1, deck, &numDeck);
		addDeck(&player2_deck, 1, deck, &numDeck);
	}
	addDeck(&centerrow, 2, deck, &numDeck); //Create CenterRow

	centerPT = centerrow;

	//TODO: Create Full Game
	while (numDeck != 0 || P1Score < 200 || P2Score < 200) {
		while (numCardsP2 != 0 || numCardsP1 != 0) {
			if (currTurn == P1) { //Player 1 Turn
				currentTurn(&player1_deck, currTurn, deck, &centerrow, &numCardsP1, &numDeck, &centerRowNum, &isDoubleMatch);
				if (numCardsP1 == 0) {
					rdWinner = P1;
					break;
				}
				else {
					currTurn = P2;
				}
			}
			else if (currTurn == P2) { //Player 2 Turn
				currentTurn(&player2_deck, currTurn, deck, &centerrow, &numCardsP2, &numDeck, &centerRowNum, &isDoubleMatch);
				if (numCardsP2 == 0) {
					rdWinner = P2;
					break;
				}
				else {
					currTurn = P1;
				}
			}
		}
		//Print round points and total points
		if (rdWinner == P1) {
			roundPoints = calcPoints(&player2_deck);
			P1Score += roundPoints;
			printf("Player 1 wins the round with %d point(s)!\n", roundPoints);
		}
		else if (rdWinner == P2) {
			roundPoints = calcPoints(&player1_deck);
			P2Score += roundPoints;
			printf("Player 2 wins the round with %d point(s)!\n", roundPoints);
		}
		printf("Total Points:\n");
		printf("Player One: %d point(s)\n", P1Score);
		printf("Player Two: %d point(s)\n", P2Score);
		roundPoints = 0;
		//TODO: Clear old decks and centerrows and create new ones.
		freeGame(&player1_deck, &player2_deck, &centerrow);

		for (int i = 1; i <= 7; i++) { //Alternate Deal for userDecks
			addDeck(&player1_deck, 1, deck, &numDeck);
			addDeck(&player2_deck, 1, deck, &numDeck);
		}
		addDeck(&centerrow, 2, deck, &numDeck); //Create CenterRow
		numCardsP1 = 7;
		numCardsP2 = 7;
		centerRowNum = 2;
		currTurn = P1;
		rdWinner = 0;
		isDoubleMatch = false;
		if (P1Score > 200 || P2Score > 200) {
			break;
		}
		if (numDeck == 0) {
			break;
		}
	}

	if (numDeck == 0) {
		printf("No more cards in the deck! Game over!\n");
		if (P2Score > P1Score) {
			printf("Player 2 Wins!\n");
			return 2;
		}
		else if (P1Score > P2Score) {
			printf("Player 1 Wins!\n");
			return 1;
		}
		else {
			printf("Tie!\n");
			return 3;
		}
	}

	if (P1Score >= 200) {
		winner = P1;
	}
	else if (P2Score >= 200) {
		winner = P2;
	}
	printf("The winner is Player %d!\n", winner);
	freeGame(&player1_deck, &player2_deck, &centerrow);

	return 0;
}

void shuffleDeck(card* deck, char colors[][10]) { //shuffles the game deck
	for (int i = 0; i < DECKSIZE; i++) {
		int value = (rand() % 11) + 1;

		if (value == 11) {
			strcpy(deck[i].action, "#");
		}
		else {
			deck[i].value = value;
		}

		if (value == 2) {
			strcpy(deck[i].color, colors[4]);
		}
		else {
			strcpy(deck[i].color, colors[rand() % 4]);
		}
	}
}

void addDeck(card** head, int numCards, card* deck, int* numDeck) { //removes cards from the deck and adds them to the player's deck (defines linked list)
	for (int i = 0; i < numCards; i++) {
		card* temp = (card*)malloc(sizeof(card));
		strcpy(temp->color, deck[*numDeck - 1].color);
		if (strcmp(deck[*numDeck - 1].action, "#") == 0) {
			strcpy(temp->action, deck[*numDeck - 1].action);
		}
		else {
			temp->value = deck[*numDeck - 1].value;
		}
		temp->pt = NULL;
		if (*head == NULL)
		{
			*head = temp;
			temp->pt = NULL;
		}
		else 
		{
			temp->pt = *head;
			*head = temp;
		}
		(*numDeck)--;
	}
}

void printDeck(card* head) { //Goes through the linked list and prints the deck
	card* cursor;
	cursor = head;
	while (cursor != NULL) {
		if (strcmp(cursor->action, "#") == 0) {
			printf("%s %s", cursor->action, cursor->color);
		}
		else {
			printf("%d %s", cursor->value, cursor->color);
		}

		if (cursor->pt != NULL) {
			printf(", ");
		}
		cursor = cursor->pt;
	}
	printf("\n");
}


void currentTurn(card** head, int currTurn, card deck[], card** centerrow, int* numCards, int* numDeck, int* centerRowNum, bool *isDoubleMatch) {
	card* centerPT = *centerrow; //centerrow pointer

	bool endTurn = false, viablePlays, addedCard = false;
	int numCardPlayed = 0, numCardPlayed2 = 0, numCardPlayed3 = 0, numColorMatches = 0, cardsPlayed = -1;
	
	if (*isDoubleMatch) {
		addDeck(head, 1, deck, numDeck);
		(*numCards)++;
	}

	*isDoubleMatch = false;

	while (!endTurn) {
		if (currTurn == P1) {
			printf("Players One's hand: ");
		}
		else {
			printf("Players Two's hand: ");
		}
		printDeck(*head);

		printf("Centerline: ");
		printDeck(*centerrow);

		viablePlays = checkMatches(*head, *numCards, *centerrow, centerPT);

		if (!viablePlays && !addedCard) {
			printf("No viable plays, adding card to deck!\n");
			addDeck(head, 1, deck, numDeck);
			addedCard = true;
			(*numCards)++;
			continue;
		}
		else if (!viablePlays && addedCard) {
			printf("No viable plays! Add a card to the center!\n");
			if (numColorMatches == 1 && *numCards > 1) {
				printf("You color matched and get to play two card(s) in the center row.\n");
				numCardPlayed = 0;
				numCardPlayed2 = 0;
				while (numCardPlayed <= 0 || numCardPlayed > *numCards && numCardPlayed2 <= 0 || numCardPlayed2 > *numCards) {
					printf("Select two additional card(s) from 1-%d to place in the center row (Format: #,#): ", *numCards);
					scanf(" %d%*c%d", &numCardPlayed, &numCardPlayed2);
				}
				addToCenter(centerrow, numCardPlayed, head);
				numCardPlayed2--;
				addToCenter(centerrow, numCardPlayed2, head);
				*numCards -= 2;
				*centerRowNum += 2;
				centerPT = *centerrow;
				viablePlays = false;
				addedCard = false;
				endTurn = true;
				break;
			}
			else if (numColorMatches == 2 && *numCards > 2) {
				printf("You color matched and get to play three card(s) in the center row.\n");
				numCardPlayed = 0;
				numCardPlayed2 = 0;
				int numCardPlayed3 = 0;
				while (numCardPlayed <= 0 || numCardPlayed > *numCards) {
					printf("Select three additional card(s) from 1-%d to place in the center row (Format: #,#,#): ", *numCards);
					scanf(" %d%*c%d%*c%d", &numCardPlayed, &numCardPlayed2, &numCardPlayed3); //Add three cards to deck
				}
				addToCenter(centerrow, numCardPlayed, head);
				(*numCards)--;
				(*centerRowNum)++;
			}
			else {
				numCardPlayed = 0;
				while (numCardPlayed <= 0 || numCardPlayed > *numCards) {
					printf("Select one additional card from 1-%d to place in the center row: ", *numCards);
					scanf(" %d", &numCardPlayed);
				}
				addToCenter(centerrow, numCardPlayed, head);
				(*numCards)--;
				(*centerRowNum)++;
			}
			centerPT = *centerrow;
			if (*numCards == 0) {
				endTurn = true;
				return;
			}
			break;
		}


		while (cardsPlayed != 1 && cardsPlayed != 2) {
			addedCard = false;
			viablePlays = true;
			if (strcmp(centerPT->action, "#") == 0) {
				printf("How many cards do you want to play on %s %s (1 or 2): ", centerPT->action, centerPT->color);
			}
			else {
				printf("How many cards do you want to play on %d %s (1 or 2): ", centerPT->value, centerPT->color);
			} 
			scanf("%d", &cardsPlayed);
		}
		if (cardsPlayed == 1) {
			numCardPlayed = 0;
			while (numCardPlayed <= 0 || numCardPlayed > *numCards) {
				printf("Select a card from 1-%d: ", *numCards);
				scanf(" %d", &numCardPlayed);
			}
			if (viablePlay(*head, 1, numCardPlayed, 0, centerPT)) {
				playCard(head, numCardPlayed, &numColorMatches, centerPT);
				centerPT = centerPT->pt;
				(*numCards)--;
			}
			else {
				printf("This selection does not total to the center row card. Select again.\n"); //Add functionality for incorrect moves
				continue;
			}
			if (*numCards == 0) {
				return;
			}
			cardsPlayed = 0;
		}
		else if (cardsPlayed == 2) {
			numCardPlayed = 0;
			numCardPlayed2 = 0;
			while (numCardPlayed <= 0 || numCardPlayed > *numCards && numCardPlayed2 <= 0 || numCardPlayed2 > *numCards) {
				printf("Select two cards from 1-%d (Format: #,#): ", *numCards);
				scanf(" %d%*c%d", &numCardPlayed, &numCardPlayed2);
			}
			if (viablePlay(*head, 2, numCardPlayed, numCardPlayed2, centerPT)) {
				if (checkDoubleMatch(*head, numCardPlayed, numCardPlayed2, centerPT)) {
					*isDoubleMatch = true;
					printf("That's a double color match! Adding a card to opponents' decks!\n");
					numColorMatches--;
				}
				playCard(head, numCardPlayed, &numColorMatches, centerPT);
				numCardPlayed2--;
				playCard(head, numCardPlayed2, &numColorMatches, centerPT);
				centerPT = centerPT->pt;
				*numCards -= 2;

			}
			else {
				printf("This selection does not total to the center row card. Select again.\n"); //Add functionality for incorrect moves
				continue;
			}
			if (*numCards == 0) {
				return;
			}
			cardsPlayed = 0;
		}

		if (centerPT == NULL) {
			printf("There are no more center row cards to match.\n");
			refreshCenter(centerrow);
			addDeck(centerrow, 2, deck, numDeck);
			if (numColorMatches > 0 && *numCards != 0) {
				printf("You color matched and get to play %d card(s) in the center row\n", numColorMatches);
				if (currTurn == P1) {
					printf("Player One's hand: ");
				}
				else {
					printf("Player Two's hand: ");
				}
				printDeck(*head);
			}
			if (numColorMatches == 1 && *numCards >= 1) {
				numCardPlayed = 0;
				while (numCardPlayed <= 0 || numCardPlayed > *numCards) {
					printf("Select one additional card from 1-%d to place to the center row: ", *numCards);
					scanf(" %d", &numCardPlayed);
				}
				addToCenter(centerrow, numCardPlayed, head);
				(*numCards)--;
				(*centerRowNum)++;
			}
			else if (numColorMatches == 2 && *numCards >= 2) {
				numCardPlayed = 0;
				numCardPlayed2 = 0;
				while (numCardPlayed <= 0 || numCardPlayed > *numCards && numCardPlayed2 <= 0 || numCardPlayed2 > *numCards) {
					printf("Select two addtional cards from 1-%d to place to the center row (Format: #,#): ", *numCards);
					scanf(" %d%*c%d", &numCardPlayed, &numCardPlayed2);
				}
				addToCenter(centerrow, numCardPlayed, head);
				numCardPlayed2--;
				addToCenter(centerrow, numCardPlayed2, head);
				*numCards -= 2;
				*centerRowNum += 2;
			}
			else if (numColorMatches == 3 && *numCards >= 3) {
				numCardPlayed3 = 0;
				numCardPlayed = 0;
				numCardPlayed2 = 0;
				while (numCardPlayed <= 0 || numCardPlayed > *numCards && numCardPlayed2 <= 0 || numCardPlayed2 > *numCards && numCardPlayed3 <= 0 || numCardPlayed3 > *numCards) {
					printf("Select three additional cards from 1-%d to place to the center row (Format: #,#,#): ", *numCards);
					scanf(" %d%*c%d%*c%d", &numCardPlayed, &numCardPlayed2, &numCardPlayed3);
				}
				addToCenter(centerrow, numCardPlayed, head);
				numCardPlayed2--;
				addToCenter(centerrow, numCardPlayed2, head);
				numCardPlayed3 -= 2;
				addToCenter(centerrow, numCardPlayed3, head);
				*numCards -= 3;
				*centerRowNum += 3;
			}
			else if (numColorMatches == 4 && *numCards >= 4) {
				int numCardPlayed4 = 0;
				numCardPlayed3 = 0;
				numCardPlayed2 = 0;
				numCardPlayed = 0;
				while (numCardPlayed <= 0 || numCardPlayed > *numCards && numCardPlayed2 <= 0 || numCardPlayed2 > *numCards && numCardPlayed3 <= 0 || numCardPlayed3 > *numCards && numCardPlayed4 <= 0 || numCardPlayed4 > *numCards) {
					printf("Select four additional cards from 1-%d to place to the center row (Format: #,#,#,#): ", *numCards);
					scanf(" %d%*c%d%*c%d%*c%d", &numCardPlayed, &numCardPlayed2, &numCardPlayed3, &numCardPlayed4);
				}
				addToCenter(centerrow, numCardPlayed, head);
				numCardPlayed2--;
				addToCenter(centerrow, numCardPlayed2, head);
				numCardPlayed3 -= 2;
				addToCenter(centerrow, numCardPlayed3, head);
				numCardPlayed4 -= 3;
				addToCenter(centerrow, numCardPlayed4, head);
				*numCards -= 4;
				*centerRowNum += 4;
			}
			else if (numColorMatches == 0) {
				printf("You made no color matches.\n");
			}
			else {
				printf("Your number of color matches exceeds the number of cards in your deck! Adding remaining cards to the centerrow.\n");
				while (*head != NULL) {
					addToCenter(centerrow, 1, head);
				}
				*centerRowNum += *numCards;
				*numCards = 0;
				endTurn = true;
				return;
			}
			centerPT = *centerrow;
			numColorMatches = 0;
			addedCard = false;
			viablePlays = false;
			endTurn = true;
			if (*numCards == 0) {
				endTurn = true;
				return;
			}
		}
	}
}

bool viablePlay(card* head, int numCardsPlayed, int numCardPlayed, int numCardPlayed2, card* centerrow) { //Checks if the play is viable
	card* cursor;
	cursor = head;
	if (numCardsPlayed == 1) {
		if (strcmp(centerrow->action, "#") == 0) {
			return true;
		}
		while (numCardPlayed != 0) {
			if (numCardPlayed == 1) {
				if (strcmp(cursor->action, "#") == 0) {
					return true;
				}
				if (centerrow->value == cursor->value) {
					return true;
				}
			}
			cursor = cursor->pt;
			numCardPlayed--;
		}
		return false;
	}
	else {
		int finalValue = 0;
		while (numCardPlayed != 0) {
			if (numCardPlayed == 1) {
				if (strcmp(cursor->action, "#") == 0) {
					return true;
				}
				finalValue += cursor->value;
			}
			cursor = cursor->pt;
			numCardPlayed--;
		}
		cursor = head;
		while (numCardPlayed2 != 0) {
			if (numCardPlayed2 == 1) {
				if (strcmp(cursor->action, "#") == 0) {
					return true;
				}
				finalValue += cursor->value;
			}
			cursor = cursor->pt;
			numCardPlayed2--;
		}
		if (finalValue > 10) {
			return false;
		} 
		else if (finalValue == centerrow->value) {
			return true;
		}

		if (strcmp(centerrow->action, "#") == 0) {
			return true;
		}
		return false;
	}
}

bool checkDoubleMatch(card* head, int numCardPlayed, int numCardPlayed2, card* centerrow) {
	card* cursor = head;
	card* cursor2 = head;

	for (int i = 1; i < numCardPlayed; i++) {
		cursor = cursor->pt;
	}

	for (int j = 1; j < numCardPlayed2; j++) {
		cursor2 = cursor2->pt;
	}

	if (strcmp(cursor->color, centerrow->color) == 0 && strcmp(cursor2->color, centerrow->color) == 0) {
		return true;
	}
	else if (strcmp(cursor->color, "anycolor") == 0 && strcmp(cursor2->color, centerrow->color) == 0) {
		return true;
	}
	else if (strcmp(cursor->color, centerrow->color) == 0 && strcmp(cursor2->color, "anycolor") == 0) {
		return true;
	}
	else if (strcmp(cursor->color, "anycolor") == 0 && strcmp(cursor2->color, "anycolor") == 0) {
		return true;
	}
	return false;
}

bool checkMatches(card* head, int numCards, card* centerrow, card* centerPT) { //Check for any matches in the deck
	//while (centerPT != NULL) {
		for (int i = 1; i <= numCards; i++) {
			if (viablePlay(head, 1, i, 0, centerPT)) {
				return true;
			}
			else {
				for (int j = i + 1; j <= numCards; j++) {
					if (viablePlay(head, 2, i, j, centerPT)) {
						return true;
					}
				}
			}
		}
		//centerPT = centerPT->pt;
	//}
	//centerPT = centerrow;
	return false;
}

void playCard(card** head, int numCardPlayed, int *colorMatch, card* centerrow) { //check if card aligns with centerrow and remove from deck
	card* cursor;
	card* previousCursor;
	cursor = *head;
	previousCursor = *head;
	if (numCardPlayed == 1) {
		*head = cursor->pt;
		if (strcmp(cursor->color, centerrow->color) == 0) {
			(*colorMatch)++;
		}
		else if (strcmp(cursor->color, "anycolor") == 0) {
			(*colorMatch)++;
		}
		else if (strcmp(centerrow->color, "anycolor") == 0) {
			(*colorMatch)++;
		}
		free(cursor);
		return;
	}

	for (int i = 1; i < numCardPlayed; i++) {
		previousCursor = cursor;
		cursor = cursor->pt;
	}
	if (strcmp(cursor->color, centerrow->color) == 0) {
		(*colorMatch)++;
	}
	else if (strcmp(cursor->color, "anycolor") == 0) {
		(*colorMatch)++;
	}
	else if (strcmp(centerrow->color, "anycolor") == 0) {
		(*colorMatch)++;
	}
	previousCursor->pt = cursor->pt;
	free(cursor);
	return;
}

void refreshCenter(card** centerrow) {
	while (*centerrow != NULL) {
		card* temp = *centerrow;
		*centerrow = (*centerrow)->pt;
		free(temp);
	}
}

void addToCenter(card** centerrow, int numCardPlayed, card** head) {
	card* temp = (card *)malloc(sizeof(card));
	card* cursor;
	card* previousCursor;
	cursor = *head;
	previousCursor = *head;
	temp->pt = NULL;

	if (numCardPlayed == 1) {
		strcpy(temp->color, cursor->color);
		if (strcmp(cursor->action, "#") == 0) {
			strcpy(temp->action, "#");
		}
		else {
			temp->value = cursor->value;
		}
		*head = cursor->pt;
		free(cursor);
		numCardPlayed--;
	}
	while (numCardPlayed != 0) {
		if (numCardPlayed == 1) {
			strcpy(temp->color, cursor->color);
			if (strcmp(cursor->action, "#") == 0) {
				strcpy(temp->action, "#");
			}
			else {
				temp->value = cursor->value;			
			}
			previousCursor->pt = cursor->pt;
			free(cursor);
			break;
		}
		previousCursor = cursor;
		cursor = cursor->pt;
		numCardPlayed--;
	}
	temp->pt = *centerrow;
	*centerrow = temp;
}

int calcPoints(card** head) {
	card* cursor; 
	cursor = *head;
	int points = 0;
	while (cursor != NULL) {
		if (strcmp(cursor->action, "#") == 0) {
			points += 40;
		}
		else if (cursor->value == 2) {
			points += 20;
		}
		else {
			points += cursor->value;
		}
		cursor = cursor->pt;
	}
	return points;
}

void freeGame(card** player1, card** player2, card** centerrow) {
	while (*player1 != NULL) {
		card* temp = *player1;
		*player1 = (*player1)->pt;
		free(temp);
	}
	while (*player2 != NULL) {
		card* temp = *player2;
		*player2 = (*player2)->pt;
		free(temp);
	}
	while (*centerrow != NULL) {
		card* temp = *centerrow;
		*centerrow = (*centerrow)->pt;
		free(temp);
	}
}

void swap(card deck[]) {
	int i = 0, j = 0;
	for (int n = 0; n < DECKSIZE; n++) {
		i = rand() % DECKSIZE;
		j = rand() % DECKSIZE;
		while (i == j) {
			i = rand() % DECKSIZE;
			j = rand() % DECKSIZE;
		}
		//Temp = i i = j j = temp
		card* temp = (card*)malloc(sizeof(card));
		strcpy(temp->color, deck[j].color);
		temp->value = deck[j].value;
		if (strcmp(deck[j].action, "#") == 0) {
			strcpy(temp->action, deck[j].action);
		}

		strcpy(deck[j].color, deck[i].color);
		deck[j].value = deck[i].value;
		if (strcmp(deck[i].action, "#") == 0) {
			strcpy(deck[j].action, deck[i].action);
		}

		strcpy(deck[i].color, temp->color);
		deck[i].value = temp->value;
		if (strcmp(temp->action, "#") == 0) {
			strcpy(deck[i].action, temp->action);
		}
	}
}