#ifndef _BOARD_H
#define _BOARD_H

typedef struct board board;

void createBoard(board *b, faction *f1, faction *f2);

void freeBoard(board b);

int newRound(int counterRoundNumber, faction f1, faction f2);

int listFactions();

void putDownCard(board b, card c, faction f);

int flipCard(card *c);

#endif 