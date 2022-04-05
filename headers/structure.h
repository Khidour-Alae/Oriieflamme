#ifndef _STRUCTURE_H  
#define _STRUCTURE_H

#include "cards.h"
#include "faction.h"

#define NB_CARDS_IN_DECK 44
#define NB_CARDS_IN_HAND 8
#define SIZE_2DBOARD 1089 // (4*NB_CARDS_IN_HAND + 1)^2

// #######################################################
// ### The deck structure
// #######################################################

/**
* \brief The deck is a classic FILO structure
**/
typedef struct {
    card *c;
    int top;
} deck;

/**
* \brief Initializes the deck (mostly memory allocation)
* \param d is the deck
**/
void init_deck(deck *d);

/**
* \brief Check if there are cards in the deck
* \param d is the deck
* \return 0 if there is at least one card in the deck, 1 otherwise
**/
int isEmpty_deck(deck *d);

/**
* \brief Puts a card on top of the deck
* \param d is the deck
* \param c is the card
**/
void push_deck(card c, deck *d);

/**
* \brief Draws the card on top of the deck
* \param d is the deck
* \return the card that was on top of the deck
**/
card pop_deck(deck *d);

/**
* \brief Shuffles the deck using Fisher–Yates shuffle algorithm
* \param d is the deck
**/
void shuffle_deck(deck *d);

/**
* \brief Empty the deck
* \param d is the deck
**/
void reset_deck(deck *d);

/**
* \brief Deletes the deck (free allocated memory)
* \param d is the deck
**/
void delete_deck(deck *d);


// #######################################################
// ### The hand structure
// #######################################################

/**
* \brief The hand is a classic FILO structure
**/
typedef struct {
    card *c;
    int top;
} hand;

/**
* \brief Initializes the hand (mostly memory allocation)
* \param h is the hand
**/
void init_hand(hand *h);

/**
* \brief Check if there are cards in the hand
* \param h is the hand
* \return 0 if there is at least one card in the hand, 1 otherwise
**/
int isEmpty_hand(hand *h);

/**
* \brief Puts a card in the hand
* \param c is the card
* \param h is the hand
**/
void push_hand(card c, hand *h);

/**
* \brief discard the last card that was added to the hand
* \param h is the hand
* \return the last card that was added to the hand
**/
card pop_hand(hand *h);

/**
* \brief discard the nth card (the 1-th being the last card added to the hand)
* \param h is the hand
* \param n correspond to the number of the card you want to discard (the n-th card)
* \return the discarded card
**/
card popNthCard_hand(hand *h, int n);

/**
* \brief get the last card that was added to the hand
* \param h is the hand
* \return the last card that was added to the hand
**/
card getTopCard_hand(hand *h);

/**
* \brief get the n-th card (1-th being the last card added)
* \param h is the hand
* \param n correspond to the number of the card you want to get (the n-th card)
* \return the n-th card
**/
card getNthCard_hand(hand *h, int n);

/**
* \brief Empty the hand
* \param h is the hand
**/
void reset_hand(hand *h);

/**
* \brief Deletes the hand (free allocated memory)
* \param h is the hand
**/
void delete_hand(hand *h);


// #######################################################
// ### The board2D structure
// #######################################################

/**
* It's recommended to read the comments of board2D before.
* board2DBoundingBox defines the boundingBox of the board2D.
* It just represent a rectangle with two points :
* one point is the lowest left point
* one point is the highest right point
*
**/
typedef struct {
    int xmin; int ymin; int xmax; int ymax;
} board2DBoundingBox;

/**
* board2D is a complex structure. It represents the board,
* that is to say a 2D plane on which you can place cards 
* belonging to factions. The cards can only be placed on 
* integer coordinates (for example : (0,0), (3,2), (-1,-2)).
* 
* To do this, we make two 2D-arrays, one that store the cards
* and one that stores from which faction the card was placed.
* 
* The 2D-arrays are implemented as a 1D-array with the following
* transformation : 1DArray[sizeOfLine * y + x] <=> 2DArray[x][y]
* 
* To represent an infinite 2D plane, the arrays can be
* dynamically resized. For example, if the array covers the
* 2D plane for :
*   # -5 <= x <= 5
*   # -5 <= y <= 5
* and we need to place a card at (5,6), then the array will
* grow in order to cover enough of the 2D plane to place the card
*
* As the array could be covering unecessary parts of the 2D plane
* we are using a bounding box. For example :
* if the array is covering -50 <= x <= 50 and -50 <= y <= 50
* but there are cards only in -10 <= x <= 3 and -7 <= y <= 4
* then the bounding box is used to get this information
* This is usefull mostly for printing the board or iterating
* through the board.
*
**/
typedef struct {
    card *c;
    faction *f;
    board2DBoundingBox box;
    int sideLength; //the board is a square
    int sizeBoard2D; //the 1D-array length
} board2D;

/**
* \brief Initializes the board (mostly memory allocation)
* \param b2D is the board2D
**/
void init_board2D(board2D *b2D);

/**
* \brief Check if the board is empty
* \param b2D is the board2D
* \return 0 if there is at least one card placed on the board, 1 otherwise
**/
int isEmpty_board2D(board2D *b2D);

/**
* \brief get the position of the center of the board (the index of the 1D-array)
* \param b2D is the board2D
* \return the index that gives the center of the board
**/
int getCenter_board2D(board2D *b2D);

/**
* \brief get the card placed at coordinates ( \a x, \a y)
* \param b2D is the board2D
* \param x is the x-coordinate
* \param y is the y-coordinate
* \return NULL if there are no card placed at at coordinates ( \a x, \a y); return the card otherwise
**/
card getCard_board2D(board2D *b2D, int x, int y);

/**
* \brief get the faction that played the card placed at coordinates ( \a x, \a y)
* \param b2D is the board2D
* \param x is the x-coordinate
* \param y is the y-coordinate
* \return NULL if there are no card placed at coordinates ( \a x, \a y); return the faction that played the card otherwise
**/
faction getFaction_board2D(board2D *b2D, int x, int y);

/**
* \brief place a card \a c at coordinates ( \a x, \a y)
* \param b2D is the board2D
* \param c is the card
* \param f is the faction that played the card
* \param x is the x-coordinate at which you want to place the card
* \param y is the y-coordinate at which you want to place the card
**/
void addCard_board2D(board2D *b2D, card c, faction f, int x, int y);

/**
* \brief puts the board back to its original state (that is to say the empty board)
* \param b2D is the board2D
**/
void reset_board2D(board2D *b2D);

/**
* \brief Deletes the board2D (free allocated memory)
* \param b2D is the board2D
**/
void delete_board2D(board2D *b2D);

/**
* \brief get the bounding box of the board2D
* \param b2D is the board2D
* \param xmin is the x coordinate of the lowest left point of the bounding box (=rectangle)
* \param ymin is the y coordinate of the lowest left point of the bounding box (=rectangle)
* \param xmax is the x coordinate of the highest right point of the bounding box (=rectangle)
* \param ymax is the y coordinate of the highest right point of the bounding box (=rectangle)
**/
void getBoundingBox(board2D *b2D, int *xmin, int *ymin, int *xmax, int *ymax);

#endif