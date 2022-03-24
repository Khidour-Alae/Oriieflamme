#ifndef _STRUCTURE_H  
#define _STRUCTURE_H

#include "cards.h"
#include "faction.h"

#define NB_CARDS_IN_DECK 44
#define NB_CARDS_IN_HAND 8
#define SIZE_2DBOARD 1089

//deck structure
typedef struct {
    card *c;
    int top;
} deck;

void init_deck(deck *d);
int isEmpty_deck(deck d);
void push_deck(card c, deck *d);
card pop_deck(deck *d);
void shuffle_deck(deck *d);
void reset_deck(deck *d);
void delete_deck(deck *d);
//void print_deck(deck *d);


//hand structure
typedef struct {
    card *c;
    int top;
} hand;

void init_hand(hand *h);
int isEmpty_hand(hand h);
void push_hand(card c, hand *h);
card pop_hand(hand *h);
void reset_hand(hand *h);
void delete_hand(hand *h);
//void print_hand(hand *h);


//board structure
typedef struct coupleCardFaction
{
    card c;
    faction f;
} coupleCardFaction;
typedef coupleCardFaction board2D[SIZE_2DBOARD]; 

#endif