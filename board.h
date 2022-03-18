#include "faction.h"
#ifndef _BOARD_H
#define _BOARD_H

typedef struct board_base * board;

board createBoard();

void freeBoard(board *b);

int newRound(int counterRoundNumber, faction f1, faction f2);

faction* listFactions(board b);

void putDownCard(board *b, card c, faction f);

int flipCard(card *c);

#endif 