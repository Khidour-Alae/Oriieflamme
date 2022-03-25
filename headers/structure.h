#ifndef _STRUCTURE_H  
#define _STRUCTURE_H

#include "cards.h"
#include "faction.h"

#define NB_CARDS_IN_DECK 44
#define NB_CARDS_IN_HAND 8
#define SIZE_2DBOARD 1089 // (4*NB_CARDS_IN_HAND + 1)^2

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
card popNthCard_hand(hand *h, int n);
void reset_hand(hand *h);
void delete_hand(hand *h);
//void print_hand(hand *h);


//board2D structure
typedef struct {
    int pmin; int pmax;
} board2DBoundingBox;

typedef struct {
    card *c;
    faction *f;
    board2DBoundingBox box; //this box will be used for displaying the board
    int sideLength; //the board is will be a square
    int sizeBoard2D;
} board2D;

void init_board2D(board2D *b2D);
int isEmpty_board2D(board2D b2D);
int getCenter_board2D(board2D *b2D);
card getCard_board2D(board2D *b2D, int p);
faction getFaction_board2D(board2D *b2D, int p);
void addCard_board2D(board2D *b2D, card c, faction f, int pos);
void reset_board2D(board2D *b2D);
void delete_board2D(board2D *b2D);


//we need the board2D argument as the board can be dynamically extended
int getPositionFromCoordinates_board2D(board2D *b2D, int x, int y);
int getXFromPosition_board2D(board2D *b2D, int p);
int getYFromPosition_board2D(board2D *b2D, int p);


#endif