#ifdef _INTERFACE_H
#define _INTERFACE_H

#include <board.h>
#include <card.h>
#include <faction.h>

void showBoard(board b) {} 

void showHand(faction f) {}

int askReshuffle(faction f) {}

card askCardWantToPlay(faction f) {}

int askWhereWantToPlaceCard(card c) {}

void showCardEffect(card c) {}

void showWinner(faction f) {}

#endif


