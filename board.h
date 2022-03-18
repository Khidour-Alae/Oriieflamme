#ifndef _BOARD_H
#define _BOARD_H

typedef struct board board;

int createBoard();

void freeBoard(board b);

int newRound(int counterRoundNumber, faction f, faction f);

int listFactions();

void putDownCard(board b, card c, faction f);

void flipCard(card c);

#endif 